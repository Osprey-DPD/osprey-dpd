/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// MicelleBuilder.cpp: implementation of the CMicelleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "MicelleBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// Needed for random number generator
#include "LogSimErrorTrace.h"   // Needed to issue error messages


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicelleBuilder::CMicelleBuilder(zLongVector polymerTypes, double xc, double yc, double zc, 
							double radius) : m_PolymerTypes(polymerTypes),
															  m_XC(xc), m_YC(yc), m_ZC(zc),
															  m_Radius(radius)
{
}

CMicelleBuilder::~CMicelleBuilder()
{

}

// Virtual function to assemble beads into a micelle initial configuration.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required micelle configuration. The polymers that compose the 
// micelle have already been created using a bulk concentration specified by
// the user. 
//
// The tail bead of each micelle polymer is positioned in the centre of the
// micelle and the head beads on the surfacew of a sphere with the specified 
// radius. Note that the user can set the radius (as long as the micelle lies
// within the SimBox and does not overlap any wall present) so that polymers
// may be squashed or stretched beyond their preferred length.
//

bool CMicelleBuilder::Assemble(CInitialState& riState)
{
    bool bSuccess = true;
	long index = 0;	// counter used everywhere below
	cPolymerVectorIterator iterPoly;
	cBeadVectorIterator    iterBead;

	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Create a micelle

	isMicelle micelle(m_PolymerTypes, m_XC, m_YC, m_ZC, m_Radius);

	micelle.Assemble(riState);

	// Calculate the free space available between the micelle and the walls, and
	// fill it with the remaining polymers in the simulation. We assign coordinates
	// randomly to the beads and then move them if they overlap with the micelle
	// volume.

	// Store the polymer types that don't make up the micelle

	zLongVector nonMicellePolymerTypes;

	nonMicellePolymerTypes.clear();

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) == m_PolymerTypes.end())
		{
			nonMicellePolymerTypes.push_back(ip);
		}	
	}

	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a random region.

	PolymerVector vPolymers = riState.GetPolymers();
	PolymerVector vRandomPolymers;

	vRandomPolymers.clear();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(nonMicellePolymerTypes.begin(), nonMicellePolymerTypes.end(), ip) != nonMicellePolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vRandomPolymers));
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	// Exclude any CNT cells that contain wall beads. We just mark the cells whose
	// indices are within the walls. We first set a flag in a vector showing that
	// the corresponding index is for a free CNT cell, next we exclude all cells
    // that lie within the wall regions, and finally we copy all the 
	// free CNT cell indeices into another vector to allow them to be randomly accessed
	// using a second index into the vector.

	const long CNTXNo = riState.GetCNTXCellNo();
	const long CNTYNo = riState.GetCNTYCellNo();
	const long CNTZNo = riState.GetCNTZCellNo();
  
	zLongVector vAllFreeCNTCells(CNTXNo*CNTYNo*CNTZNo);

	for(long unsigned int j=0; j<vAllFreeCNTCells.size(); j++)
	{
		vAllFreeCNTCells.at(j) = 1;
	}

	long xCellId, yCellId, zCellId;

	if(riState.GetWallXCellNo() != 0)	// X wall
	{
		for(xCellId=0; xCellId<riState.GetWallXCellNo(); xCellId++)
		{
			for(yCellId=0; yCellId<CNTYNo; yCellId++)
			{
				for(zCellId=0; zCellId<CNTZNo; zCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
          
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + CNTXNo-xCellId-1;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	if(riState.GetWallYCellNo() != 0)	// Y wall
	{
		for(yCellId=0; yCellId<riState.GetWallYCellNo(); yCellId++)
		{
			for(xCellId=0; xCellId<CNTXNo; xCellId++)
			{
				for(zCellId=0; zCellId<CNTZNo; zCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;

					index = CNTXNo*(CNTYNo*zCellId + CNTYNo-yCellId-1) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	if(riState.GetWallZCellNo() != 0)	// Z wall
	{
		for(zCellId=0; zCellId<riState.GetWallZCellNo(); zCellId++)
		{
			for(xCellId=0; xCellId<CNTXNo; xCellId++)
			{
				for(yCellId=0; yCellId<CNTYNo; yCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
					index = CNTXNo*(CNTYNo*(CNTZNo-zCellId-1) + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	// Now exclude the CNT cells than contain beads within the micelle. We perform
    // checks to ensure that all such cells lie within the simulation box, and
    // abort the run if not. If the user specifies a micelle radius that
    // is too small to allow all the polymers to be positioned between the
    // (program-defined) inner radius and (user-defined) outer radius, then some
    // of the polymer beads may have coordinates outside the simulations box.
    // We return false to abort the run.

	const double CNTXCellWidth = riState.GetCNTXCellWidth();
	const double CNTYCellWidth = riState.GetCNTYCellWidth();
	const double CNTZCellWidth = riState.GetCNTZCellWidth();

	long ix, iy, iz;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(bSuccess && find(m_PolymerTypes.begin(), m_PolymerTypes.end(), (*iterPoly)->GetType()) != m_PolymerTypes.end())
		{
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				ix = static_cast<long>((*iterBead)->GetXPos()/CNTXCellWidth);
				iy = static_cast<long>((*iterBead)->GetYPos()/CNTYCellWidth);

#if SimDimension == 2
				iz = 0;
#elif SimDimension == 3
				iz = static_cast<long>((*iterBead)->GetZPos()/CNTZCellWidth);		
#endif
				index = CNTXNo*(CNTYNo*iz+iy) + ix;

                if(index < riState.GetCNTCellTotal())
                {
				    vAllFreeCNTCells.at(index) = 0;
                }
                else
                {
                    bSuccess = false;
                }
			}
		}
	}

    // Abort the run on error
    if(!bSuccess)
    {
        new CLogSimErrorTrace(0, "Error in micelle builder (outer radius too large for box): stopping run");
        return bSuccess;
    }

	// Now copy the remaining free CNT cell indices to the vFreeCNTCells vector

	zLongVector vFreeCNTCells;

	for(long j=0; j<CNTXNo*CNTYNo*CNTZNo; j++)
	{
		if(vAllFreeCNTCells.at(j) == 1) {
			vFreeCNTCells.push_back(j); }
	}

	// Now assign coordinates to the beads in non-micelle polymers among
	// the free CNT cells

	double xp[3];
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;

	const long freeCells = vFreeCNTCells.size();

	long rCellIndex;
	long rCNTCellIndex;
	bool bFirstBead;
	double xFirstBead = 0.0;
        double yFirstBead = 0.0;
        double zFirstBead = 0.0;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), (*iterPoly)->GetType()) == m_PolymerTypes.end())
		{
		    bFirstBead = true;
			
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
			    if(bFirstBead)
				{
				    bFirstBead = false;
				
				    // Pick the cellcontaining the first bead randomly from those outside the vesicle. This avoids the new
					// algorithm breaking the assignment of positions for single-bead polymers such as water.
					
				    rCellIndex = static_cast<long>(freeCells*CCNTCell::GetRandomNo());
				    if(rCellIndex == freeCells) {
					    rCellIndex = 0; }

				    rCNTCellIndex = vFreeCNTCells.at(rCellIndex); // cells outside  vesicle

				    ix = rCNTCellIndex%CNTXNo;
				    iy = ((rCNTCellIndex-ix)/CNTXNo)%CNTYNo;
				    iz = ((rCNTCellIndex-ix)/CNTXNo - iy)/CNTYNo;

				    xp[0] = riState.GetCNTXCellWidth()*(ix + CCNTCell::GetRandomNo());
				    xp[1] = riState.GetCNTYCellWidth()*(iy + CCNTCell::GetRandomNo());
				    xp[2] = riState.GetCNTZCellWidth()*(iz + CCNTCell::GetRandomNo());

				    (*iterBead)->SetXPos(xp[0]);
				    (*iterBead)->SetYPos(xp[1]);
				    (*iterBead)->SetZPos(xp[2]);
					
					xFirstBead = xp[0];
					yFirstBead = xp[1];
					zFirstBead = xp[2];
				}
				else
				{
				    // Position the polymer's subsequent beads randomly but close to the first bead
					
				    xp[0] = xFirstBead + riState.GetCNTXCellWidth()*(CCNTCell::GetRandomNo());
				    xp[1] = yFirstBead + riState.GetCNTYCellWidth()*(CCNTCell::GetRandomNo());
				    xp[2] = zFirstBead + riState.GetCNTZCellWidth()*(CCNTCell::GetRandomNo());
					
					// Ensure that subsequent beads are not placed outside the free volume of the SimBox, ie inside any walls or
					// outside the whole box.
					
					if(xp[0] < riState.GetWallXWidth())
					{
					    xp[0] = riState.GetWallXWidth() + riState.GetCNTXCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[0] > (riState.GetSimBoxXLength() - riState.GetWallXWidth()))
					{
					    xp[0] = riState.GetSimBoxXLength() - riState. GetWallXWidth() - riState.GetCNTXCellWidth()*CCNTCell::GetRandomNo();
					}
					(*iterBead)->SetXPos(xp[0]);					
					
					
					if(xp[1] < riState.GetWallYWidth())
					{
					    xp[1] = riState.GetWallYWidth() + riState.GetCNTYCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[1] > (riState.GetSimBoxYLength() - riState.GetWallYWidth()))
					{
					    xp[1] = riState.GetSimBoxYLength() - riState. GetWallYWidth() - riState.GetCNTYCellWidth()*CCNTCell::GetRandomNo();
					}
				    (*iterBead)->SetYPos(xp[1]);
					
					if(xp[2] < riState.GetWallZWidth())
					{
					    xp[2] = riState.GetWallZWidth() + riState.GetCNTZCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[2] > (riState.GetSimBoxZLength() - riState.GetWallZWidth()))
					{
					    xp[2] = riState.GetSimBoxZLength() - riState. GetWallZWidth() - riState.GetCNTZCellWidth()*CCNTCell::GetRandomNo();
					}
				    (*iterBead)->SetZPos(xp[2]);
				}
			}

			// Check that pairs of beads that are bonded together are not too far apart 
			// that they generate huge bond forces. This arises because their positions 
			// are just randomly selected from the free space and for boxes of 
			// 20 x 20 x 20 or more this can lead to large separations. If they are 
			// further apart than "rbond", move them along the line joining their centres 
			// so that they are only unit distance apart.

			const double rbond = 1.0;
			double rmag;
			double rdiff[3], runit[3];

			for(cBondVectorIterator iterBond=(*iterPoly)->GetBonds().begin(); iterBond!= (*iterPoly)->GetBonds().end(); iterBond++)
			{
				rdiff[0] = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
				rdiff[1] = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();
				rdiff[2] = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();

				rmag = sqrt(rdiff[0]*rdiff[0] + rdiff[1]*rdiff[1] + rdiff[2]*rdiff[2]);

				runit[0] = rdiff[0]/rmag;
				runit[1] = rdiff[1]/rmag;
				runit[2] = rdiff[2]/rmag;

				if( fabs(rdiff[0]) > rbond )
				{
					(*iterBond)->GetTail()->SetXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
				}

				if( fabs(rdiff[1]) > rbond )
				{
					(*iterBond)->GetTail()->SetYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
				}

				if( fabs(rdiff[2]) > rbond )
				{
					(*iterBond)->GetTail()->SetZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
				}
			}
		}
	}

    // Abort the run on error
    if(!bSuccess)
    {
        new CLogSimErrorTrace(0, "Error in micelle builder (unable to assign coordinates to non-micelle polymers): stopping run");
        return bSuccess;
    }


	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the velocities of all the beads from a Maxwell distribution 
	// with a mean  value and variance fixed from the temperature.
	// We do it this way so that the total CM momentum is zero. If the micelle
	// beads have a non-zero CM momentum it should exchange with the surrounding
	// water and dissipate to zero. The rate at which this happens may be 
	// use ful to monitor.
	//
	// The bead velocities are selected randomly from a much larger vector 
	// of velocities taken from a Maxwell distribution with unit mean and variance.
	// If the number of values needed exceeds 50000 then the actual number required
	// is used. 

	zDoubleVector rvelDist = MaxwellDist(riState.GetBeadTotal());

	zDoubleVector velXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velZDist(riState.GetBeadTotal(), 0.0);

	double vmag, vtheta, vphi;
	double vp[3];
	double vcm[3];

	vcm[0] = 0.0;
	vcm[1] = 0.0;
	vcm[2] = 0.0;


	for(long iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
		vmag	= rvelDist.at(index);
		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

#if SimDimension == 3
		vtheta	= acos(1.0-2.0*CCNTCell::GetRandomNo());

		vp[0]   = vmag*sin(vtheta)*cos(vphi);
		vp[1]   = vmag*sin(vtheta)*sin(vphi);
		vp[2]   = vmag*cos(vtheta);
#elif SimDimension == 2
		vp[0]   = vmag*cos(vphi);
		vp[1]   = vmag*sin(vphi);
		vp[2]   = 0.0;
#endif
		vcm[0] += vp[0];
		vcm[1] += vp[1];
		vcm[2] += vp[2];

		velXDist.at(iBead) = vp[0];
		velYDist.at(iBead) = vp[1];
		velZDist.at(iBead) = vp[2];
	}

	double vmean[3], v2mean[3];

	for(short i=0; i<3; i++)
	{
		vcm[i]	   /= static_cast<double>(riState.GetBeadTotal());
		vmean[i]	= 0.0;
		v2mean[i]	= 0.0;
	}

	// remove CM velocity from bead velocities

	double vtotal = 0.0;

	for(long iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		velXDist.at(iBead)  -= vcm[0];
		velYDist.at(iBead)  -= vcm[1];
		velZDist.at(iBead)  -= vcm[2];

		vtotal = vtotal + velXDist.at(iBead)*velXDist.at(iBead) +
						  velYDist.at(iBead)*velYDist.at(iBead) +
						  velZDist.at(iBead)*velZDist.at(iBead);
	}

	// Normalize the velocities according to the dimension of the simulation
	// and the number of beads. 

#if SimDimension == 2
	vtotal = sqrt(vtotal/(2.0*static_cast<double>(riState.GetBeadTotal())*riState.GetkT()));
#elif SimDimension == 3
	vtotal = sqrt(vtotal/(3.0*static_cast<double>(riState.GetBeadTotal())*riState.GetkT()));
#endif

	// finally normalize the velocities to the required temperature, 
	// calculate the mean and variance, and assign the bead velocities

	for(long iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		velXDist.at(iBead)	= velXDist.at(iBead)/vtotal;
		velYDist.at(iBead)	= velYDist.at(iBead)/vtotal;
		velZDist.at(iBead)  = velZDist.at(iBead)/vtotal;

		(riState.GetBeads()).at(iBead)->SetXMom(velXDist.at(iBead));
		(riState.GetBeads()).at(iBead)->SetYMom(velYDist.at(iBead));
		(riState.GetBeads()).at(iBead)->SetZMom(velZDist.at(iBead));

		vmean[0] += velXDist.at(iBead);
		vmean[1] += velYDist.at(iBead);
		vmean[2] += velZDist.at(iBead);

		v2mean[0] += velXDist.at(iBead)*velXDist.at(iBead);
		v2mean[1] += velYDist.at(iBead)*velYDist.at(iBead);
		v2mean[2] += velZDist.at(iBead)*velZDist.at(iBead);
	}

	for(long jv=0; jv<3; jv++)
	{
		vmean[jv]  =  vmean[jv]/static_cast<double>(riState.GetBeadTotal());
		v2mean[jv] = v2mean[jv]/static_cast<double>(riState.GetBeadTotal());
	}


#ifdef TraceOn

       double var[3];
       
	var[0] = v2mean[0] - vmean[0]*vmean[0];
	var[1] = v2mean[1] - vmean[1]*vmean[1];
	var[2] = v2mean[2] - vmean[2]*vmean[2];

	Trace("Initial vel distn");
	TraceVector("Mean Vel      = ", vmean[0],  vmean[1],  vmean[2]);
	TraceVector("Mean Sq Vel   = ", v2mean[0], v2mean[1], v2mean[2]);
	TraceVector("Var Vel       = ", var[0],    var[1],    var[2]);
	Trace("********************");
#endif

	return bSuccess;
}
