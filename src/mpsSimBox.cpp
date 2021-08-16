/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsSimBox.cpp: implementation of the mpsSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpsSimBox.h"
#include "SimAlgorithmFlags.h"
#include "SimMathFlags.h"
#include "IGlobalSimBox.h"
#include "ISimBox.h"
#include "mpsSimBox.h"
#include "Nanoparticle.h"

// MPI wrapper classes for World observables
#include "pmProcessorBeadAngularMomentum.h"
#include "pmProcessorBeadCMVelocity.h"
#include "pmProcessorBeadTotal.h"

// Log message classes for World observables
#include "LogpmAngularMomentum.h"
#include "LogpmBeadTotal.h"
#include "LogpmCMVelocity.h"


// ****************************************
// Global members to return information about a parallel experiment

long mpsSimBox::GlobalCellCellIntCounter = 0;
long mpsSimBox::GlobalCellCounter = 0;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that stores the containing processor's location within the 
// set of processors performing the simulation. The pids of its
// 26 neighbouring processors are stored in this class as the processors are
// wired up by the ISimBox instance.
//
// The data we know at the time of creation is:
//
// the total number of processors (the World), and the rank of the containing processor
// the number of processors in each space dimension
// the size of each SimBox copy in numbers of CNT cells per dimension
//
// The array of SimBox copies are connected according to a formula that allows 
// their location in the grid comprising the total simulation box to be determined 
// given just their rank and the World.
//
// P0 first defines the geometry of the World using the numbers of processors
// in each dimension.   We use 1, 2 or 3 for Linear, Planar or Cubic respectively.
// We also store the normal to the space, which is defined as the axis of a 
// Linear space, the normal to the plane for a Planar space, and the dimension
// containing the highest number of processors for a Cubic space. The special case
// of a single processor is given the geometry (3,0) and the case where a cubic 
// SimBox has the same number of processors in all dimensions is given (3,3), ie,
// we take the Z axis as the nominal normal.

mpsSimBox::mpsSimBox(long geometry, long normal, long px, long py, long pz, 
                     long lx, long ly, long lz, double dx, double dy, double dz,
                     CNTCellVector&  rvCNTCells,  PolymerVector&  rvBulkPolymers) : m_LX(lx), m_LY(ly), m_LZ(lz),
                                                  m_DX(dx), m_DY(dy), m_DZ(dz),
                                                  m_SimBoxXLength(static_cast<double>(lx)*dx),
                                                  m_SimBoxYLength(static_cast<double>(ly)*dy),
                                                  m_SimBoxZLength(static_cast<double>(lz)*dz),
                                                  m_rvCNTCells(rvCNTCells),
						    m_BeadTotal(0), m_ExtendedBondCounter(0), m_ExtendedBondForceCounter(0),
                                                  m_pR(0),         
                                                  m_pL(0),      
                                                  m_pT(0),      
                                                  m_pB(0),
                                                  m_pU(0),
                                                  m_pD(0),         
                                                  m_pTR(0),
                                                  m_pBR(0),      
                                                  m_pUR(0),         
                                                  m_pDR(0),        
                                                  m_pTL(0),         
                                                  m_pBL(0),        
                                                  m_pUL(0),         
                                                  m_pDL(0),        
                                                  m_pUT(0),         
                                                  m_pDT(0),        
                                                  m_pUB(0),       
                                                  m_pDB(0),         
                                                  m_pUTR(0),      
                                                  m_pDTR(0),     
                                                  m_pUBR(0),     
                                                  m_pDBR(0),     
                                                  m_pUTL(0),     
                                                  m_pDTL(0),     
                                                  m_pUBL(0),     
                                                  m_pDBL(0),
						       UFacePid(-1),
	                                              DFacePid(-1),
						        RFacePid(-1),
	                                              LFacePid(-1),
	                                              TFacePid(-1),
	                                              BFacePid(-1),
	                                              TREdgePid(-1),
	                                              TLEdgePid(-1),
	                                              BREdgePid(-1),
	                                              BLEdgePid(-1),
	                                              UREdgePid(-1),
	                                              ULEdgePid(-1),
	                                              UTEdgePid(-1),
	                                              UBEdgePid(-1),
	                                              DREdgePid(-1),
							DLEdgePid(-1),
	                                              DTEdgePid(-1),
	                                              DBEdgePid(-1),
	                                              UTRCornerPid(-1),
	                                              UTLCornerPid(-1),
	                                              UBRCornerPid(-1),
	                                              UBLCornerPid(-1),
	                                              DTRCornerPid(-1),
							DTLCornerPid(-1),
                                                  DBRCornerPid(-1),
                                                  DBLCornerPid(-1),
                                                  localBeadTotalTimer(0),
                                                  m_Geometry(geometry), m_Normal(normal),
					           m_BCTotal(((pow(3,geometry)-1)/2)*GetWorld()),
                                                  m_PX(px), m_PY(py), m_PZ(pz),
                                                  m_pi(0), m_pj(0), m_pk(0),
						   m_bFailed(false), m_bExternal(false),
						   velCounter(0), m_IntPerBeadCounter(0)
                     
{
    // We store an invalid value in the neighbour pid array, so that we can check that it is correctly filled in later.
    for(long i=0; i<27; i++)
    {
        m_NPid[i] = -1;
    }

    // Now store the coordinates of the containing processor given its rank
    // and the number of processors in each dimension of the World.

    m_pi = GetRank() % m_PX;
    m_pj = ((GetRank() - m_pi)/m_PX) % m_PY;
    m_pk = (((GetRank() - m_pi)/m_PX) - m_pj)/m_PY;

    // Store the coordinates of the SimBox origin in the whole simulation space
    // in the parallel base class for propagation to the rest of the code.

    xxParallelBase::m_SimBoxXOrigin = static_cast<double>(m_pi)*m_SimBoxXLength;
    xxParallelBase::m_SimBoxYOrigin = static_cast<double>(m_pj)*m_SimBoxYLength;
    xxParallelBase::m_SimBoxZOrigin = static_cast<double>(m_pk)*m_SimBoxZLength;
	
	// and a flag showing if the processor is on the simulation world boundary, 
    // and hence has to check the PBCs when moving particles.
	
	if( m_pi == 0 || m_pi == m_PX - 1 ||
	    m_pj == 0 || m_pj == m_PY - 1 ||
		m_pk == 0 || m_pk == m_PZ - 1)
	{
		m_bExternal = true;
	}
	    
	
//	std::cout << " Processor " << GetRank() << " has World geometry/normal " << GetGeometry() << " " << GetNormal() << zEndl;
//	std::cout << " Processor " << GetRank() << " has dimensions " << m_PX << " " << m_PY << " " << m_PZ << zEndl;
//	std::cout << " Processor " << GetRank() << " has coords     " << m_pi << " " << m_pj << " " << m_pk << zEndl;

    // Ensure the containers of CNT cells, CPolymers, and extended polymers are empty and create the initial number of 
	// polymers and empty extended polymers. Note that these in turn create a concrete polymer instance inside their constructor
	// and are responsbible for deleting it until, and if,  its wrapped polymer instance is moved to the bulk polymers container.

    m_vBulkCNTCells.clear();
	m_mExtendedPolymers.clear();
	
	m_lEmptyPolymers.clear();
	for(long ip=0; ip<m_InitialEmptyPolymers; ip++)
	{
	    m_lEmptyPolymers.push_back(new CPolymer());
	}
/*	
	m_lEmptyExtendedPolymers.clear();
	for(long ip=0; ip<m_InitialEmptyExtendedPolymers; ip++)
	{
	    m_lEmptyExtendedPolymers.push_back(new mpuExtendedPolymer());
	}
*/	
	// Now insert all the multi-bead polymers into the bulk polymers map using their unique id as the key. This map holds those 
	// polymers wholly contained within the processor's Space, and because the initial state forbids polymers from 
	// initially crossing the processor boundaries, this means all of the polymers in the SimBox.
	// Single-bead polymers are not inserted into the map as they cannot span processor boundaries, so do not need to be
	// moved between the bulk and extended polymer maps.
	
	m_mBulkPolymers.clear();
	for(PolymerVectorIterator iterPoly=rvBulkPolymers.begin(); iterPoly!=rvBulkPolymers.end(); iterPoly++)
	{
	    if((*iterPoly)->GetSize() > 1)
		{
		    m_mBulkPolymers.insert(zPairLongPolymer((*iterPoly)->GetId(), *iterPoly));
		}
	}
	
	// Clear the multimap that holds extended bonds.
	
	m_mmExtendedBonds.clear();
    
    // Clear the nanoparticle container. Because nanoparticles are only created as the result of a command, there will not 
    // be any when the mpsSimBox is constructed. They are passed in later via the CSimBox::CreateNanoparticle() function.
    // Also note that the CSimBox is responsible for destroying nanoparticle instances.
    
    m_mBulkNanoparticles.clear();

	// Initialise debug and logging storage and counters. These may be used by any derived class that wants to write out
	// information about each processor's set of beads or the whole set of beads in the simulations.
	
	localBeadTotalTimer = 0;  // Zero the timer for writing out the bead totals on each processor
	
	// Zero the counter and storage for the processors' CM velocities; then initialise the counter vectors.
	// Note that some of these are only used by P0, but we initialise them here anyway for all processors.
	
	m_vXCMVel.clear();
	m_vYCMVel.clear();
	m_vZCMVel.clear();
	
	for(long pi=0; pi<GetWorld(); pi++)
	{
	    m_vXCMVel.push_back(0.0);
	    m_vYCMVel.push_back(0.0);
	    m_vZCMVel.push_back(0.0);
	}
	
	for(long iv=0; iv<3; iv++)
	{
		m_AngMom[iv] = 0.0;
	    m_CMVel[iv]  = 0.0;
	}

    // Initialise the counters for collecting statistics on message passing.
	// The number of counters depends on the processor geometry, and we store
	// results for all processors. A description of each component of this vector
	// is given in the constructor of the derived classes

	for(long j=0; j<m_BCTotal; j++)
	{
	    m_vBeadCrossing.push_back(0);
	    m_vBeadCrossing2.push_back(0);
	}

    // Now initialise the debug data that calculates the mean of various bead properties for each Border region
	
    velCounter = 0;           // Zero the sampling counter
	m_IntPerBeadCounter = 0;  // Zero the number of samples counter
	
    m_MeanBulkBeadsPerCell   = 0.0;
	m_MeanSqBulkBeadsPerCell = 0.0;
	m_SDBulkBeadsPerCell     = 0.0;
	
    m_MeanBulkIntPerBead     = 0.0;
    m_MeanSqBulkIntPerBead   = 0.0;
    m_SDBulkIntPerBead       = 0.0;
	
	for(long in=0; in<6; in++)
	{
	    m_MeanFaceBeadsPerCell[in]     = 0.0;
	    m_MeanEdgeBeadsPerCell[in]     = 0.0;
	    m_MeanEdgeBeadsPerCell[6+in]   = 0.0;
	    m_MeanCornerBeadsPerCell[in]   = 0.0;
		
	    m_MeanSqFaceBeadsPerCell[in]   = 0.0;
        m_MeanSqEdgeBeadsPerCell[in]   = 0.0;
        m_MeanSqEdgeBeadsPerCell[6+in] = 0.0;
		m_MeanSqCornerBeadsPerCell[in] = 0.0;
		
	    m_SDFaceBeadsPerCell[in]       = 0.0;
        m_SDEdgeBeadsPerCell[in]       = 0.0;
        m_SDEdgeBeadsPerCell[6+in]     = 0.0;
		m_SDCornerBeadsPerCell[in]     = 0.0;
		
		
	    m_MeanFaceIntPerBead[in]       = 0.0;
        m_MeanEdgeIntPerBead[in]       = 0.0;
        m_MeanEdgeIntPerBead[6+in]     = 0.0;
		m_MeanCornerIntPerBead[in]     = 0.0;
		
	    m_MeanSqFaceIntPerBead[in]     = 0.0;
        m_MeanSqEdgeIntPerBead[in]     = 0.0;
        m_MeanSqEdgeIntPerBead[6+in]   = 0.0;
		m_MeanSqCornerIntPerBead[in]   = 0.0;
		
	    m_SDFaceIntPerBead[in]         = 0.0;
        m_SDEdgeIntPerBead[in]         = 0.0;
        m_SDEdgeIntPerBead[6+in]       = 0.0;
		m_SDCornerIntPerBead[in]       = 0.0;
	}
	
	m_MeanCornerBeadsPerCell[6]   = 0.0;
	m_MeanCornerBeadsPerCell[7]   = 0.0;
	m_MeanSqCornerBeadsPerCell[6] = 0.0;
	m_MeanSqCornerBeadsPerCell[7] = 0.0;
	m_SDCornerBeadsPerCell[6]     = 0.0;
	m_SDCornerBeadsPerCell[7]     = 0.0;

	m_MeanCornerIntPerBead[6]   = 0.0;
	m_MeanCornerIntPerBead[7]   = 0.0;
	m_MeanSqCornerIntPerBead[6] = 0.0;
	m_MeanSqCornerIntPerBead[7] = 0.0;
	m_SDCornerIntPerBead[6]     = 0.0;
	m_SDCornerIntPerBead[7]     = 0.0;

}

// Copy constructor

