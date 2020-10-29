/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsBorder.cpp: implementation of the mpsBorder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "mpsBorder.h"
#include "mpsSimBox.h"
#include "AbstractBead.h"

// ****************************************
// Global members 



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor for the base class of regions that bound the bulk of each
// processor's Space. 

mpsBorder::mpsBorder(mpsSimBox* const pSimBox, long lx, long ly, long lz, 
                     long xc, long yc, long zc): m_pSimBox(pSimBox),
                                                 m_LX(lx),
                                                 m_LY(ly),
                                                 m_LZ(lz),
                                                 m_CNTXCellNo(xc),
                                                 m_CNTYCellNo(yc),
                                                 m_CNTZCellNo(zc),
                                                 m_CNTCellTotal(xc*yc*zc)
{
    m_vCNTCells.clear();
    m_vExternalCNTCells.clear();
	m_vCNTCells.reserve(m_CNTCellTotal);
    m_vExternalCNTCells.reserve(m_CNTCellTotal);
    m_mmLostBeads.clear();

	for(long i=0; i<3; i++)
	{
	    m_TotalForce[i] = 0.0;
	    m_TotalVel[i]   = 0.0;
	}
}

// Copy constructor

mpsBorder::mpsBorder(const mpsBorder& oldBorder) : xxParallelBase(oldBorder), 
                                                   m_pSimBox(oldBorder.m_pSimBox),
                                                   m_LX(oldBorder.m_LX),
                                                   m_LY(oldBorder.m_LY),
                                                   m_LZ(oldBorder.m_LZ),
                                                   m_CNTXCellNo(oldBorder.m_CNTXCellNo),
                                                   m_CNTYCellNo(oldBorder.m_CNTYCellNo),
                                                   m_CNTZCellNo(oldBorder.m_CNTZCellNo),
                                                   m_CNTCellTotal(oldBorder.m_CNTCellTotal),
                                                   m_vCNTCells(oldBorder.m_vCNTCells),
                                                   m_vExternalCNTCells(oldBorder.m_vExternalCNTCells),
                                                   m_mmLostBeads(oldBorder.m_mmLostBeads)
{
	for(long i=0; i<3; i++)
	{
	    m_TotalForce[i] = oldBorder.m_TotalForce[i];
	    m_TotalVel[i]   = oldBorder.m_TotalVel[i];
	}
}
                           
mpsBorder::~mpsBorder()
{
    // Note that external CNT cells for all Border instances are owned by the mpsFace class.
}

// Function to return the 1d index into the main CNT cell array given the i,j,k coordinates.

long mpsBorder::GetCellIndexFromCoords(long i, long j, long k) const
{
    return i + j*m_LX + k*m_LX*m_LY;
}

// Overloaded function to return the 1d index into am arbitrary-sized CNT cell array. 
// It takes the i,j,k coordinates and the sizes of the CNT cell array, LX, LY, LZ.
// This allows border regions to determine the index of cells inside their own space.

long mpsBorder::GetCellIndexFromCoords(long i, long j, long k, long LX, long LY) const
{
    return i + j*LX + k*LX*LY;
}

// Overloaded function to return the 1d index into am arbitrary-sized CNT cell array 
// specified by the directions alpha, beta, gamma.
// It takes the i,j,k coordinates and the sizes of the CNT cell array, LX, LY, LZ.
// This allows border regions to determine the index of cells inside their own space.

long mpsBorder::GetCellIndexFromCoords(long i, long j, long k, long LX, long LY, long LZ,
                                       long alpha, long beta, long gamma) const
{
	long oldIndex, newIndex;

	oldIndex = i + j*LX + k*LX*LY;

// X test
	newIndex = oldIndex + alpha;
	if( newIndex >= LX*(1 + j + k*LY) )
		newIndex -= LX;
	else if( newIndex < LX*(j + k*LY) )
		newIndex += LX;

// Y test
	newIndex += beta*LX;
	if( newIndex >= (1 + k)*LX*LY )
		newIndex -= LX*LY;
	else if( newIndex < k*LX*LY )
		newIndex += LX*LY;

// Z test

	newIndex += gamma*LX*LY;
	if( newIndex >= LX*LY*LZ )
		newIndex -= LX*LY*LZ;
	else if( newIndex < 0 )
		newIndex += LX*LY*LZ;

	return newIndex;
}

// Function to empty the lost bead map

void mpsBorder::ClearLostBeadMap()
{
    m_mmLostBeads.clear();
}

// Function to return the number of lost beads for this Border instance.

long mpsBorder::GetLostBeadTotal() const
{
    return m_mmLostBeads.size();
}

// Function to return the number of lost beads that have a given processor id as
// their key.

long mpsBorder::GetLostBeadTotalForKey(long key) const
{
    return m_mmLostBeads.count(key);
}

// Function to add a bead to the lost beads map using the pid of the receiving
// processor as the key. We use a LongBeadMMap as the container.

void mpsBorder::AddLostBeadToMap(long key, CAbstractBead* pBead)
{
    m_mmLostBeads.insert(zPairLongBead(key, pBead));
}

// Overloaded function to add a bead to the lost beads map using the coordinate
// differences to calculate the neighbouring processor's pid that is the key.

