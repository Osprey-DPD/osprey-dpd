/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ExternalCNTCell.cpp: implementation of the CExternalCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimMPSFlags.h"
#include "ExternalCNTCell.h"
#include "Monitor.h"
#include "mpsSimBox.h"
#include "Bead.h"
#include "xxParallelBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor that converts an existing CCNTCell instance into an external instance.
// It copies the original cell's id and other data into the new one so that
// bead-bead force calculations across processor boundaries can be calculated.

CExternalCNTCell::CExternalCNTCell(CCNTCell* const pCell) : CCNTCell(pCell->m_id, pCell->m_BLIndex, pCell->m_bExternal),
                                                            m_pCell(pCell)
{
    // We don't copy the bead list as this class holds bead information from 
    // a neighbouring processor not beads in the same processor. 

    // Ensure the local data containers are empty.

    m_vBeadIds.clear();
    m_vForceCounter.clear();
    m_vBeadTypes.clear();  
    m_vBeadRadii.clear();

    m_vLX.clear();
    m_vLY.clear();    
    m_vLZ.clear();

    m_vVX.clear();
    m_vVY.clear();
    m_vVZ.clear();

    m_vFX.clear();
    m_vFY.clear();
    m_vFZ.clear();

}

CExternalCNTCell::~CExternalCNTCell()
{

}

// Function used to calculate the forces between the passed-in bead  and
// all beads in the current (external) cell. This function is called when 
// a Border cell in one processor has to calculate the forces between its beads
// and those owned by a neighbouring processor. The neighbour cell of the original 
// cell is an external cell and, instead of calling the internal version of the
// UpdateForceBetween Cells() calls this function. An external cell does not 
// contain bead instances but only the bead coordinates in local storage.
// Note that it must be called only after this storage has been updated with
// the coordinates of all beads from the neighbouring processor.
//
// We store the resulting forces so that they can be passed back 
// to the originating processor and added to the forces on its beads. They are
// stored in the same order as the beads in the external cell. Note that
// this also means the cell must clear the force vectors each time step to avoid
// incorrect allocation of forces to beads.