mpsSimBox::mpsSimBox(const mpsSimBox& oldSimBox) : m_LX(oldSimBox.m_LX),
                                                   m_LY(oldSimBox.m_LY),
                                                   m_LZ(oldSimBox.m_LZ),
                                                   m_DX(oldSimBox.m_DX),
                                                   m_DY(oldSimBox.m_DY),
                                                   m_DZ(oldSimBox.m_DZ),
                                                   m_SimBoxXLength(oldSimBox.m_SimBoxXLength),
                                                   m_SimBoxYLength(oldSimBox.m_SimBoxYLength),
                                                   m_SimBoxZLength(oldSimBox.m_SimBoxZLength),
                                                   m_rvCNTCells(oldSimBox.m_rvCNTCells),
						     m_BeadTotal(oldSimBox.m_BeadTotal),
					             m_ExtendedBondCounter(oldSimBox.m_ExtendedBondCounter),
						     m_ExtendedBondForceCounter(oldSimBox.m_ExtendedBondForceCounter),
                                                   m_pR(oldSimBox.m_pR),         
                                                   m_pL(oldSimBox.m_pL),      
                                                   m_pT(oldSimBox.m_pT),      
                                                   m_pB(oldSimBox.m_pB),
                                                   m_pU(oldSimBox.m_pU),
                                                   m_pD(oldSimBox.m_pD),         
                                                   m_pTR(oldSimBox.m_pTR),
                                                   m_pBR(oldSimBox.m_pBR),      
                                                   m_pUR(oldSimBox.m_pUR),         
                                                   m_pDR(oldSimBox.m_pDR),        
                                                   m_pTL(oldSimBox.m_pTL),
                                                   m_pBL(oldSimBox.m_pBL),      
                                                   m_pUL(oldSimBox.m_pUL),         
                                                   m_pDL(oldSimBox.m_pDL),        
                                                   m_pUT(oldSimBox.m_pUT),         
                                                   m_pDT(oldSimBox.m_pDT),        
                                                   m_pUB(oldSimBox.m_pUB),       
                                                   m_pDB(oldSimBox.m_pDB),         
                                                   m_pUTR(oldSimBox.m_pUTR),      
                                                   m_pDTR(oldSimBox.m_pDTR),     
                                                   m_pUBR(oldSimBox.m_pUBR),     
                                                   m_pDBR(oldSimBox.m_pDBR),     
                                                   m_pUTL(oldSimBox.m_pUTL),     
                                                   m_pDTL(oldSimBox.m_pDTL),     
                                                   m_pUBL(oldSimBox.m_pUBL),     
                                                   m_pDBL(oldSimBox.m_pDBL),
                                                   m_Geometry(oldSimBox.m_Geometry),
                                                   m_Normal(oldSimBox.m_Normal),
                                                   m_BCTotal(oldSimBox.m_BCTotal),
                                                   m_PX(oldSimBox.m_PX),
                                                   m_PY(oldSimBox.m_PY),
                                                   m_PZ(oldSimBox.m_PZ),
                                                   m_pi(oldSimBox.m_pi),
                                                   m_pj(oldSimBox.m_pj),
                                                   m_pk(oldSimBox.m_pk),
                      				     m_bFailed(oldSimBox.m_bFailed), 
                      				     m_bExternal(oldSimBox.m_bExternal), 
                      				     velCounter(0), m_IntPerBeadCounter(0)

{
    for(long i=0; i<27; i++)
    {
        m_NPid[i] = oldSimBox.m_NPid[i];
    }

    m_vBulkCNTCells             = oldSimBox.m_vBulkCNTCells;
    m_lEmptyPolymers            = oldSimBox.m_lEmptyPolymers;
    m_lEmptyExtendedPolymers    = oldSimBox.m_lEmptyExtendedPolymers;
    m_mBulkPolymers             = oldSimBox.m_mBulkPolymers;
    m_mExtendedPolymers         = oldSimBox.m_mExtendedPolymers;
    m_mmExtendedBonds           = oldSimBox.m_mmExtendedBonds;
    m_mBulkNanoparticles        = oldSimBox.m_mBulkNanoparticles;
	
	for(long iv=0; iv<3; iv++)
	{
	    m_CMVel[iv]  = oldSimBox.m_CMVel[iv];
	    m_AngMom[iv] = oldSimBox.m_AngMom[iv];
	}

}

mpsSimBox::~mpsSimBox()
{
    // Write out the statistics on the mean number of beads per cell and the interactions per bead for each of the Border regions and the Bulk SimBox.

    m_MeanBulkBeadsPerCell /= static_cast<double>(m_IntPerBeadCounter);
    m_MeanSqBulkBeadsPerCell /= static_cast<double>(m_IntPerBeadCounter);
	m_SDBulkBeadsPerCell = sqrt(m_MeanSqBulkBeadsPerCell - m_MeanBulkBeadsPerCell*m_MeanBulkBeadsPerCell);
	
    m_MeanBulkIntPerBead /= static_cast<double>(m_IntPerBeadCounter);
    m_MeanSqBulkIntPerBead /= static_cast<double>(m_IntPerBeadCounter);
	m_SDBulkIntPerBead = sqrt(m_MeanSqBulkIntPerBead - m_MeanBulkIntPerBead*m_MeanBulkIntPerBead);
	
	std::cout << "Proc " << GetRank() << " Bulk has beads/cell and interactions/bead (mean/sdev) = " << m_MeanBulkBeadsPerCell << " " << m_SDBulkBeadsPerCell << "   " << m_MeanBulkIntPerBead << " " << m_SDBulkIntPerBead << " for samples " << m_IntPerBeadCounter << zEndl;
	
	for(long inf=0; inf<6; inf++)
	{
	    m_MeanFaceBeadsPerCell[inf] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqFaceBeadsPerCell[inf] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDFaceBeadsPerCell[inf] = sqrt(m_MeanSqFaceBeadsPerCell[inf] - m_MeanFaceBeadsPerCell[inf]*m_MeanFaceBeadsPerCell[inf]);
		
	    m_MeanFaceIntPerBead[inf] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqFaceIntPerBead[inf] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDFaceIntPerBead[inf] = sqrt(m_MeanSqFaceIntPerBead[inf] - m_MeanFaceIntPerBead[inf]*m_MeanFaceIntPerBead[inf]);
	}

	std::cout << "Proc " << GetRank() << " U Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[0] <<  " " << m_SDFaceBeadsPerCell[0] << "   " << m_MeanFaceIntPerBead[0] <<  " " << m_SDFaceIntPerBead[0] << zEndl;
	std::cout << "Proc " << GetRank() << " D Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[1] <<  " " << m_SDFaceBeadsPerCell[1] << "   " << m_MeanFaceIntPerBead[1] <<  " " << m_SDFaceIntPerBead[1] << zEndl;
	std::cout << "Proc " << GetRank() << " T Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[2] <<  " " << m_SDFaceBeadsPerCell[2] << "   " << m_MeanFaceIntPerBead[2] <<  " " << m_SDFaceIntPerBead[2] << zEndl;
	std::cout << "Proc " << GetRank() << " B Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[3] <<  " " << m_SDFaceBeadsPerCell[3] << "   " << m_MeanFaceIntPerBead[3] <<  " " << m_SDFaceIntPerBead[3] << zEndl;
	std::cout << "Proc " << GetRank() << " R Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[4] <<  " " << m_SDFaceBeadsPerCell[4] << "   " << m_MeanFaceIntPerBead[4] <<  " " << m_SDFaceIntPerBead[4] << zEndl;
	std::cout << "Proc " << GetRank() << " L Face has beads/cell and interactions/bead (mean/sdev) = " << m_MeanFaceBeadsPerCell[5] <<  " " << m_SDFaceBeadsPerCell[5] << "   " << m_MeanFaceIntPerBead[5] <<  " " << m_SDFaceIntPerBead[5] << zEndl;

	for(long ine=0; ine<12; ine++)
	{
	    m_MeanEdgeBeadsPerCell[ine] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqEdgeBeadsPerCell[ine] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDEdgeBeadsPerCell[ine] = sqrt(m_MeanSqEdgeBeadsPerCell[ine] - m_MeanEdgeBeadsPerCell[ine]*m_MeanEdgeBeadsPerCell[ine]);
		
	    m_MeanEdgeIntPerBead[ine] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqEdgeIntPerBead[ine] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDEdgeIntPerBead[ine] = sqrt(m_MeanSqEdgeIntPerBead[ine] - m_MeanEdgeIntPerBead[ine]*m_MeanEdgeIntPerBead[ine]);
	}

	std::cout << "Proc " << GetRank() << " UT Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[0] << " " << m_SDEdgeBeadsPerCell[0] << "   " << m_MeanEdgeIntPerBead[0] << " " << m_SDEdgeIntPerBead[0] << zEndl;
	std::cout << "Proc " << GetRank() << " UB Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[1] << " " << m_SDEdgeBeadsPerCell[1] << "   " << m_MeanEdgeIntPerBead[1] << " " << m_SDEdgeIntPerBead[1] << zEndl;
	std::cout << "Proc " << GetRank() << " UR Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[2] << " " << m_SDEdgeBeadsPerCell[2] << "   " << m_MeanEdgeIntPerBead[2] << " " << m_SDEdgeIntPerBead[2] << zEndl;
	std::cout << "Proc " << GetRank() << " UL Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[3] << " " << m_SDEdgeBeadsPerCell[3] << "   " << m_MeanEdgeIntPerBead[3] << " " << m_SDEdgeIntPerBead[3] << zEndl;
	std::cout << "Proc " << GetRank() << " DT Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[4] << " " << m_SDEdgeBeadsPerCell[4] << "   " << m_MeanEdgeIntPerBead[4] << " " << m_SDEdgeIntPerBead[4] << zEndl;
	std::cout << "Proc " << GetRank() << " DB Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[5] << " " << m_SDEdgeBeadsPerCell[5] << "   " << m_MeanEdgeIntPerBead[5] << " " << m_SDEdgeIntPerBead[5] << zEndl;
	std::cout << "Proc " << GetRank() << " DR Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[6] << " " << m_SDEdgeBeadsPerCell[6] << "   " << m_MeanEdgeIntPerBead[6] << " " << m_SDEdgeIntPerBead[6] << zEndl;
	std::cout << "Proc " << GetRank() << " DL Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[7] << " " << m_SDEdgeBeadsPerCell[7] << "   " << m_MeanEdgeIntPerBead[7] << " " << m_SDEdgeIntPerBead[7] << zEndl;
	std::cout << "Proc " << GetRank() << " TR Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[8] << " " << m_SDEdgeBeadsPerCell[8] << "   " << m_MeanEdgeIntPerBead[8] << " " << m_SDEdgeIntPerBead[8] << zEndl;
	std::cout << "Proc " << GetRank() << " TL Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[9] << " " << m_SDEdgeBeadsPerCell[9] << "   " << m_MeanEdgeIntPerBead[9] << " " << m_SDEdgeIntPerBead[9] << zEndl;
	std::cout << "Proc " << GetRank() << " BR Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[10] << " " << m_SDEdgeBeadsPerCell[10] << "   " << m_MeanEdgeIntPerBead[10] << " " << m_SDEdgeIntPerBead[10] << zEndl;
	std::cout << "Proc " << GetRank() << " BL Edge has beads/cell and interactions/bead (mean/sdev) = " << m_MeanEdgeBeadsPerCell[11] << " " << m_SDEdgeBeadsPerCell[11] << "   " << m_MeanEdgeIntPerBead[11] << " " << m_SDEdgeIntPerBead[11] << zEndl;


	for(long inc=0; inc<8; inc++)
	{
	    m_MeanCornerBeadsPerCell[inc] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqCornerBeadsPerCell[inc] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDCornerBeadsPerCell[inc] = sqrt(m_MeanSqCornerBeadsPerCell[inc] - m_MeanCornerBeadsPerCell[inc]*m_MeanCornerBeadsPerCell[inc]);
		
	    m_MeanCornerIntPerBead[inc] /=  static_cast<double>(m_IntPerBeadCounter);
	    m_MeanSqCornerIntPerBead[inc] /=  static_cast<double>(m_IntPerBeadCounter);
		m_SDCornerIntPerBead[inc] = sqrt(m_MeanSqCornerIntPerBead[inc] - m_MeanCornerIntPerBead[inc]*m_MeanCornerIntPerBead[inc]);
	}

	std::cout << "Proc " << GetRank() << " UTR Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[0] << " " << m_SDCornerBeadsPerCell[0] << "   " << m_MeanCornerIntPerBead[0] << " " << m_SDCornerIntPerBead[0] << zEndl;
	std::cout << "Proc " << GetRank() << " UTL Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[1] << " " << m_SDCornerBeadsPerCell[1] << "   " << m_MeanCornerIntPerBead[1] << " " << m_SDCornerIntPerBead[1] << zEndl;
	std::cout << "Proc " << GetRank() << " UBR Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[2] << " " << m_SDCornerBeadsPerCell[2] << "   " << m_MeanCornerIntPerBead[2] << " " << m_SDCornerIntPerBead[2] << zEndl;
	std::cout << "Proc " << GetRank() << " UBL Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[3] << " " << m_SDCornerBeadsPerCell[3] << "   " << m_MeanCornerIntPerBead[3] << " " << m_SDCornerIntPerBead[3] << zEndl;
	std::cout << "Proc " << GetRank() << " DTR Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[4] << " " << m_SDCornerBeadsPerCell[4] << "   " << m_MeanCornerIntPerBead[4] << " " << m_SDCornerIntPerBead[4] << zEndl;
	std::cout << "Proc " << GetRank() << " DTL Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[5] << " " << m_SDCornerBeadsPerCell[5] << "   " << m_MeanCornerIntPerBead[5] << " " << m_SDCornerIntPerBead[5] << zEndl;
	std::cout << "Proc " << GetRank() << " DBR Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[6] << " " << m_SDCornerBeadsPerCell[6] << "   " << m_MeanCornerIntPerBead[6] << " " << m_SDCornerIntPerBead[6] << zEndl;
	std::cout << "Proc " << GetRank() << " DBL Corner has beads/cell and interactions/bead (mean/sdev) = " << m_MeanCornerBeadsPerCell[7] << " " << m_SDCornerBeadsPerCell[7] << "   " << m_MeanCornerIntPerBead[7] << " " << m_SDCornerIntPerBead[7] << zEndl;

    // Destroy the contained mpsBorder instances: we don't check for existence as they
    // are created automatically in the constructor so they must exist.

    delete m_pR;
    delete m_pL;
    delete m_pT;
    delete m_pB;
    delete m_pU;
    delete m_pD;
    delete m_pTR;
    delete m_pBR;
    delete m_pUR;
    delete m_pDR;
    delete m_pTL;
    delete m_pBL;
    delete m_pUL;
    delete m_pDL;
    delete m_pUT;
    delete m_pDT;
    delete m_pUB;
    delete m_pDB;
    delete m_pUTR;
    delete m_pDTR;
    delete m_pUBR;
    delete m_pDBR;
    delete m_pUTL;
    delete m_pDTL;
    delete m_pUBL;
    delete m_pDBL;
	
	// Delete the polymer instances in the SimBox that in turn deletes their beads, bonds and bondpairs.
	// This has to be done here as the number of polymers (and, therefore, beads, bonds, etc) is not constant
	// for each processor as it is in the serial code.
	
	long deadPolymerCount = 0;
	long deadExtendedPolymerCount = 0;
	
//	CPolymer* pPolymer;
	mpuExtendedPolymer* pExtPoly;
	
	if(m_mBulkPolymers.size() > 0)
	{
	    deadPolymerCount += m_mBulkPolymers.size();
	    std::cout << "Proc " << GetRank() << " destroying its " << m_mBulkPolymers.size() << " bulk polymers" << zEndl;
	    for(LongPolymerIterator iterMapPoly=m_mBulkPolymers.begin(); iterMapPoly!=m_mBulkPolymers.end(); iterMapPoly++)
	    {
//	        pPolymer = iterMapPoly->second;
//		    delete pPolymer;
	    }
	    std::cout << zEndl;
	    m_mBulkPolymers.clear();
	}
	else
	{
	    std::cout << "Proc " << GetRank() << " has no bulk polymers" << zEndl;
	}
	
	if(m_mExtendedPolymers.size() > 0)
	{
	    deadExtendedPolymerCount += m_mExtendedPolymers.size();
	    std::cout << "Proc " << GetRank() << " destroying its " << m_mExtendedPolymers.size() << " extended polymers" << zEndl;
	    for(LongExtendedPolymerIterator iterMapExtPoly=m_mExtendedPolymers.begin(); iterMapExtPoly!= m_mExtendedPolymers.end(); iterMapExtPoly++)
	    {
	        pExtPoly = iterMapExtPoly->second;
		    delete pExtPoly;
	    }
		std::cout << zEndl;
	    m_mExtendedPolymers.clear();
	}
	else
	{
	    std::cout << "Proc " << GetRank() << " has no extended polymers" << zEndl;
	}
	
	// and the empty polymer instances
	
	if(m_lEmptyPolymers.size() > 0)
	{
	    deadPolymerCount += m_lEmptyPolymers.size();
	    std::cout << "Proc " << GetRank() << " destroying its " << m_lEmptyPolymers.size() << " empty polymers" << zEndl;
	    for (PolymerListIterator iterPoly=m_lEmptyPolymers.begin(); iterPoly!= m_lEmptyPolymers.end(); iterPoly++)
	    {
	        delete *iterPoly;
	    }
	    m_lEmptyPolymers.clear();
	}
	else
	{
	    std::cout << "Proc " << GetRank() << " has no empty polymers" << zEndl;
	}
	
	// and the empty extended polymers that have been created. Note that there may be more than the initial
	// default number so we iterate over the whole container.
	
	if(m_lEmptyExtendedPolymers.size() > 0)
	{
	    deadExtendedPolymerCount += m_lEmptyExtendedPolymers.size();
	    std::cout << "Proc " << GetRank() << " destroying its " << m_lEmptyExtendedPolymers.size() << " empty extended polymers" << zEndl;
	    for (ExtendedPolymerListIterator iterExtPoly=m_lEmptyExtendedPolymers.begin(); iterExtPoly!= m_lEmptyExtendedPolymers.end(); iterExtPoly++)
	    {
	        delete *iterExtPoly;
	    }
	    m_lEmptyExtendedPolymers.clear();
	}
	else
	{
	    std::cout << "Proc " << GetRank() << " has no empty extended polymers" << zEndl;
	}
	
	std::cout << "Proc " << GetRank() << " has destroyed " << deadPolymerCount << " " <<  deadExtendedPolymerCount << " polymers and extended polymers respectively" << zEndl;
	
	// TBD - Do I need to delete the extended bond instances here or is it taken care of elsewhere?
	
	
}