void mpsBorder::AddLostBeadToMap(long alpha, long beta, long gamma, CAbstractBead* pBead)
{
    m_mmLostBeads.insert(zPairLongBead(GetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma), pBead));
}

// Overloaded function to add a bead to the lost beads map using the coordinate
// differences to calculate the neighbouring processor's pid, which is the key
// for the bead map, and to add the bead's containing CNT Cell id to a second map
// using the bead's id as the key

void mpsBorder::AddLostBeadToMap(long cntCellId, long alpha, long beta, long gamma, CAbstractBead* pBead)
{
    // Store the bead in one map
    AddLostBeadToMap(alpha, beta, gamma, pBead);

    // and its CNT cell in another map

    long beadId = pBead->GetId();

    m_mmLostBeadCells.insert(zPairLL(beadId, cntCellId));
}

// Function to return the pid of a neighbouring processor given its
// coordinate differences from the current processor.

long mpsBorder::GetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma) const
{
    return m_pSimBox->GetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma);
}

// Function to return a container holding those beads that have left the
// owning processor's Space and moved into the Space of the processor identified
// by the argument "key". Note that we return a a copy of the temporary vector.

AbstractBeadVector mpsBorder::GetLostBeadsForKey(long key)
{
    AbstractBeadVector vLostBeads;
    vLostBeads.clear();    // Clear local vector of lost beads

    LongBeadMMIterator iterBead = m_mmLostBeads.find(key);

    if(iterBead != m_mmLostBeads.end())
    {
        do
        {
            vLostBeads.push_back(iterBead->second);
            iterBead++;
        }
        while (iterBead != m_mmLostBeads.upper_bound(key));
    }       

    return vLostBeads;
}

// Function to return the number of beads contained in the Border region.

long mpsBorder::GetBeadTotal() const
{
    long total = 0;

    for(cCNTCellIterator citerCell=m_vCNTCells.begin(); citerCell!=m_vCNTCells.end(); citerCell++)
    {
        total += (*citerCell)->CellBeadTotal();
    }

    return total;
}

// Function to return a list containing pointers to all beads in the Border region.

BeadList mpsBorder::GetBeads() const
{
    BeadList lBeads;
    lBeads.clear();

    for(cCNTCellIterator citerCell=m_vCNTCells.begin(); citerCell!=m_vCNTCells.end(); citerCell++)
    {
        BeadList ltb;
        ltb = (*citerCell)->GetBeads();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }

    return lBeads;
}

// Function to empty the external CNT cells adjacent to this Border region so that 
// bead coordinates can be accumulated in them for calculating non-bonded bead-bead
// forces.

void mpsBorder::ClearExternalCells()
{
    for(CNTCellIterator iterCell=m_vExternalCNTCells.begin(); iterCell!=m_vExternalCNTCells.end(); iterCell++)
    {
        dynamic_cast<CExternalCNTCell*>(*iterCell)->Clear();
    }
}

// Function to return the container of external cells adjacent to this Border region.
// This is used to retrieve the forces on beads in the cells.

CNTCellVector& mpsBorder::GetExternalCNTCells()
{
    return m_vExternalCNTCells;
}

// Internal helper function to count the number of beads per CNT cell in this Border region.

double mpsBorder::CalculateBeadsPerCell() const
{
    if(GetCNTCellTotal() > 0)
	{
        return static_cast<double>(GetBeadTotal())/static_cast<double>(GetCNTCellTotal());
	}
	else
	{
	    return 0.0;
	}
}

// Internal helper function to count the number of bead-bead interactions for each bead in this Border region.

double mpsBorder::CalculateInteractionsPerBead()
{
    long total = 0;
	double mean = 0.0;
	
    BeadList lBeads = GetBeads();

    if(!lBeads.empty())
	{
	    for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	    {
            total += (*iterBead)->GetForceCounter();
	    }
        mean = static_cast<double>(total)/static_cast<double>(lBeads.size());
    }
	
    return mean;
}

// Internal helper function to calculate the total net force on all the beads on
// the Border region's Space.  We calculate the X,Y,Z components of the total
// force (conservative, dissipative and random) on the owned beads.

long mpsBorder::CalculateForceOnBeads()
{
	long total = 0;

	m_TotalForce[0] = 0.0;
	m_TotalForce[1] = 0.0;
	m_TotalForce[2] = 0.0;

    BeadList lBeads = GetBeads();

	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		total++;
		m_TotalForce[0] += (*iterBead)->GetXForce();
		m_TotalForce[1] += (*iterBead)->GetYForce();
		m_TotalForce[2] += (*iterBead)->GetZForce();
	}

	return total;
}

// Internal helper function to calculate the total net velocity on all the beads on
// the Border region's Space.  

long mpsBorder::CalculateVelocityOfBeads()
{
	long total = 0;

	m_TotalVel[0] = 0.0;
	m_TotalVel[1] = 0.0;
	m_TotalVel[2] = 0.0;

    BeadList lBeads = GetBeads();

	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		total++;
		m_TotalVel[0] += (*iterBead)->GetXMom();
		m_TotalVel[1] += (*iterBead)->GetYMom();
		m_TotalVel[2] += (*iterBead)->GetZMom();
	}

	return total;
}