void CExternalCNTCell::UpdateForceBetweenCells(bool bExternal, CAbstractBead* const pBead)
{
//    const long targetBeadId = 4; // Write out interactions for this bead only
	
    // Iterate over all stored bead coordinates, calculating the force with
    // the passed-in bead. Note that we know the passed-in bead is not in the same
    // cell as the current cell. Also the stored bead coordinates must not be shifted 
    // into the same cell as the PBC check will then fail.


    for(long i=0; i<GetBeadTotal(); i++)
    {
        const long id    = m_vBeadIds.at(i);
        const long type  = m_vBeadTypes.at(i);
        const double  x  = m_vLX.at(i);
        const double  y  = m_vLY.at(i);
        const double  z  = m_vLZ.at(i);
        const double  vx = m_vVX.at(i);
        const double  vy = m_vVY.at(i);
        const double  vz = m_vVZ.at(i);

#if SimIdentifier == BD

        // Not implemented yet

#else
    // DPD and MD equations of motion

	    BeadListIterator iterBead2;

	    double dx[3], dv[3], newForce[3];
	    double dr, dr2;

#if SimIdentifier == DPD

	    double gammap, rdotv, wr, wr2;
	    double conForce, dissForce, randForce;

#ifdef UseDPDBeadRadii
	    double drmax;
#endif

#elif SimIdentifier == MD

	    double magLJ;						// LJ potential variables
	    double eLJ, sLJOverR, sLJR3, sLJR6;

	    double magSC;						// SC potential variables
	    double eSC, sSCOverR, sSCR3;

#endif

		// Add in interactions of the passed-in bead with beads in the current
        // cell taking the PBCs into account. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

		dx[0] = (pBead->m_Pos[0] - x);
		dv[0] = (pBead->m_Mom[0] - vx);

		dx[1] = (pBead->m_Pos[1] - y);
		dv[1] = (pBead->m_Mom[1] - vy);

#if SimDimension == 2
		dx[2] = 0.0;
		dv[2] = 0.0;
#elif SimDimension == 3
		dx[2] = (pBead->m_Pos[2] - z);
		dv[2] = (pBead->m_Mom[2] - vz);
#endif


		if( IsExternal() && bExternal )
		{
			if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
			{
			    std::cout << "Particle coords positive great in X: " << dx[0] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[0] << " " <<  x << zEndl;
				dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
			}
			else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
			{
			    std::cout << "Particle coords negative great in X: " << dx[0] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[0] << " " <<  x << zEndl;
				dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;
			}
			if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
			{
			    std::cout << "Particle coords positive great in Y: " << dx[1] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[1] << " " <<  y << zEndl;
				dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
			}
			else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
			{
			    std::cout << "Particle coords negative great in Y: " << dx[1] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[1] << " " <<  y << zEndl;
				dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;
			}

#if SimDimension == 3
			if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
			{
			    std::cout << "Particle coords positive great in Z: " << dx[2] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[2] << " " <<  z << zEndl;
				dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
			}
			else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
			{
			    std::cout << "Particle coords negative great in Z: " << dx[2] << " (" <<  pBead->GetId() << " " << id << ") " << pBead->m_Pos[2] << " " <<  z << zEndl;
				dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
			}
#endif

		}

		dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we have to change the distance-dependence according
// to the bead interaction radii.

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
		if( dr2 < 1.0 )
		{		
			dr = sqrt(dr2);
			if( dr > 0.000000001 )
			{
				wr = (1.0 - dr);
				wr2 = wr*wr;
#else
		dr = sqrt(dr2);
		drmax = pBead->GetRadius()+m_vBeadRadii.at(i);
		if( dr < drmax )
		{		
			if( dr > 0.000000001 )
			{
				wr = (1.0 - dr/drmax);
				wr2 = wr*wr;
#endif
				conForce	= m_vvConsInt.at(pBead->GetType()).at(type)*wr;		
//				conForce = 0.0;		

				rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
				gammap		= m_vvDissInt.at(pBead->GetType()).at(type)*wr2;

				dissForce	= -gammap*rdotv;				
				randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());

				newForce[0] = (conForce + dissForce + randForce)*dx[0]/dr;
				newForce[1] = (conForce + dissForce + randForce)*dx[1]/dr;
				newForce[2] = (conForce + dissForce + randForce)*dx[2]/dr;
				
#elif SimIdentifier == MD

		if( dr2 < m_coradius2 )
		{		
			dr = sqrt(dr2);

			if( dr > 0.000000001 )
			{
				// Calculate common factors in the LJ potential and include shifted force.
				// Note that the shifted force term needs to be multiplied by the 
				// unit vector in the bead-bead separation whereas the force term 
				// has the full vector.

				eLJ       = m_vvLJDepth.at(pBead->GetType()).at(type);	
				sLJOverR  = m_vvLJRange.at(pBead->GetType()).at(type)/dr;
				sLJR3     = sLJOverR*sLJOverR*sLJOverR;
				sLJR6     = sLJR3*sLJR3;

				magLJ = (6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0) 
						 - dr*m_vvLJSlope.at(pBead->GetType()).at(type))/dr2;

				// Only add the soft-core potential if the potential depth is non-zero

				eSC = m_vvSCDepth.at(pBead->GetType()).at(type);	

				if(eSC > 0.0)
				{
					sSCOverR = m_vvSCRange.at(pBead->GetType()).at(type)/dr;
					sSCR3    = sSCOverR*sSCOverR*sSCOverR;

					magSC = (9.0*eSC*sSCR3*sSCR3*sSCR3
							 - dr*m_vvSCSlope.at(pBead->GetType()).at(type))/dr2;
				}
				else
				{
					magSC = 0.0;
				}

				newForce[0] = (magLJ + magSC)*dx[0];
				newForce[1] = (magLJ + magSC)*dx[1];
				newForce[2] = (magLJ + magSC)*dx[2];
#endif

//                if((pBead->GetId() == targetBeadId || id == targetBeadId))
//				{
//				    std::cout << "ExternalCNTCell " << GetId() << " calculating external force between beads " << pBead->GetId() << " " << id << " " << x << " " << y << " " << z << zEndl;
//				}
				
				pBead->m_ForceCounter++;
		        m_vForceCounter.at(i) = m_vForceCounter.at(i) + 1;
				
				pBead->m_Force[0] += newForce[0];
				pBead->m_Force[1] += newForce[1];
				pBead->m_Force[2] += newForce[2];

                // Next lines store the force on each bead in this cell 
                // to send back to the neighbouring processor that owns them.

                m_vFX.at(i)  = m_vFX.at(i) - newForce[0];
                m_vFY.at(i)  = m_vFY.at(i) - newForce[1];
                m_vFZ.at(i)  = m_vFZ.at(i) - newForce[2];
				
				// Next calculate any bond forces between the passed-in bead and other beads from the neighbouring processor. We use the owning
				// polymer to get the bonds containing the passed-in bead and then extract their opposite bead ids and compare them to the current
				// cell's beads.

				// stress tensor summation

				pBead->m_Stress[0] += dx[0]*newForce[0];
				pBead->m_Stress[1] += dx[1]*newForce[0];
				pBead->m_Stress[2] += dx[2]*newForce[0];
				pBead->m_Stress[3] += dx[0]*newForce[1];
				pBead->m_Stress[4] += dx[1]*newForce[1];
				pBead->m_Stress[5] += dx[2]*newForce[1];
				pBead->m_Stress[6] += dx[0]*newForce[2];
				pBead->m_Stress[7] += dx[1]*newForce[2];
				pBead->m_Stress[8] += dx[2]*newForce[2];

				// Pass the stress tensor components to the CMonitor
				// for use in analysing the stress over slices.
				// Note that dx[2] might be close to zero and we should
				// check for this.

				// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
						m_pMonitor->AddBeadStress(pBead, *iterBead2, newForce, dx);
#endif
			}
			else
			{
				TraceInt("neighbour cell bead", pBead->GetId());
				TraceInt("interacts with", id);
				TraceVector("1st pos",	pBead->GetXPos(),
										pBead->GetYPos(), 
										pBead->GetZPos() );
				TraceVector("2nd pos",	x, y, z );
				TraceDouble("Bead distance", dr);
			}
		}
#endif
    }

}

// Function used by Border regions to add beads to the external CNT cells preparatory
// to calculating non-bonded bead-bead forces between beads in adjacent processors.
// We also store a zero in the force container so that it has the storage needed to
// allow direct access when the forces are calculated.

void CExternalCNTCell::AddBeadToCell(long id, long type, double x, double y, double z, double vx, double vy, double vz)
{
//    std::cout << "Adding bead " << id << " " << type << " " << x << " " << y << " " << z << " to cell: " << zEndl;
//    std::cout << GetBLXIndex() << " " << GetBLYIndex() << " " << GetBLZIndex() << " ";
//    std::cout << m_BLCoord[0] << " " << m_BLCoord[1] << " " << m_BLCoord[2] << zEndl;

    m_vBeadIds.push_back(id);
	m_vForceCounter.push_back(0);
    m_vBeadTypes.push_back(type);
    m_vLX.push_back(x);
    m_vLY.push_back(y);
    m_vLZ.push_back(z);
    m_vVX.push_back(vx);
    m_vVY.push_back(vy);
    m_vVZ.push_back(vz);
    m_vFX.push_back(0.0);
    m_vFY.push_back(0.0);
    m_vFZ.push_back(0.0);
}

// Function to empty all local storage preparatory to new beads being added to the cell.

void CExternalCNTCell::Clear()
{
    m_vBeadIds.clear();
	m_vForceCounter.clear();
    m_vBeadTypes.clear();  
    m_vBeadRadii.clear();

    m_vLX.clear();
    m_vLY.clear();    
    m_vLZ.clear();

    m_vVX.clear();
    m_vVY.clear();
    m_vVZ.clear();

    m_vFX.clear();
    m_vFY.clear();
    m_vFZ.clear();
}

// Functions to return the components of the accumulated force on the ith bead 
// in this cell.

double CExternalCNTCell::GetXForceOnBead(long i) const
{
    if(i>=0 && i<GetBeadTotal())
    {
        return m_vFX.at(i);
    }
    else
    {
	    std::cout << "Error in cell " << GetId() << " X Force on bead index out of range " << i << zEndl;
        return 0.0;
    }
}

double CExternalCNTCell::GetYForceOnBead(long i) const
{
    if(i>=0 && i<GetBeadTotal())
    {
        return m_vFY.at(i);
    }
    else
    {
	    std::cout << "Error in cell " << GetId() << " Y Force on bead index out of range " << i << zEndl;
        return 0.0;
    }
}

double CExternalCNTCell::GetZForceOnBead(long i) const
{
    if(i>=0 && i<GetBeadTotal())
    {
        return m_vFZ.at(i);
    }
    else
    {
	    std::cout << "Error in cell " << GetId() << " Z Force on bead index out of range " << i << zEndl;
        return 0.0;
    }
}

// Function to add bond forces between beads in this external cell and beads in the owning processor's Space.
// We iterate over the beads in this external cell and pass them to the mpsSimBox to check its extended bond map.
// For each bond that contains the external bead it calculates the bond force and passes it back here via the arguments and we accumulate it
// in the bead's force arrays.

void CExternalCNTCell::UpdateBondForceBetweenCells(mpsSimBox* const pSimBox)
{
    double fx, fy, fz;  // Bond force components
	
    for(long i=0; i<GetBeadTotal(); i++)
    {
        const long   id  = m_vBeadIds.at(i);
        const double  x  = m_vLX.at(i);
        const double  y  = m_vLY.at(i);
        const double  z  = m_vLZ.at(i);

        fx = 0.0;
        fy = 0.0;
        fz = 0.0;
		
        pSimBox->AddExternalBondForcesToBeads(id, x, y, z, &fx, &fy, &fz);
		
		m_vFX.at(i) += fx;
		m_vFY.at(i) += fy;
		m_vFZ.at(i) += fz;
    }
	
}
	
	
	