// Function to return the rank of a neighbouring processor given its position in the nearest-neighbors list.

long mpsSimBox::GetNeighbourPidFromSlot(long index) const
{
    if(index >= 0 && index < 27)
    {
        return m_NPid[index];
    }
    else
    {
        return GetRank();
    }
}

// Function to return the pid of a neighbouring processor given the containing
// processor's coordinates and the direction parameters alpha, beta, gamma. 
// If an error occurs with the values, the calling routine must handle the error 
// condition as there is no way of telling here whether a processor id is valid or not.
// The pid is defined as the rank of a processor as returned by the MPI system,
// and the total number of processors is m_LX*m_LY*m_LZ.

long mpsSimBox::GetNeighbourPidFromLocation(long i, long j, long k, long alpha, long beta, long gamma)
{
	long oldIndex, newIndex;

	oldIndex = i + j*m_PX + k*m_PX*m_PY;

// X test
	newIndex = oldIndex + alpha;
	if( newIndex >= m_PX*(1 + j + k*m_PY) )
		newIndex -= m_PX;
	else if( newIndex < m_PX*(j + k*m_PY) )
		newIndex += m_PX;

// Y test
	newIndex += beta*m_PX;
	if( newIndex >= (1 + k)*m_PX*m_PY )
		newIndex -= m_PX*m_PY;
	else if( newIndex < k*m_PX*m_PY )
		newIndex += m_PX*m_PY;

// Z test

	newIndex += gamma*m_PX*m_PY;
	if( newIndex >= m_PX*m_PY*m_PZ )
		newIndex -= m_PX*m_PY*m_PZ;
	else if( newIndex < 0 )
		newIndex += m_PX*m_PY*m_PZ;

    return newIndex;
}

// Helper function to calculate the index into the m_NPD array for a neighbouring
// processor given the difference in its coordinates from the containing processor.
// The processors are labelled  with the alpha, beta, gamma parameters taking the
// values -1, 0, 1 in turn. The return value is the entry in the array that holds
// the pid for the processor in the direction specified by alpha, beta, gamma.
// 
long mpsSimBox::GetPidIndexFromCoordinateDifferences(long alpha, long beta, long gamma) const
{
    return 13 + alpha + 3*beta + 9*gamma;
}

// Function to retrieve the stored pid for a processor whose coordinates are
// specified relative to the central processor in a cube. If the parameters
// alpha, beta, gamma lead to an invalid index, we return an invalid pid to allow the error
// to be trapped.

long mpsSimBox::GetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma) const
{
    const long index = GetPidIndexFromCoordinateDifferences(alpha, beta, gamma);

    if(index >= 0 && index < 27)
    {
        return m_NPid[index];
    }
    else
    {
        return -1;
    }
}

// Function to store the proxy of a neighbouring processor whose location is 
// specified relative to the containing processor by the three parameters 
// alpha, beta, and gamma. If these parameters result in an illegal index
// (ie a number not in the range 0 to 26), we return false, otherwise we 
// return true. The calling routine must handle the error condition.
// Note that we cannot tell here whether the containing processor's location
// is valid or not, so we just store the result.

bool mpsSimBox::SetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma)
{
    bool bSuccess = true;

    const long pid = GetNeighbourPidFromLocation(m_pi, m_pj, m_pk, alpha, beta, gamma);
    const long index = GetPidIndexFromCoordinateDifferences(alpha, beta, gamma);

    if(index >= 0 && index < 27)
    {
        m_NPid[index] = pid;
//		std::cout << "Processor " << GetRank() << " (coordinates " << m_pi << " " << m_pj << " " << m_pk << " ) has neighbour " << pid << " in slot " << index << " at displacements " << alpha << " " << beta << " " << gamma << zEndl;
    }
    else
    {
        // Illegal direction differences mean that we have no valid value for 
        // the pid, so we cannot store a value.

       bSuccess = false;
    }

    return bSuccess;
}

// Function to return the 1d index into the CNT cell array given the i,j,k coordinates.

long mpsSimBox::GetCellIndexFromCoords(long i, long j, long k) const
{
    return i + j*m_LX + k*m_LX*m_LY;
}

// ****************************************
// Function to update the position coordinates of all beads in the processor's Bulk
// volume and Border regions. The first step does not require any inter-processor 
// communication, but it must be completed before updating the Border region beads. 
// Because the Border regions contain pointers to CNT cells, any beads that move 
// from a Bulk cell into a Border cell (or vice versa) on the same processor will 
// automatically belong to the Border (or Bulk) cell: we don't have to explicitly 
// update its ownership.
// 
// At the end of this function call, all beads that started in the processor's
// Bulk region will be correctly positioned therein or in one of the Border regions,
// and all beads that started in its Border regions will have been repositioned or
// sent to the appropriate neighbouring processor.
//
// All moved beads will have their SetMovable flag false indicating that they
// have already been moved in this timestep.

void mpsSimBox::UpdatePos()
{
/*	for(cCNTCellIterator iterCell1=m_rvCNTCells.begin(); iterCell1!=m_rvCNTCells.end(); iterCell1++)
	{
		std::cout << "Proc " << GetRank() << " Cell " << (*iterCell1)->GetId() << " contains " << (*iterCell1)->CellBeadTotal() << " beads with ids:  ";
		
		BeadList lBeads = (*iterCell1)->GetBeads();
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			std::cout << (*iterBead)->GetId() << " ";
		}
		std::cout << zEndl;
	} 
*/

	for(CNTCellIterator iterCell=m_vBulkCNTCells.begin(); iterCell!=m_vBulkCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdatePos();
	} 
    
    // Check if any Bulk nanoparticles should be moved to the Border container or any extended nanoparticles changed back to Bulk ones
    
    UpdateBulkNanoparticles();
    
    UpdateBorderNanoparticles();

    // Now call the derived class' function to update Border beads
	
    UpdateBorderPos();
	
	// Write out the number of extended bonds that cross processor boundaries to check the bond force calculation
	
		
//	if(GetRank() == 0)
//	    std::cout << "Proc " << GetRank() << "  at time " << IGlobalSimBox::Instance()->GetCurrentTime() << zEndl;

//	if(m_mmExtendedBonds.size() > 0)
//	{
//	    ListExtendedBonds();
//	}
	
//	ListExtendedPolymer(2771);

}

// ****************************************
// Function called by the SimBox to calculate the total force on all beads in the 
// processor's Space, both its Bulk volume and Border regions. The first step does not 
// require inter-processor communication for non-bonded forces (assuming they are 
// short-ranged) and is done here directly on the contained CNT cells; but the Border
// region calculation differs depending on the geometry of the simulation Space and
// is deferred to a PVF in the derived classes: UpdateBorderForce().
//
// Inter-processor communication is needed for the following forces even on Bulk beads:
//
// A) Bonds and BondPairs with beads in different processors
// B) Electrostatic forces between beads in different processors
//
// Bond forces are calculated differently for the following three cases:
//  1) Bulk polymer - all beads in a polymer are in the same processor's Space so the normal bond calculation is used
//  2) Extended polymer with an extended bond's beads in the same processor's Space - the force due to an extended bond both of whose
//     beads are in the same processor's Space is calculated by mpuExtendedBond
//  3) Extended polymer with an extended bond that spans a processor boundary - each processor passes bead coordinates for those beads
//     belonging to extended bonds to the U,T,R processors so that they can calculate bonded forces at the same time as they calculate
//     non-bonded forces between beads in adjacent processors/
//
//
 
void mpsSimBox::UpdateForce()
{
    // Zero the global cell-cell interaction counter 
	
    GlobalCellCounter = 0;
    GlobalCellCellIntCounter = 0;

    // Non-bonded bead forces in the bulk
	
	for(CNTCellIterator iterCell=m_vBulkCNTCells.begin(); iterCell!=m_vBulkCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateForce();
	} 
	
	// Non-bonded and bonded forces across processor boundaries
	
    UpdateBorderForce();
	
	// Bond forces for bulk polymers: we use the standard bond function here as we know
    // that both beads in the bond are in this processor's Space

	for(LongPolymerIterator iterPoly=m_mBulkPolymers.begin(); iterPoly!=m_mBulkPolymers.end(); iterPoly++)
	{
		(iterPoly->second)->AddBondForces();
	}

	// Bond forces due to bonds in extended polymers both of whose beads are in the same processor's Space
	
	for(LongExtendedPolymerIterator iterExtPoly=m_mExtendedPolymers.begin(); iterExtPoly!=m_mExtendedPolymers.end(); iterExtPoly++)
	{
		(iterExtPoly->second)->AddBondForces();
	}
		
    // Add the bond forces from dynamic bonds in nanoparticles: extended nanoparticles that cross the processor boundaries are 
    // dealt with later.
    
	for(LongNanoparticleIterator iterNano=m_mBulkNanoparticles.begin(); iterNano!=m_mBulkNanoparticles.end(); iterNano++)
	{
		(iterNano->second)->AddBondForces();
	}
    
    // Next, add the forces due to nanoparticles that may cross processor boundaries: these NPs contain concrete bonds that are still wholly 
    // in the owning processor's Space and extended bonds that have beads in two processor's Spaces. Each mpuExtendedNanoparticle instance 
    // wraps a concrete CNanoparticle instance, and handles the force calculations for concrete and extended bonds within it.
    
	for(LongExtendedNanoparticleIterator iterExtNano=m_mBorderNanoparticles.begin(); iterExtNano!=m_mBorderNanoparticles.end(); iterExtNano++)
	{
		(iterExtNano->second)->AddBondForces();
	}
    
	// Write out the Face data every so often for debug purposes.
	
    velCounter++;
	
	if(velCounter == 100000)
	{	
	    velCounter = 0;
		m_IntPerBeadCounter++;
				
		// First calculate the average number of non-bonded interactions per bead in the bulk of the simbox
		
		double beadsPerCell     = 0.0;
		double beadTotal        = 0.0;
		double totalIntPerBead  = 0.0;
		double meanBeadsPerCell = 0.0;
		double meanIntPerBead   = 0.0;
		
	    for(CNTCellIterator iterCell1=m_vBulkCNTCells.begin(); iterCell1!=m_vBulkCNTCells.end(); iterCell1++)
	    {
            BeadList lBeads = (*iterCell1)->GetBeads();
			beadsPerCell = lBeads.size();
            beadTotal += beadsPerCell;
			
	        for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	        {
                totalIntPerBead += (*iterBead)->GetForceCounter();
	        }
	    } 
	
	    meanBeadsPerCell = static_cast<double>(beadTotal)/static_cast<double>(m_vBulkCNTCells.size());
		m_MeanBulkBeadsPerCell += meanBeadsPerCell;
		m_MeanSqBulkBeadsPerCell += meanBeadsPerCell*meanBeadsPerCell;
		
	    meanIntPerBead   = static_cast<double>(totalIntPerBead)/static_cast<double>(beadTotal);		
		m_MeanBulkIntPerBead += meanIntPerBead;
		m_MeanSqBulkIntPerBead += meanIntPerBead*meanIntPerBead;
		
//	    std::cout << "P " << GetRank() << " Bulk SimBox has beads/cell and interactions/bead " << meanBeadsPerCell << " " << meanIntPerBead << zEndl;
		
	    // Now do the same for the Border regions
	
//	    double fx = 0.0;
//	    double fy = 0.0;
//	    double fz = 0.0;
		
	    double fcounter = 0.0;
//	    long fn = 0;
//	    long vn = 0;
		
    // First the Faces

	beadsPerCell = m_pU->CalculateBeadsPerCell();
	fcounter     = m_pU->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[0] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[0] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[0] += fcounter;
	m_MeanSqFaceIntPerBead[0] += fcounter*fcounter;
//	fn = m_pU->CalculateForceOnBeads();
//	vn = m_pU->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " U Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pU->GetBeadXForce() << " " <<  m_pU->GetBeadYForce() << " "  << m_pU->GetBeadZForce() << " " <<  m_pU->GetBeadXVel() << " " <<  m_pU->GetBeadYVel() << " " <<  m_pU->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pD->CalculateBeadsPerCell();
	fcounter     = m_pD->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[1] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[1] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[1] += fcounter;
	m_MeanSqFaceIntPerBead[1] += fcounter*fcounter;
//	fn = m_pD->CalculateForceOnBeads();
//	vn = m_pD->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " D Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pD->GetBeadXForce() << " " <<  m_pD->GetBeadYForce() << " "  << m_pD->GetBeadZForce() << " " <<  m_pD->GetBeadXVel() << " " <<  m_pD->GetBeadYVel() << " " <<  m_pD->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pT->CalculateBeadsPerCell();
	fcounter     = m_pT->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[2] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[2] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[2] += fcounter;
	m_MeanSqFaceIntPerBead[2] += fcounter*fcounter;
//	fn = m_pT->CalculateForceOnBeads();
//	vn = m_pT->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " T Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pT->GetBeadXForce() << " " <<  m_pT->GetBeadYForce() << " "  << m_pT->GetBeadZForce() << " " <<  m_pT->GetBeadXVel() << " " <<  m_pT->GetBeadYVel() << " " <<  m_pT->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pB->CalculateBeadsPerCell();
	fcounter     = m_pB->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[3] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[3] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[3] += fcounter;
	m_MeanSqFaceIntPerBead[3] += fcounter*fcounter;
//	fn = m_pB->CalculateForceOnBeads();
//	vn = m_pB->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " B Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pB->GetBeadXForce() << " " <<  m_pB->GetBeadYForce() << " "  << m_pB->GetBeadZForce() << " " <<  m_pB->GetBeadXVel() << " " <<  m_pB->GetBeadYVel() << " " <<  m_pB->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pR->CalculateBeadsPerCell();
	fcounter     = m_pR->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[4] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[4] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[4] += fcounter;
	m_MeanSqFaceIntPerBead[4] += fcounter*fcounter;
//	fn = m_pR->CalculateForceOnBeads();
//	vn = m_pR->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " R Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pR->GetBeadXForce() << " " <<  m_pR->GetBeadYForce() << " "  << m_pR->GetBeadZForce() << " " <<  m_pR->GetBeadXVel() << " " <<  m_pR->GetBeadYVel() << " " <<  m_pR->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pL->CalculateBeadsPerCell();
	fcounter     = m_pL->CalculateInteractionsPerBead();
	
	m_MeanFaceBeadsPerCell[5] += beadsPerCell;
	m_MeanSqFaceBeadsPerCell[5] += beadsPerCell*beadsPerCell;	
	m_MeanFaceIntPerBead[5] += fcounter;
	m_MeanSqFaceIntPerBead[5] += fcounter*fcounter;
//	fn = m_pL->CalculateForceOnBeads();
//	vn = m_pL->CalculateVelocityOfBeads();
	
//	std::cout << "P " << GetRank() << " L Face has " << beadsPerCell << " " << fcounter << " beads/cell and interactions/bead,  total/force/velocity = " << fn << " " << vn << " " << m_pL->GetBeadXForce() << " " <<  m_pL->GetBeadYForce() << " "  << m_pL->GetBeadZForce() << " " <<  m_pL->GetBeadXVel() << " " <<  m_pL->GetBeadYVel() << " " <<  m_pL->GetBeadZVel() << zEndl;
	
	// Now the Edges
	
	beadsPerCell = m_pUT->CalculateBeadsPerCell();
	fcounter     = m_pUT->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[0] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[0] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[0] += fcounter;
	m_MeanSqEdgeIntPerBead[0] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UT Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pUB->CalculateBeadsPerCell();
	fcounter     = m_pUB->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[1] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[1] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[1] += fcounter;
	m_MeanSqEdgeIntPerBead[1] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UB Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pUR->CalculateBeadsPerCell();
	fcounter     = m_pUR->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[2] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[2] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[2] += fcounter;
	m_MeanSqEdgeIntPerBead[2] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UR Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pUL->CalculateBeadsPerCell();
	fcounter     = m_pUL->CalculateInteractionsPerBead();
		
	m_MeanEdgeBeadsPerCell[3] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[3] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[3] += fcounter;
	m_MeanSqEdgeIntPerBead[3] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UL Edge has " << beadsPerCell << " " << fcounter << zEndl;
		
	beadsPerCell = m_pDT->CalculateBeadsPerCell();
	fcounter     = m_pDT->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[4] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[4] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[4] += fcounter;
	m_MeanSqEdgeIntPerBead[4] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DT Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pDB->CalculateBeadsPerCell();
	fcounter     = m_pDB->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[5] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[5] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[5] += fcounter;
	m_MeanSqEdgeIntPerBead[5] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DB Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pDR->CalculateBeadsPerCell();
	fcounter     = m_pDR->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[6] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[6] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[6] += fcounter;
	m_MeanSqEdgeIntPerBead[6] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DR Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pDL->CalculateBeadsPerCell();
	fcounter     = m_pDL->CalculateInteractionsPerBead();
		
	m_MeanEdgeBeadsPerCell[7] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[7] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[7] += fcounter;
	m_MeanSqEdgeIntPerBead[7] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DL Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pTR->CalculateBeadsPerCell();
	fcounter     = m_pTR->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[8] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[8] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[8] += fcounter;
	m_MeanSqEdgeIntPerBead[8] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " TR Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pTL->CalculateBeadsPerCell();
	fcounter     = m_pTL->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[9] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[9] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[9] += fcounter;
	m_MeanSqEdgeIntPerBead[9] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " TL Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pBR->CalculateBeadsPerCell();
	fcounter     = m_pBR->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[10] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[10] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[10] += fcounter;
	m_MeanSqEdgeIntPerBead[10] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " BR Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	beadsPerCell = m_pBL->CalculateBeadsPerCell();
	fcounter     = m_pBL->CalculateInteractionsPerBead();	
	
	m_MeanEdgeBeadsPerCell[11] += beadsPerCell;
	m_MeanSqEdgeBeadsPerCell[11] += beadsPerCell*beadsPerCell;
	m_MeanEdgeIntPerBead[11] += fcounter;
	m_MeanSqEdgeIntPerBead[11] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " BL Edge has " << beadsPerCell << " " << fcounter << zEndl;
	
	// Finally the Corners
	
	beadsPerCell = m_pUTR->CalculateBeadsPerCell();
	fcounter = m_pUTR->CalculateInteractionsPerBead();	
//	fn = m_pUTR->CalculateForceOnBeads();
//	vn = m_pUTR->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[0] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[0] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[0] += fcounter;
	m_MeanSqCornerIntPerBead[0] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UTR Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pUTR->GetBeadXForce() << " " <<  m_pUTR->GetBeadYForce() << " "  << m_pUTR->GetBeadZForce() << " " <<  m_pUTR->GetBeadXVel() << " " <<  m_pUTR->GetBeadYVel() << " " <<  m_pUTR->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pUTL->CalculateBeadsPerCell();
	fcounter = m_pUTL->CalculateInteractionsPerBead();	
//	fn = m_pUTL->CalculateForceOnBeads();
//	vn = m_pUTL->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[1] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[1] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[1] += fcounter;
	m_MeanSqCornerIntPerBead[1] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UTL Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pUTL->GetBeadXForce() << " " <<  m_pUTL->GetBeadYForce() << " "  << m_pUTL->GetBeadZForce() << " " <<  m_pUTL->GetBeadXVel() << " " <<  m_pUTL->GetBeadYVel() << " " <<  m_pUTL->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pUBR->CalculateBeadsPerCell();
	fcounter = m_pUBR->CalculateInteractionsPerBead();	
//	fn = m_pUBR->CalculateForceOnBeads();
//	vn = m_pUBR->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[2] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[2] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[2] += fcounter;
	m_MeanSqCornerIntPerBead[2] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UBR Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pUBR->GetBeadXForce() << " " <<  m_pUBR->GetBeadYForce() << " "  << m_pUBR->GetBeadZForce() << " " <<  m_pUBR->GetBeadXVel() << " " <<  m_pUBR->GetBeadYVel() << " " <<  m_pUBR->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pUBL->CalculateBeadsPerCell();
	fcounter = m_pUBL->CalculateInteractionsPerBead();	
//	fn = m_pUBL->CalculateForceOnBeads();
//	vn = m_pUBL->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[3] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[3] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[3] += fcounter;
	m_MeanSqCornerIntPerBead[3] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " UBL Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pUBL->GetBeadXForce() << " " <<  m_pUBL->GetBeadYForce() << " "  << m_pUBL->GetBeadZForce() << " " <<  m_pUBL->GetBeadXVel() << " " <<  m_pUBL->GetBeadYVel() << " " <<  m_pUBL->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pDTR->CalculateBeadsPerCell();
	fcounter = m_pDTR->CalculateInteractionsPerBead();	
//	fn = m_pDTR->CalculateForceOnBeads();
//	vn = m_pDTR->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[4] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[4] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[4] += fcounter;
	m_MeanSqCornerIntPerBead[4] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DTR Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pDTR->GetBeadXForce() << " " <<  m_pDTR->GetBeadYForce() << " "  << m_pDTR->GetBeadZForce() << " " <<  m_pDTR->GetBeadXVel() << " " <<  m_pDTR->GetBeadYVel() << " " <<  m_pDTR->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pDTL->CalculateBeadsPerCell();
	fcounter = m_pDTL->CalculateInteractionsPerBead();	
//	fn = m_pDTL->CalculateForceOnBeads();
//	vn = m_pDTL->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[5] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[5] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[5] += fcounter;
	m_MeanSqCornerIntPerBead[5] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DTL Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pDTL->GetBeadXForce() << " " <<  m_pDTL->GetBeadYForce() << " "  << m_pDTL->GetBeadZForce() << " " <<  m_pDTL->GetBeadXVel() << " " <<  m_pDTL->GetBeadYVel() << " " <<  m_pDTL->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pDBR->CalculateBeadsPerCell();
	fcounter = m_pDBR->CalculateInteractionsPerBead();	
//	fn = m_pDBR->CalculateForceOnBeads();
//	vn = m_pDBR->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[6] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[6] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[6] += fcounter;
	m_MeanSqCornerIntPerBead[6] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DBR Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pDBR->GetBeadXForce() << " " <<  m_pDBR->GetBeadYForce() << " "  << m_pDBR->GetBeadZForce() << " " <<  m_pDBR->GetBeadXVel() << " " <<  m_pDBR->GetBeadYVel() << " " <<  m_pDBR->GetBeadZVel() << zEndl;
	
	beadsPerCell = m_pDBL->CalculateBeadsPerCell();
	fcounter = m_pDBL->CalculateInteractionsPerBead();	
//	fn = m_pDBL->CalculateForceOnBeads();
//	vn = m_pDBL->CalculateVelocityOfBeads();
	
	m_MeanCornerBeadsPerCell[7] += beadsPerCell;
	m_MeanSqCornerBeadsPerCell[7] += beadsPerCell*beadsPerCell;
	m_MeanCornerIntPerBead[7] += fcounter;
	m_MeanSqCornerIntPerBead[7] += fcounter*fcounter;
//	std::cout << "P " << GetRank() << " DBL Corner has " << beadsPerCell << " " << fcounter << " and F/V " <<  m_pDBL->GetBeadXForce() << " " <<  m_pDBL->GetBeadYForce() << " "  << m_pDBL->GetBeadZForce() << " " <<  m_pDBL->GetBeadXVel() << " " <<  m_pDBL->GetBeadYVel() << " " <<  m_pDBL->GetBeadZVel() << zEndl;
	
	

	}
	
	// Write out the number of cell-cell interactions
	
/*	if(IsProcessZero())
	{
	    std::cout << "***** P0 Number of intra-cell forces counted " << GlobalCellCounter << " *****" << zEndl;
	    std::cout << "***** P0 Number of cell-cell interactions counted " << GlobalCellCellIntCounter << " *****" << zEndl;
    }
	else
	{
	    std::cout << "***** P " << GetRank() << " Number of intra-cell forces counted " << GlobalCellCounter << " *****" << zEndl;
	    std::cout << "***** P " << GetRank() << " Number of cell-cell interactions counted " << GlobalCellCellIntCounter << " *****" << zEndl;
	}
*/
}

// ****************************************
// Function to update the momenta of all beads in the processor's Space..
// Because this does not require any inter-processor communication, we actually
// perform the update here for ALL beads in the Bulk and Border regions.

void mpsSimBox::UpdateMom()
{
	for(cCNTCellIterator iterCell=m_rvCNTCells.begin(); iterCell!=m_rvCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateMom();
	} 
}

// Function to collect all beads that have moved from the Border regions
// surrounding a given face and package then for sending to the adjacent
// processor identified by its direction vectors.
// The regions include: 1 Face + 4 Edges + 4 Corners.
// 

long mpsSimBox::UpdateFaceProcessor(long alpha, long beta, long gamma)
{
//    const long newPid = GetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma);

    return 0;
}

// Function to collect all beads that have moved from the owning processor
// to a neighbour that borders it at an edge. The regions include:
// 1 Edge + 2 Corners.

long mpsSimBox::UpdateEdgeProcessor(long alpha, long beta, long gamma)
{
//    const long newPid = GetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma);

    return 0;
}

// Function to collect all beads that have moved from the owning processor
// to a neighbour that touches it only at one corner. The region is simply
// the Corner region itself.

long mpsSimBox::UpdateCornerProcessor(long alpha, long beta, long gamma, mpsCorner* const pC1)
{
    const long newPid = GetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma);

    // See if any beads in the lone Corner region have left the owning processor's
    // Space. We count the number of lost beads.

    AbstractBeadVector  vLostBeads;
    vLostBeads.clear();

    std::cout << "Corner region " << alpha << " " << beta << " " << gamma << " has " << pC1->GetBeadTotal() << " beads" << zEndl;

	copy(pC1->GetLostBeadsForKey(newPid).begin(), pC1->GetLostBeadsForKey(newPid).end(), back_inserter(vLostBeads));

    long bt = pC1->GetLostBeadTotalForKey(newPid);
    std::cout << " Corner region " << alpha << " " << beta << " " << gamma << " has lost bead total " << bt << " " << vLostBeads.size() << zEndl;

    // Create a message instance to send the vLostBeads to the 
    // neighbouring processor identified by newPid.

    // pmSendBeadPacketToProc

    // Loop over all lost beads, stripping out their data into the message
    // and storing the empty bead instance locally for later use when 
    // beads move into this processor's Space from its neighbours.


    // and add it to the queue for this processor


    // not implemented yet


    // Return the number of lost beads so that the calling routine knows a message
    // has been created

    return bt;

}

// Helper function to return a container of the bead instances that are in
// any of the Faces and its adjacent Edges and Corners.

BeadList mpsSimBox::GetFaceEdgeCornerBeads(const mpsFace& rFace)
{
    BeadList lBeads;
    lBeads.clear();

    lBeads = rFace.GetFECBeads();

    return lBeads;
}

BeadList mpsSimBox::GetEdgeCornerBeads(const mpsEdge& rEdge)
{
    BeadList lBeads;
    lBeads.clear();

    lBeads = rEdge.GetECBeads();

    return lBeads;
}

BeadList mpsSimBox::GetCornerBeads(const mpsCorner& rCorner)
{
    BeadList lBeads;
    lBeads.clear();

    lBeads = rCorner.GetBeads();

    return lBeads;
}


// Function to return a container of the external CNT cells adjacent to any
// of the Faces and its associated Edges and Corners. These are retrieved from 
// the Face instance that owns them.

CNTCellVector& mpsSimBox::GetFaceEdgeCornerExternalCells(mpsFace& rFace)
{
    return rFace.GetExternalFECCells();
}


CNTCellVector& mpsSimBox::GetEdgeCornerExternalCells(mpsEdge& rEdge)
{
    return rEdge.GetExternalECCells();
}

CNTCellVector& mpsSimBox::GetCornerExternalCells(mpsCorner& rCorner)
{
    return rCorner.GetExternalCNTCells();
}


// Function to set a flag showing if the assembly and connection of the
// parallel SimBox failed for any reason. This means the simulation cannot proceed.

void mpsSimBox::SetAssemblyFailed()
{
    m_bFailed = true;
}

// Function to set a flag showing if this instance is an external processor
// that has to use the PBCs when sending messages.

void mpsSimBox::SetExternal()
{
    m_bExternal = true;
}

// Private helper function to check that all bead Ids are non-negative. This is used to confirm that bead transfers
// between processors do not inadvertently use uninitialized bead instances.o

bool mpsSimBox::CheckBeadIds() const
{
    bool bValid = true;
	
	for(cCNTCellIterator iterCell=m_rvCNTCells.begin(); iterCell!=m_rvCNTCells.end(); iterCell++)
	{
		BeadList lBeads = (*iterCell)->GetBeads();
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			if((*iterBead)->GetId() <= 0)
			{
			    bValid = false;
			    std::cout << "Proc " << GetRank() << " has invalid bead id in cell " << (*iterCell)->GetId() << " coords " << (*iterCell)->GetBLXIndex() << " " <<  (*iterCell)->GetBLYIndex() << " " << (*iterCell)->GetBLZIndex() << " : bead is " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << zEndl;
		        std::cout << zEndl;
			}
		}
	} 

    return bValid;
}

// Function used to debug a parallel simulation containing only single-bead polymers. It checks that each bead and its owning polymer have the same id.

bool mpsSimBox::CheckAllSingleBeadPolymerIds() const
{
    bool bValid = true;
	
#if EnableParallelSimBox == SimMPSEnabled

	for(cCNTCellIterator iterCell=m_rvCNTCells.begin(); iterCell!=m_rvCNTCells.end(); iterCell++)
	{
		BeadList lBeads = (*iterCell)->GetBeads();
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    const CPolymer* const pPolymer = (*iterBead)->GetPolymer();
			
			if((*iterBead)->GetId() != pPolymer->GetId())
			{
			    bValid = false;
			    std::cout << "Proc " << GetRank() << "  cell " << (*iterCell)->GetId() << " " << (*iterCell)->GetBLXIndex() << " " <<  (*iterCell)->GetBLYIndex() << " " << (*iterCell)->GetBLZIndex() << " has bead/polymer mismatched ids "  << pPolymer->GetId() << " " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << zEndl;
		        std::cout << zEndl;
			}
		}
	} 
#endif

    return bValid;
}

// Function to connect the containing processor to its 26 neighbours and itself via
// their ranks stored in this class. The array stores the rank of the neighbouring
// processor in a location that maps onto its position in the 3d array of processors
// in the World.

bool mpsSimBox::ConnectNeighbours()
{
    bool bValid = true;

		for( long gamma=-1; gamma<2; gamma++ )
		{
			for( long  beta=-1;  beta<2;  beta++ )
			{
				for( long alpha=-1; alpha<2; alpha++ )
				{
                    if(!SetNeighbourPidFromCoordinateDifferences(alpha, beta, gamma))
					{
						bValid = false;
					}
				}
			}
		}

    return bValid;
}

// Function to return the 1d index to a CNT cell defined by the directions
// alpha, beta, gamma from the cell with indices i, j, k. It is a copy
// of the same function in CSimBox.

long mpsSimBox::MapCellToIndex(long i, long j, long k, long alpha, long beta, long gamma) const
{
	long oldIndex, newIndex;

	oldIndex = i + j*m_LX + k*m_LX*m_LY;

// X test
	newIndex = oldIndex + alpha;
	if( newIndex >= m_LX*(1 + j + k*m_LY) )
		newIndex -= m_LX;
	else if( newIndex < m_LX*(j + k*m_LY) )
		newIndex += m_LX;

// Y test
	newIndex += beta*m_LX;
	if( newIndex >= (1 + k)*m_LX*m_LY )
		newIndex -= m_LX*m_LY;
	else if( newIndex < k*m_LX*m_LY )
		newIndex += m_LX*m_LY;

// Z test

	newIndex += gamma*m_LX*m_LY;
	if( newIndex >= m_LX*m_LY*m_LZ )
		newIndex -= m_LX*m_LY*m_LZ;
	else if( newIndex < 0 )
		newIndex += m_LX*m_LY*m_LZ;

	return newIndex;

}

// Function to return the 1d index to the NN CNT cell array belonging to a specified cell
// given the directions alpha, beta, gamma. We just wrap the private processor function.

long mpsSimBox::MapCellToNNIndex(long alpha, long beta, long gamma) const
{
    return GetPidIndexFromCoordinateDifferences(alpha, beta, gamma);
}

// Private helper function to add forces between beads in any of this processor's
// Face Border regions, plus associated Edges and Corners, and those owned by the  
// associated adjacent processor.

void mpsSimBox::AddExternalForcesToBeads(BeadList& rlBeads, zLongVector& rIds, zDoubleVector& rFX, zDoubleVector& rFY, zDoubleVector& rFZ)
{
    // We conditionally compile this functon as this class is only a friend of 
    // the abstract bead class in the parallel code.

#if EnableParallelSimBox == SimMPSEnabled
    if(rlBeads.size() == rIds.size())
    {
//        std::cout << " Processor " << GetRank() << " adding external forces to beads: " << rlBeads.size() << zEndl;
        long i = 0;
        for(BeadListIterator iterBead=rlBeads.begin(); iterBead!=rlBeads.end(); iterBead++)
        {
            const long  oldId = (*iterBead)->GetId();
            const long   id = rIds.at(i);
            const double fx = rFX.at(i);
            const double fy = rFY.at(i);
            const double fz = rFZ.at(i);

      //      std::cout << "Bead " << id << " is about to add force " << fx << " " << fy << " " << fz << " to accumulated forces " <<  (*iterBead)->m_Force[0] << " " <<  (*iterBead)->m_Force[1] << " " <<  (*iterBead)->m_Force[2] << zEndl;
            (*iterBead)->m_Force[0] += fx;
            (*iterBead)->m_Force[1] += fy;
            (*iterBead)->m_Force[2] += fz;

            if(oldId != id)
			{
			    // This should never be seen: I tested it on a 3**3 box and it wasnt'
			    std::cout << "***** ERROR ON Proc " << GetRank() << " bead " << oldId << " " << id << " adding force " << fx << " " << fy << " " << fz << zEndl;
			}
            i++;
        }
    }
    else
    {
        std::cout << " Processor " << GetRank() << " error adding external forces to beads: " << rlBeads.size() << " " << rIds.size() << zEndl;

    }
#endif
}

// Debug version of the private helper function to add forces between beads in any of this processor's
// Face Border regions, plus associated Edges and Corners, and those owned by the  
// associated adjacent processor.

void mpsSimBox::AddExternalForcesToBeads(BeadList& rlBeads, zLongVector& rIds, zLongVector& rFCs, zDoubleVector& rFX, zDoubleVector& rFY, zDoubleVector& rFZ)
{
    // We conditionally compile this functon as this class is only a friend of 
    // the abstract bead class in the parallel code.

#if EnableParallelSimBox == SimMPSEnabled
    if(rlBeads.size() == rIds.size())
    {
//        std::cout << " Processor " << GetRank() << " adding external forces to beads: " << rlBeads.size() << zEndl;
        long i = 0;
        for(BeadListIterator iterBead=rlBeads.begin(); iterBead!=rlBeads.end(); iterBead++)
        {
            const long  oldId = (*iterBead)->GetId();
            const long   id = rIds.at(i);
            const long   fc = rFCs.at(i);
            const double fx = rFX.at(i);
            const double fy = rFY.at(i);
            const double fz = rFZ.at(i);

      //      std::cout << "Bead " << id << " is about to add force " << fx << " " << fy << " " << fz << " to accumulated forces " <<  (*iterBead)->m_Force[0] << " " <<  (*iterBead)->m_Force[1] << " " <<  (*iterBead)->m_Force[2] << zEndl;
            (*iterBead)->m_Force[0] += fx;
            (*iterBead)->m_Force[1] += fy;
            (*iterBead)->m_Force[2] += fz;
			
            (*iterBead)->m_ForceCounter += fc;			

            if(oldId != id)
			{
			    // This should never be seen: I tested it on a 3**3 box and it wasnt'
			    std::cout << "***** ERROR ON Proc " << GetRank() <<  " " << rlBeads.size() << " terms:  adding forces to beads with distinct ids: "  << oldId << " " << id << " adding force " << fx << " " << fy << " " << fz << zEndl;
			}
            i++;
        }
    }
    else
    {
        std::cout << "***** ERROR ON Proc " << GetRank() << ": different bead totals " << rlBeads.size() << " " << rIds.size() << zEndl;

    }
#endif
}

// Function to calculate the bond forces between beads in a given Border region and its adjacent external cells, and to add it both to the 
// Border's beads and those in the neighbouring processor's adjacent Border. It is called by external CNT cells.
//
// The passed-in bead belongs to a neighbouring processor's cell, and we search the extended bond map to find all bonds that contain it at either end.
// For each such bond, we add the bond force to the owning processor's bead instance directly, but accumulate the force(s) on the adjacent processor's bead 
// via the passed-in force pointers. The force is then added to the external bead's force array in the external cell's calling routine.
//
// We need to ensure that bonded beads are always in adjacent cells as otherwise the coordinates of the neighbouring processor's bead will not have been 
// passed to the external CNT cell class for calculation of non-bonded forces. Hence, the bond strength/unstretched length must be so as to keep the
// bond length about equal to the CNT cell width.

void mpsSimBox::AddExternalBondForcesToBeads(long id, double x, double y, double z, double* pfx, double* pfy, double* pfz)
{
	LongExtendedBondMMIterator iterBond = m_mmExtendedBonds.find(id);

	if(iterBond != m_mmExtendedBonds.end())
	{
		do
		{
			mpuExtendedBond* pBond = iterBond->second;
			pBond->AddSharedForce(id, x, y, z, pfx, pfy, pfz);
/*			
			if(IGlobalSimBox::Instance()->GetCurrentTime() > 280)
			{
			    mpuGhostBead* pBondHead = pBond->GetHeadGhost();
			    mpuGhostBead* pBondTail = pBond->GetTailGhost();
				
			    if(id == pBondHead->GetId())
				{
				    std::cout << "Extended bond " << pBond->GetId() << " has head ghost " << pBondHead->GetId() << " equal to passed in id " << id << zEndl;
			        std::cout << "Proc " << GetRank() << " calculating extended bond force for bond tail/polymer " << pBondTail->GetId() << "  " << pBondTail->GetPolymerId()  << zEndl;					
				}
				else if(id == pBondTail->GetId())
				{
				    std::cout << "Extended bond " << pBond->GetId() << " has tail ghost " << pBondTail->GetId() << " equal to passed in id " << id << zEndl;
			        std::cout << "Proc " << GetRank() << " calculating extended bond force for bond head/polymer " << pBondHead->GetId() << "  " << pBondHead->GetPolymerId()  << zEndl;
				
				}
			}  */
			
			m_ExtendedBondForceCounter++; 
			iterBond++;
		}
		while(iterBond !=m_mmExtendedBonds.upper_bound(id));
	}
}

// Function to count the number of beads in all CNT cells belonging to this processor.
// We just iterate over all the CNT cells and accumulate the beads in them.

long mpsSimBox::GetBeadTotalInCNTCells() const
{
	long total = 0;

	for(cCNTCellIterator citerCell=m_rvCNTCells.begin(); citerCell!=m_rvCNTCells.end(); citerCell++)
	{
        total += (*citerCell)->CellBeadTotal();
    }

	return total;
}

// Function to calculate the number of beads in this processor's Space and store the
// resulting value.

long mpsSimBox::CalculateBeadTotal()
{
	m_BeadTotal = GetBeadTotalInCNTCells();
	
	return m_BeadTotal;
}

// Function to calculate the total angular momentum of all beads in this processor's Space taken around the local origin.
// Because the total CM velocity is zero, we do not need to modify this when translating it into P0's Space.

long mpsSimBox::CalculateAngularMomentum()
{
    m_AngMom[0] = 0.0;
    m_AngMom[1] = 0.0;
    m_AngMom[2] = 0.0;
	
	long nbeads = 0;
	double x, y, z;
	double vx, vy, vz;
	
	for(cCNTCellIterator iterCell=m_rvCNTCells.begin(); iterCell!=m_rvCNTCells.end(); iterCell++)
	{
		BeadList lBeads = (*iterCell)->GetBeads();
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ++nbeads;
			x = (*iterBead)->GetXPos();
			y = (*iterBead)->GetYPos();
			z = (*iterBead)->GetZPos();
			
			vx = (*iterBead)->GetXMom();
			vy = (*iterBead)->GetYMom();
			vz = (*iterBead)->GetZMom();
			
			m_AngMom[0] += (y*vz - z*vy);
			m_AngMom[1] += (z*vx - x*vz);
			m_AngMom[2] += (x*vy - y*vx);
		}
	} 
	
	return nbeads;
}

// Function to calculate the total combined velocity of all beads in this processor's Space.  To convert this into the
// CM velocity for all beads in this processor's space it must be divided by the number of beads.

long  mpsSimBox::CalculateCMVel()
{
    m_CMVel[0] = 0.0;
    m_CMVel[1] = 0.0;
    m_CMVel[2] = 0.0;
	
	long nbeads = 0;
	
	for(cCNTCellIterator iterCell=m_rvCNTCells.begin(); iterCell!=m_rvCNTCells.end(); iterCell++)
	{
		BeadList lBeads = (*iterCell)->GetBeads();
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ++nbeads;
			m_CMVel[0] += (*iterBead)->GetXMom();
			m_CMVel[1] += (*iterBead)->GetYMom();
			m_CMVel[2] += (*iterBead)->GetZMom();
		}
	} 

    if(nbeads > 0)
	{
        m_CMVel[0] /= static_cast<double>(nbeads);
        m_CMVel[1] /= static_cast<double>(nbeads);
        m_CMVel[2] /= static_cast<double>(nbeads);
	}
	else
	{
	    std::cout << "Error - no beads in processor " << GetRank() << " calculation of CM velocity" << zEndl;
	}
	
	return nbeads;
}

// Protected function used by derived classes to calculate the total angular momentum 
// of all beads in the processor World and to log the resulting values. P0 sets its
// own data and then receives messages from all other PN before writing the total
// value to the log file.

void mpsSimBox::LogWorldAngularMomentum()
{
	// Calculate the total angular momentum for all PN including P0. The results are stored in the base class.
			
	const long beadTotal = CalculateAngularMomentum();
		
	// Now create message instance to pass PN data to P0 for accumulation
	
	pmProcessorBeadAngularMomentum  pmBeadAM;

	pmBeadAM.ZeroAngMom();
		
	if(IsProcessZero())
	{		
		// Store P0 data first and then receive results from other PN
		
		pmBeadAM.SetMessageData(beadTotal, m_AngMom[0], m_AngMom[1], m_AngMom[2]);

        if(pmBeadAM.Validate())
		{
             pmBeadAM.Receive();
			 
			 // Now write out the data for the World
			 
		    std::cout << "World ang mom is " << pmBeadAM.GetWorldBeadTotal() << " " << pmBeadAM.GetWorldXAngMom() << " " << pmBeadAM.GetWorldYAngMom() << " " << pmBeadAM.GetWorldZAngMom() << zEndl;

			// and write the results to the log file

//			CLogpmAngularMomentum* pMsg = new CLogpmAngularMomentum(IGlobalSimBox::Instance()->GetCurrentTime(), pmBeadAM.GetWorldBeadTotal(), pmBeadAM.GetWorldXAngMom(), pmBeadAM.GetWorldYAngMom(), pmBeadAM.GetWorldZAngMom());		
		}
		else
		{
		    std::cout << "**** Error on P0 - Ang Mom validation failed **** " << beadTotal << zEndl;
		}
	}
	else
	{
				
		pmBeadAM.SetMessageData(beadTotal, m_AngMom[0], m_AngMom[1], m_AngMom[2]);

        if(pmBeadAM.Validate())
		{
             pmBeadAM.SendP0();
	    }
		else
		{
		    std::cout << "**** Error on PN " << GetRank() << " - Ang Mom validation failed **** " << beadTotal << zEndl;
		}

	}
}

// Protected function used by derived classes to calculate the total CM velocity
// of all beads in the processor World and to log the resulting values. P0 sets its
// own data and then receives messages from all other PN before writing the result
// to the log file.

void mpsSimBox::LogWorldCMVel()
{
	// Calculate the total CM velocity and angular momentum for all PN including P0. The results are stored in the base class.
			
	const long beadTotal = CalculateCMVel();
			
	// Now create message instance to pass PN data to P0 for accumulation
	
	pmProcessorBeadCMVelocity pmBeadCMV;

	pmBeadCMV.ZeroCMVel();
		
	if(IsProcessZero())
	{				
		// Store P0 data first and then receive results from other PN
		
		pmBeadCMV.SetMessageData(beadTotal, m_CMVel[0], m_CMVel[1], m_CMVel[2]);

        if(pmBeadCMV.Validate())
		{
             pmBeadCMV.Receive();
			 
			 // Now write out the data for the World
			 
		    std::cout << "World CM vel is " << pmBeadCMV.GetWorldBeadTotal() << " " << pmBeadCMV.GetWorldXCMVel() << " " << pmBeadCMV.GetWorldYCMVel() << " " << pmBeadCMV.GetWorldZCMVel() << " " << zEndl;

			// and write the results to the log file. But the next line does not work on G5Q, the IGlobalSimBox::Instance() function 
			// appears to crash with a bus error. Perhaps this is because the log file is not properly created?
	
//			CLogpmCMVelocity* pMsg = new CLogpmCMVelocity(IGlobalSimBox::Instance()->GetCurrentTime(), pmBeadCMV.GetWorldBeadTotal(),  pmBeadCMV.GetWorldXCMVel(), pmBeadCMV.GetWorldYCMVel(), pmBeadCMV.GetWorldZCMVel());		

		}
		else
		{
		    std::cout << "**** Error on P0 - CM vel validation failed **** " << beadTotal << zEndl;
		}
	}
	else
	{
		// Send owning processor's bead total and CM vel and ang mom to P0

		pmBeadCMV.SetMessageData(beadTotal, m_CMVel[0], m_CMVel[1], m_CMVel[2]);

        if(pmBeadCMV.Validate())
		{
             pmBeadCMV.SendP0();
	    }
		else
		{
		    std::cout << "**** Error on PN " << GetRank() << " - CM vel validation failed **** " << beadTotal << zEndl;
		}

	}
}

// Protected function used by derived classes to calculate the total number of beads
// n the processor World and to log the resulting values. P0 sets its
// own data and then receives messages from all other PN before writing the result
// to the log file.

void mpsSimBox::LogWorldBeadTotal()
{
	// Let each processor calculate its own total
	
	m_BeadTotal = CalculateBeadTotal();

	pmProcessorBeadTotal pmBeadTotal;
	pmBeadTotal.SetMessageData(m_BeadTotal);

	// Check the total is valid on each processor - if not, replace it by a zero.

	if(!pmBeadTotal.Validate())
	{
	    pmBeadTotal.SetMessageData(0);
		std::cout << "Error: processor " << GetRank() << " has invalid bead total " << m_BeadTotal << zEndl;
	}

	if(IsProcessZero())
	{
		// Count up all the bead totals and write out the result: note that the Receive
		// function takes care of zeroing the container of bead totals.

		pmBeadTotal.Receive();

		const long worldBeadTotal = pmBeadTotal.GetWorldBeadTotal();

		// and write the results to the log file

//		CLogpmBeadTotal* pMsg = new CLogpmBeadTotal(IGlobalSimBox::Instance()->GetCurrentTime(), pmBeadTotal.GetWorldBeadTotal());

		std::cout << "P0 counts " << worldBeadTotal << " for all processors" << zEndl;
	}
	else
	{
		// Send owning processor's bead total to P0

		pmBeadTotal.SendP0();
	}
}

// Function to take a list of beads that are about to leave the owning processor's Space in one of the pmSendXXXBeads messages and modify their
// owning polymers depending on whether they are a single-bead polymer or multi-bead polymer. Note that the bead instances must still exist
// and contain all their data, otherwise this function will fail. Also note that we cannot interrogate a bead's owning polymer to determine its
// size when the polymer is part of an extended polymer as its size will vary depending on how many of its beads are in each processor's Space.
// Instead, we check both its size and whether it is wrapped in an extended polymer. In the latter case we don't check the size at all.
//
// Single-bead polymers are removed from the bulk container, emptied,  and stored for later re-use when another single-bead polymer 
// enters the owning processor's Space. 
//
// Multi-bead polymers are moved from the bulk polymer container to the extended polymer container in the TransferBulkPolymerToExtended() function.
// Note that the pid of the new owning processor is needed to identify where the beads have gone in the extended polymer instance.

void mpsSimBox::MoveOwningPolymersToExtended(long newPid, BeadList& lBeads)
{
#if EnableParallelSimBox == SimMPSEnabled

//	std::cout << "Proc " << GetRank() << " MoveOwningPolymersToExtended: Starting with " << m_mBulkPolymers.size() << " bulk polymers and " << m_mExtendedPolymers.size() << " extended polymers" << zEndl;
		
    for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
	    CPolymer* pPolymer = (*iterBead)->GetPolymer();
		
		if(pPolymer)
		{ 
		    if(pPolymer->GetSize() > 1 || pPolymer->GetExtendedPolymer())
		    {
			    TransferBulkPolymerToExtended(newPid, *iterBead);
		    }
			else
			{
			    // Single-bead polymers are just stored for re-use when a bead moves out of the owning processor's Space, and 
				// their owning polymer is not stored in the bulk polymers map, so we don't remove it.
										
//				m_mBulkPolymers.erase(pPolymer->GetId());
				pPolymer->Clear();
				delete pPolymer;
//				m_lEmptyPolymers.push_back(pPolymer);				
			}
		}	
		else
		{
		    std::cout << "ERROR on Proc " << GetRank() << " sending to proc " << newPid << " bead " << (*iterBead)->GetId() << " has no owning polymer" << zEndl;
		}
	}
	
//	std::cout << "Proc " << GetRank() << " MoveOwningPolymersToExtended has " << m_mBulkPolymers.size() << "  " <<  m_mExtendedPolymers.size() << " bulk and extended polymers, and " << m_mmExtendedBonds.size() << " extended bonds" << zEndl;

#endif
}


// Function called when a bead in a multi-bead polymer leaves the owning processor's Space.  It transfers the bead's owning polymer 
// from the processor's bulk container to its extended polymer container (after wrapping it in an extended polymer instance)  or, 
// if it is already there, replaces one of its beads with an empty ghost bead. The function returns true if the polymer was successfully 
// inserted into the extended polymer's map, and false otherwise. The latter result either means that the polymer has now left the 
// owning processor's Space or that an error occurred.  If the bead is the last bead in a polymer to 
// leave the Space, we delete the polymer instance and the extended polymer wrapper instance. It would be more efficient to recycle them,
// but that is more complicated and may be implemented later.
//
// Note that the calling routine is responsible for ensuring that the bead's owning polymer is not a single-bead polymer.
// On the assumption that polymers that are already near the boundaries of the processor's Space are more likely than bulk polymers
// to have their beads move back and forthr between processors, we search the extended polymers map first.
//
// The function returns true if all goes well, otherwise it returns false.

bool mpsSimBox::TransferBulkPolymerToExtended(long newPid, CAbstractBead* const pBead)
{
    bool bInserted = false;
		
#if EnableParallelSimBox == SimMPSEnabled

    CPolymer* pPolymer = pBead->GetPolymer();
	
	if(m_mExtendedPolymers.find(pPolymer->GetId()) != m_mExtendedPolymers.end())
	{
	    // Polymer is already in the extended polymer map, so we update its information for the passed-in bead and remove any bonds
		
		mpuExtendedPolymer* pExtPolymer = m_mExtendedPolymers.find(pPolymer->GetId())->second;
	
		if(pExtPolymer->RemoveBeadFromGhost(newPid, pBead))
		{			
		    bInserted = true;
			
			// Update the extended bond map
			UpdateExtendedBondMapOnBeadExit(pBead->GetId(), pExtPolymer);
			
		    // If this was the last bead in the polymer to leave the owning processor's Space we recycle the polymer instance,
			// and delete the extended polymer and its contained ghost beads. Note that the concrete bead instances are recycled in 
			// the pmSendXXXBeads messages. We set the return value to false to indicate that the polymer is no longer in the owning
			// processor's Space. We also remove all entries from the exended bond map that are indexed by the bead's id. We have to do this
			// here as the bead's owning polymer will not exist after returning from this function.

			if(pExtPolymer->IsEmpty())
			{
     			m_mExtendedPolymers.erase(pExtPolymer->GetId());
				pPolymer->Clear();
                delete pPolymer;
				pPolymer = 0;
//			    m_lEmptyPolymers.push_back(pPolymer);
				delete pExtPolymer;
				pExtPolymer = 0;
			}
			
		}
		else
		{
		    bInserted = false;
		    std::cout << "Error in mpsSimBox: unable to replace ghost bead for bead/polymer in existing extended polymer " << pBead->GetId() << " " << pPolymer->GetId() << " moving to processor " << newPid << zEndl;
		}
	}
    else if(m_mBulkPolymers.find(pPolymer->GetId()) != m_mBulkPolymers.end())
	{
	    // Polymer is in the bulk map, so we move it to the extended map by wrapping it in a new mpuExtendedPolymer instance,
		// and replace its missing bead with an empty ghost bead.
							
		mpuExtendedPolymer* pExtPolymer = new mpuExtendedPolymer(newPid, pBead, pPolymer);
								
		pair<LongExtendedPolymerIterator, bool> iterInsert;		
		iterInsert = m_mExtendedPolymers.insert(make_pair(pPolymer->GetId(), pExtPolymer));
				
		if(iterInsert.second)
		{
		    bInserted = true;
			m_mBulkPolymers.erase(pPolymer->GetId());
			
			// Update the extended bond map
			UpdateExtendedBondMapOnBeadExit(pBead->GetId(), pExtPolymer);
		}
		else
		{
		    bInserted = false;
		    std::cout << "Error in mpsSimBox: unable to create new extended polymer for bead/polymer " << pBead->GetId() << " " << pPolymer->GetId() << " for processor " << newPid << zEndl;
		    // Clean up if the newly-created extended polymer could not be inserted
		    delete pExtPolymer;
		}
	}
	else
	{
	    std::cout << "Error in mpsSimBox: bead/polymer is not in bulk or extended polymer maps " << pBead->GetId() << " " << pPolymer->GetId() << zEndl;
	}
	
#endif

	return bInserted;
}

		
// Function to update the extended bond map when the specified bead enters the owning processor's Space.
//
// When a bead enters the owning processor's Space, we iterate over all extended bonds in the polymer and remove those
// that contain the specified bead at either end and whose other end is already full. This indicates that both beads of the bond
// are now in the owning processor's Space. We only work with ghost beads here as the concrete 
// bead instances may have already been recycled. 

void mpsSimBox::UpdateExtendedBondMapOnBeadEntry(long beadId, mpuExtendedPolymer* const pExtPolymer)
{
    ExtendedBondList lExtBonds = pExtPolymer->GetExtendedBonds();
	
//		std::cout << "Proc " << GetRank() << " has " << m_mmExtendedBonds.size() << " extended bonds to calculate" << zEndl;

	for(ExtendedBondListIterator iterBond=lExtBonds.begin(); iterBond!=lExtBonds.end(); iterBond++)
	{
		long headId = (*iterBond)->GetHeadGhost()->GetId();
		long tailId = (*iterBond)->GetTailGhost()->GetId();
			
		// Bead is tail of bond
		if(beadId == headId)
		{
			if((*iterBond)->GetTailGhost()->IsEmpty())
			{
//			    std::cout << "Adding bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " to map as bead " << beadId << " enters space" << zEndl;
				m_mmExtendedBonds.insert(zPairLongExtendedBond(headId, *iterBond));
				m_mmExtendedBonds.insert(zPairLongExtendedBond(tailId, *iterBond));			
			}
			else
			{
//		        std::cout << "Removing bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " from map as bead " << beadId << " enters space" << zEndl;
				RemoveExtendedBondFromMap(headId, (*iterBond)->GetId());
				RemoveExtendedBondFromMap(tailId, (*iterBond)->GetId());
			}
		}
			
		// Bead is tail of bond
		if(beadId == tailId)
		{
			if((*iterBond)->GetHeadGhost()->IsEmpty())
			{
//		        std::cout << "Adding bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " to map as bead " << beadId << " enters space" << zEndl;
				m_mmExtendedBonds.insert(zPairLongExtendedBond(headId, *iterBond));
				m_mmExtendedBonds.insert(zPairLongExtendedBond(tailId, *iterBond));			
			}
			else
			{
//		        std::cout << "Removing bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " from map as bead " << beadId << " enters space" << zEndl;
				RemoveExtendedBondFromMap(headId, (*iterBond)->GetId());
				RemoveExtendedBondFromMap(tailId, (*iterBond)->GetId());
			}
		}
	}
}


// Function to update the extended bond map when the specified bead leaves the owning processor's Space.
//
// When a bead leaves the owning processor's Space, we iterate over all extended bonds in the polymer and remove those
// that contain the specified bead at either end and whose other end is already empty, and add those whose other end still contains a concrete bead
// instance. We only work with ghost beads here as the concrete bead instances may have already been recycled. The calling routine must ensure 
// that the extended polymer instance is still valid in this case.
//
// An important implementation detail here is that the multimap function erase() cannot be used to remove a single map entry as it removes ALL entries 
// for the given key. Instead, we have to obtain an iterator to the single entry that we want to remove, and then erase it. This is crucial when a bead
// is part of multiple exnteded bonds. There are two entries in the map for each bond, and we have to ensure that we erase only the appropriate ones.

void mpsSimBox::UpdateExtendedBondMapOnBeadExit(long beadId, mpuExtendedPolymer* const pExtPolymer)
{
    ExtendedBondList lExtBonds = pExtPolymer->GetExtendedBonds();
	
//	std::cout << "Proc " << GetRank() << " has " << m_mmExtendedBonds.size() << " extended bonds to calculate" << zEndl;
	for(ExtendedBondListIterator iterBond=lExtBonds.begin(); iterBond!=lExtBonds.end(); iterBond++)
	{
		long headId = (*iterBond)->GetHeadGhost()->GetId();
		long tailId = (*iterBond)->GetTailGhost()->GetId();
			
		// Bead is head of bond
		if(beadId == headId)
		{
			if((*iterBond)->GetTailGhost()->IsEmpty())
			{
				// We have the key to the map entry, but we must only remove the specified extended bond not all bonds with the same key.
				// We iterate over all entries with the same key until we find the two that match the given bond instance.
					
					
//			    std::cout << "Removing bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " from map as bead " << beadId << " leaves space" << zEndl;
				RemoveExtendedBondFromMap(headId, (*iterBond)->GetId());
				RemoveExtendedBondFromMap(tailId, (*iterBond)->GetId());
			}
			else
			{
//			    std::cout << "Adding bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " to map as bead " << beadId << " leaves space" << zEndl;
				m_mmExtendedBonds.insert(zPairLongExtendedBond(headId, *iterBond));
				m_mmExtendedBonds.insert(zPairLongExtendedBond(tailId, *iterBond));			
			}
		}
			
		// Bead is tail of bond
		if(beadId == tailId)
		{
			if((*iterBond)->GetHeadGhost()->IsEmpty())
			{
//	            std::cout << "Removing bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " from map as bead " << beadId << " leaves space" << zEndl;
				RemoveExtendedBondFromMap(headId, (*iterBond)->GetId());
				RemoveExtendedBondFromMap(tailId, (*iterBond)->GetId());
			}
			else
			{
//		        std::cout << "Adding bond " << (*iterBond)->GetId() << " with head/tail beads " << headId << " " << tailId << " to map as bead " << beadId << " leaves space" << zEndl;
				m_mmExtendedBonds.insert(zPairLongExtendedBond(headId, *iterBond));
				m_mmExtendedBonds.insert(zPairLongExtendedBond(tailId, *iterBond));			
			}
		}
	}
}

// Private helper function to remove an entry from the extended bond map given its key and the extended bond id. We cannot use the multimap's
// erase(key) funtion as it removes ALL entries with the same key. Instead, we iterate over all entries with the given key and only remove
// the bond whose id matches that specified in the argument list.

void mpsSimBox::RemoveExtendedBondFromMap(long key, long bondId)
{
	LongExtendedBondMMIterator iterBond        = m_mmExtendedBonds.find(key);
	LongExtendedBondMMIterator iterDeletedBond = m_mmExtendedBonds.end();

	if(iterBond != m_mmExtendedBonds.end())
	{
		do
		{
			mpuExtendedBond* pBond = iterBond->second;
//			std::cout << "Proc " << GetRank() << " calculating bond force for " << id << " " << pBond->GetId() << " " << pBond->GetHeadGhost()->GetId() << " " << pBond->GetTailGhost()->GetId() << " " << zEndl;
			if(pBond->GetId() == bondId)
			{
			    iterDeletedBond = iterBond;
			}
			iterBond++;
		}
		while(iterBond !=m_mmExtendedBonds.upper_bound(key));
		
		if(iterDeletedBond !=  m_mmExtendedBonds.end())
		{
		    m_mmExtendedBonds.erase(iterDeletedBond);
		}
	}
}

// Function to take a list of beads that are entering the owning processor's Space in one of the pmSendXXXBeads messages and assign them 
// to their owning polymer depending on whether they are a single-bead polymer or multi-bead polymer. 
//
// Each bead instance that belongs to a single-bead polymer is inserted into a newly-retrieved, empty polymer and its data set accordinly.
// Then the polymer is inserted into the bulk polyers container. Note that single-bead polymers should never appear in the extended polymers
// container as they cannot span the processor boundaries.
//
// Beads that belong to multi-bead polymers are dealt with in the TransferExtendedPolymerToBulk() function. This retrieves an empty
// polymer instance when the first bead of a polymer enters the processor's Space, adds subsequent beads to the polymer and, when the last
// bead of a polymer arrives, moves the polymer from the extended polymer container into the bulk polymer container. Once the beads have been
// inserted into their owning extended polymers, we add their extended bonds to the extended bond map. 
//
// Note that the bead instances are not yet complete here - they have no owning polymer pointer, for example; their remaining data are 
// passed in the argument list.

void mpsSimBox::MoveOwningPolymersToBulk(long oldPid, BeadList& lBeads, const zLongVector& vBeadLocations, const zLongVector& vFirstBondIds, const zLongVector& vFirstBPIds, const zLongVector& vPolymerIds, const zLongVector& vPolymerTypes)
{
#if EnableParallelSimBox == SimMPSEnabled

    // First ensure that there are enough empty polymer instances to wrap incoming beads
	
	while(static_cast<long>(lBeads.size()) > GetEmptyPolymerTotal() && CreateEmptyPolymers())
	{
		std::cout << "Processor " << GetRank()  << " has to create new empty polymers for " << lBeads.size() << " received instances" << zEndl;
	}
	
    long ip = 0;
			
    for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
	    const long location         = vBeadLocations.at(ip);
	    const long firstBondId      = vFirstBondIds.at(ip);
	    const long firstBondPairId  = vFirstBPIds.at(ip);
	    const long polyId           = vPolymerIds.at(ip);
	    const long polyType         = vPolymerTypes.at(ip);
		const long polySize         = IGlobalSimBox::Instance()->GetPolymerSizeFromType(polyType);
		
        if(polySize > 1)
		{		
			TransferExtendedPolymerToBulk(oldPid, *iterBead, location, firstBondId, firstBondPairId, polyId, polyType,  polySize);
		}
		else
		{
			// Owning polymer is a single-bead polymer: these are not stored in the bulk polymers map,  so we just create a new polymer instance
			// and insert the newly-arrived bead into it, set the bead's owning polymer pointer  Note that this version of the 
			// overloaded AddBead() function automatically
			// clears out the polymer's internal containers before adding the new bead, so it can only be called on single-bead polymers
			// and then only once.
			
			// For debugging purposes, I don't reuse polymers here but create a new one.
				 
//			CPolymer* pPolymer = m_lEmptyPolymers.back();
//			m_lEmptyPolymers.pop_back();
            CPolymer* pPolymer = new CPolymer();
		    pPolymer->Clear();
		    pPolymer->SetId(polyId);
			pPolymer->SetType(polyType);
			pPolymer->AddSingleBead(*iterBead);  // AddSingleBead() sets the passed-in bead's owning polymer pointer as well
		}
		
		ip++;
	}
	
//	std::cout << "Proc " << GetRank() << " MoveOwningPolymersToBulk has " << m_mBulkPolymers.size() << "  " <<  m_mExtendedPolymers.size() << " bulk and extended polymers, and " << m_mmExtendedBonds.size() << " extended bonds" << zEndl;

#endif
}


// Function to create or update polymers in the extended polymers map when beads arrive in the owning processor's Space. 
// However, this is a bit of a misnomer as the function is also called to replace ghost beads within an extended polymer by 
// concrete bead instances if the ghost has moved back into the owning polymer's processor.  It is the complement to the above 
// TransferBulkPolymerToExtended function. Note that the bead instances passed in are not yet complete - they have no owning polymer
// pointer for example.
//
// When the first bead of a polymer moves into the owning processor's Space, we create a new extended polymer and fill it with 
// empty ghost beads that represent the remaining beads of the polymer.  The newly-arrived bead is wrapped in the ghost that represents
// its location in the polymer's shape.  Subsequent beads are then inserted into the appropriate ghosts as they arrive. Note that we 
// only know the pid of the single bead that has arrived not the remaining ghost beads in the polymer.
// 
// If all of the polymer's beads are now concrete, the extended polymer (and its extended bonds and ghost beads) are replaced
// and the polymer is moved back to the bulk map. 
//
// This function returns true if all goes well, otherwise it returns false.

bool mpsSimBox::TransferExtendedPolymerToBulk(long oldPid, CAbstractBead* const pBead, long location, long firstBondId, long firstBondPairId, long polyId, long polyType, long polySize)
{	
    bool bInserted = false;
	
#if EnableParallelSimBox == SimMPSEnabled	

// Check the polymers on P4
	
	if(m_mExtendedPolymers.find(polyId) != m_mExtendedPolymers.end())
	{
	    // Polymer is already in the extended polymer map, so we add the passed-in bead to it
				
		mpuExtendedPolymer* pExtPolymer = (m_mExtendedPolymers.find(polyId))->second;
									 
		if(pExtPolymer->AddBeadToGhost(oldPid, pBead))
		{		
			// Update the extended bond map 
			UpdateExtendedBondMapOnBeadEntry(pBead->GetId(), pExtPolymer);

			// Check if the extended polymer is now wholly in the owning processor's Space: if so, move its wrapped polymer instance into
			// the bulk map and delete the extended polymer. Note that the polymer's data must have been set in its extended polymer instance.
			
			if(pExtPolymer->IsComplete())
			{				 
		    	 CPolymer* pPolymer = pExtPolymer->GetPolymer();
				 pPolymer->SetExtendedPolymer(0);    // Clear the owning extended polymer pointer
		
		         // Check the polymer before putting it back into the bulk map
				 
				if(!pPolymer->Validate())
				{
					 std::cout << "ERROR on proc " << GetRank() << " Extended polymer " << pExtPolymer->GetId() << " (" << pExtPolymer->GetPolymer()->GetId() << ")  is invalid" << zEndl;
				     pExtPolymer->Dump();
				}
					 					 
		         pair<LongPolymerIterator, bool> iterInsert;
		         iterInsert = m_mBulkPolymers.insert(make_pair(pPolymer->GetId(), pPolymer));
					
		         if(iterInsert.second)
				 {
		             bInserted = true;
				     // Polymer is back in the bulk map so we remove the extended polymer from the extended polymer map and delete it
					 m_mExtendedPolymers.erase(polyId);
					 delete pExtPolymer;
					 pExtPolymer = 0;
				}
				 else
		         {
			         // An error occurred so we leave the extended polymer in the extended polymers map
		             bInserted = false;
		             std::cout << "ERROR in mpsSimBox: unable to move multi-bead polymer " <<  pExtPolymer->GetId() << " " << pPolymer->GetId() << " into bulk polymer map on receiving last bead " << pBead->GetId() << " from processor " << oldPid << zEndl;
		         }
			}  
			else if(pExtPolymer->IsEmpty())
			{
			    std::cout << " ERROR on Proc " << GetRank() << " extended polymer " << pExtPolymer->GetId() << " is unexpectedly empty" << zEndl;
			}
		}
		else
		{
		    std::cout << "Error in mpsSimBox: unable to replace ghost bead for bead/polymer in existing extended polymer " << pBead->GetId() << " " << polyId << " arriving from processor " << oldPid << zEndl;
		}
	}
    else
	{
	    // This is the first bead of a polymer, so we create a new extended polymer and add it to the extended polymers map.
		// We pass an exemplar polymer of the required type into the constructor so that the extended polymer class can connect its
		// beads in the correct order using extended bonds and bondpairs, preparatory to receiving beads to fill it up.
		// The bead is inserted into its owning polymer in the extended polymer's constructor. 
					
		const PolymerVector& rPolymerTypes = IGlobalSimBox::Instance()->GetPolymerTypes();
		CPolymer* const pExemplar = rPolymerTypes.at(polyType);
		
//		CPolymer* pPolymer = m_lEmptyPolymers.back();
//		m_lEmptyPolymers.pop_back();

        CPolymer* pPolymer = new CPolymer();		
		pPolymer->Clear();
		pPolymer->SetId(polyId);
		pPolymer->SetType(polyType);
		pBead->SetOwningPolymer(pPolymer);
				
		mpuExtendedPolymer* pExtPolymer = new mpuExtendedPolymer(oldPid, pBead, pPolymer, pExemplar, polySize, location, firstBondId, firstBondPairId);
		
		pair<LongExtendedPolymerIterator, bool> iterInsert;
		iterInsert = m_mExtendedPolymers.insert(make_pair(polyId, pExtPolymer));

		if(iterInsert.second)
		{
		    bInserted = true;
			
			// Update the extended bond map 
			UpdateExtendedBondMapOnBeadEntry(pBead->GetId(), pExtPolymer);
		}
		else
		{
		    std::cout << "Error in mpsSimBox: unable to create new extended polymer for bead/polymer " << pBead->GetId() << " " << polyId << " arriving from processor " << oldPid << zEndl;
		    // Clean up if the newly-created extended polymer could not be inserted
			pPolymer->Clear();
//			m_lEmptyPolymers.push_back(pPolymer);
            delete pPolymer;
			pPolymer = 0;
		    delete pExtPolymer;
			pExtPolymer = 0;
		}
	}

#endif
		
	return bInserted;
}

// Protected helper function to write out information on the extended polymers owned by this processor.
// We also output a warning message if the key and the stored polymer's id are not the same.

void mpsSimBox::ListExtendedPolymers() const
{
#if EnableParallelSimBox == SimMPSEnabled

    std::cout << "********************************************************************************" << zEndl;
	std::cout << "Proc " << GetRank() << " at time " << IGlobalSimBox::Instance()->GetCurrentTime() << " has " << m_mExtendedPolymers.size() << " extended polymers:" << zEndl;
		
	for(cLongExtendedPolymerIterator iterExtPoly=m_mExtendedPolymers.begin(); iterExtPoly!=m_mExtendedPolymers.end(); iterExtPoly++)
	{
	    const mpuExtendedPolymer* const pPoly = iterExtPoly->second;
		
		if(iterExtPoly->first == pPoly->GetId())
		{
            pPoly->Dump();
        }
		else
		{
		    std::cout << "ERROR: Extended polymer key/id not equal: " << iterExtPoly->first << " " << pPoly->GetId() << "  and wrapped polymer " << pPoly->GetPolymer()->GetId() << " " << pPoly->GetPolymer()->GetSize() << " with concrete/empty ghost bead totals " << pPoly->GetConcreteBeadTotal() << " " << pPoly->GetEmptyGhostBeadTotal() << zEndl;		
		}
	}
    std::cout << "********************************************************************************" << zEndl;
	
#endif
}

// Protected helper function to write out information on a single extended polymer owned by this processor. If the polymer is not found, we do nothing.

void mpsSimBox::ListExtendedPolymer(long polyId) const
{
#if EnableParallelSimBox == SimMPSEnabled

	if(m_mExtendedPolymers.find(polyId) != m_mExtendedPolymers.end())
	{				
	    long                        key = (m_mExtendedPolymers.find(polyId))->first;
		mpuExtendedPolymer* pExtPolymer = (m_mExtendedPolymers.find(polyId))->second;
									 
        std::cout << "********************************************************************************" << zEndl;
	    std::cout << "Proc " << GetRank() << " at time " << IGlobalSimBox::Instance()->GetCurrentTime() << " dumping extended polymer  " << polyId << zEndl;
		
		if(key == pExtPolymer->GetId())
		{
			pExtPolymer->Dump();
		}
		else
		{
			std::cout << "ERROR: Extended polymer key/id not equal: " << key << " " << pExtPolymer->GetId() << "  and wrapped polymer " << pExtPolymer->GetPolymer()->GetId() << " " << pExtPolymer->GetPolymer()->GetSize() << " with concrete/empty ghost bead totals " << pExtPolymer->GetConcreteBeadTotal() << " " << pExtPolymer->GetEmptyGhostBeadTotal() << zEndl;		
		}
        std::cout << "********************************************************************************" << zEndl;
	}
	
#endif
}

// Protected helper function to write out information on the extended bonds owned by this processor.
// Note that the extended bond map has each bond stored twice - using its head and tail bead ids as the keys.

void mpsSimBox::ListExtendedBonds() const
{
#if EnableParallelSimBox == SimMPSEnabled

    std::cout << "********************************************************************************" << zEndl;
	std::cout << "Proc " << GetRank() << " at time " << IGlobalSimBox::Instance()->GetCurrentTime() << " has " << m_mmExtendedBonds.size()/2 << " extended bonds spanning processor boundaries:" << zEndl;
		
	for(cLongExtendedBondMMIterator iterExtBond=m_mmExtendedBonds.begin(); iterExtBond!=m_mmExtendedBonds.end(); iterExtBond++)
	{
		iterExtBond->second->Dump();
	}
    std::cout << "********************************************************************************" << zEndl;
	
#endif
}

// Protected helper function to write out information on a single extended polymer owned by this processor. If the polymer is not found, we do nothing.

void mpsSimBox::ListExtendedBond(long bondId) const
{
#if EnableParallelSimBox == SimMPSEnabled

	if(m_mmExtendedBonds.find(bondId) != m_mmExtendedBonds.end())
	{				
//	    long                        key = (m_mmExtendedBonds.find(bondId))->first;
		mpuExtendedBond* pExtBond       = (m_mmExtendedBonds.find(bondId))->second;
									 
        std::cout << "********************************************************************************" << zEndl;
	    std::cout << "Proc " << GetRank() << " at time " << IGlobalSimBox::Instance()->GetCurrentTime() << " dumping extended bond  " << bondId << zEndl;
		pExtBond->Dump();
        std::cout << "********************************************************************************" << zEndl;
	}
	
#endif
}

// Protected function used by derived classes to retrieve an extended polymer instance from the extended polymers map given its 
// unique id as the key. This is used when passing ghost bead coordinates between processors.

mpuExtendedPolymer* mpsSimBox::GetExtendedPolymerFromId(long id) const
{
    mpuExtendedPolymer* pPolymer = 0;
	
	if(m_mExtendedPolymers.find(id) != m_mExtendedPolymers.end())
	{
		pPolymer = m_mExtendedPolymers.find(id)->second;
	}
	else
	{
		std::cout << "Proc " << GetRank() << " Error in mpsSimBox: extended polymer with id " << id << " is not in the extended polymers map" << zEndl;
	}
	
	return pPolymer;
}

// Function to replenish the store of empty CPolymer instances that wrap incoming beads that arrive from other processors.

bool mpsSimBox::CreateEmptyPolymers()
{
    std::cout << "**********  **********" << zEndl;
    for(long j=0; j<m_InitialEmptyPolymers; j++)
    {
        m_lEmptyPolymers.push_back(new CPolymer());
    }
	std::cout << "Created " << m_InitialEmptyPolymers << " new empty polymer instances" << zEndl;
    std::cout << "**********  **********" << zEndl;
	
	if(m_lEmptyPolymers.size() < m_InitialEmptyPolymers)
	    return false;
	else
	    return true;
}

// Function to replenish the store of empty extended polymers that wrap incoming polymers from other processors.
// // Not used yet.

bool mpsSimBox::CreateEmptyExtendedPolymers()
{


    return true;
}

// Private function to dump out the contents of an extended polymer for debug purposes. The first argument is the polymer id, and
// the second is a flag that indicates whether to dump all the processor's extended polymers as well.

void mpsSimBox::DumpExtendedPolymer(long polyId, bool bDumpAll)
{
    const long pid=GetRank();
	
	for(long i=0; i<10; i++)
	{
	    std::cout << pid;
	}
	std::cout << " Proc " << GetRank() << " dumping extended polymer " << polyId << zEndl;

	ListExtendedPolymer(polyId);
	
	if(bDumpAll)
	{
	    std::cout << " Proc " << GetRank() << " dumping all extended polymers " << zEndl;
		ListExtendedPolymers();
    }
}

// Private function to dump out the contents of an extended bond for debug purposes. 

void mpsSimBox::DumpExtendedBond(long bondId, bool bDumpAll)
{
    const long pid=GetRank();
	
	for(long i=0; i<10; i++)
	{
	    std::cout << pid;
	}
	std::cout << " Proc " << GetRank() << " dumping extended bond " << bondId << zEndl;

	ListExtendedBond(bondId);
	
	if(bDumpAll)
	{
	    std::cout << " Proc " << GetRank() << " dumping all extended bonds " << zEndl;
		ListExtendedBonds();
    }
}

// Function to return a vector of all multi-bead polymers that are wholly contained in the owning processor's Bulk space.
// NB This excludes single-bead Water polymers.
//
// This is used by commands that want to manipulate the polymers and their beads, but which cannot deal with 
// extended polymers. We copy the polymers from the bulk map into a vector for later ease of access.

PolymerVector mpsSimBox::GetAllBulkPolymers() const
{
    PolymerVector vAllPolymers;
    vAllPolymers.clear();
    
	for(cLongPolymerIterator citerPoly=m_mBulkPolymers.begin(); citerPoly!=m_mBulkPolymers.end(); citerPoly++)
	{
		vAllPolymers.push_back(citerPoly->second);
	}

    return vAllPolymers;
}

// Access functions to the CNanoparticles created during a simulation. Note that CSimBox creates and destroys the CNanoparticle 
// instances, but mpsSimBox updates the dynamic bonds' forces. All nanoparticles are assumed to be created wholly within a single 
// processor's space.

void mpsSimBox::AddNanoparticle(CNanoparticle* const pNano)
{
    m_mBulkNanoparticles.insert(zPairLongNanoparticle(pNano->GetId(), pNano));
    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " added Bulk nanoparticle with id " << pNano->GetId() << zEndl;
}

// Function to check if any Bulk NPs have have moved into the Border region. If so, we wrap them in an mpuExtendedNanoparticle instance 
// and transfer them to the Border container. The condition for transfer is that the NP is within its largest dimension of one of the  
// SimBox boundaries.

void mpsSimBox::UpdateBulkNanoparticles()
{
    for(LongNanoparticleIterator iterNP=m_mBulkNanoparticles.begin(); iterNP!=m_mBulkNanoparticles.end(); ++iterNP)
    {
        CNanoparticle* pNano = iterNP->second;        
        
        if(pNano->IsBorder(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength, m_DX, m_DY, m_DZ)) 
        {
            mpuExtendedNanoparticle* pExtNano = new mpuExtendedNanoparticle(pNano);
        
            m_mBorderNanoparticles.insert(make_pair(pExtNano->GetId(), pExtNano));
        
            m_mBulkNanoparticles.erase(pNano->GetId());
            
            std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " moving Bulk nanoparticle with id " << pNano->GetId() << " to Border: Bulk/Border totals now " << m_mBulkNanoparticles.size() << " " << m_mBorderNanoparticles.size() << zEndl;
        }
    }
}


// Function to check if any Border NPs have lost beads in the latest UpdatePos call or if they have moved away from the Border and 
// back into the Bulk region. In the latter case we unwrap them, insert the original nanoparticle back into the Bulk mape and 
// destroy the wrapper nanoparticle instance.

void mpsSimBox::UpdateBorderNanoparticles()
{
    for(LongExtendedNanoparticleIterator iterNP=m_mBorderNanoparticles.begin(); iterNP!=m_mBorderNanoparticles.end(); ++iterNP)
    {
        mpuExtendedNanoparticle* pExtNano = iterNP->second;
        CNanoparticle*           pNano    = pExtNano->GetNanoparticle();
        
        if(!pNano->IsBorder(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength, m_DX, m_DY, m_DZ))
        {
            // Validate the nanoparticle here by checking that all bonds are valid and all beads exist
            
            
             std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " moving Border nanoparticle with id " << pNano->GetId() << " to Bulk " << zEndl;
           
            
            // Move the wrapped nanoparticle back into the Bulk map and remove the extended nanoparticle from the Border map and destroy it.
            
            m_mBulkNanoparticles.insert(make_pair(pNano->GetId(), pNano));
            
            m_mBorderNanoparticles.erase(pExtNano->GetId());        
            delete pExtNano;
        }
        else
        {
            // Now check Border nanoparticles to see if beads have moved to other processors or other beads have returned
            
            
        
        
        }
    }
}




