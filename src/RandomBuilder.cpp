/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RandomBuilder.cpp: implementation of the CRandomBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "RandomBuilder.h"
#include "InitialState.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "pmInteger.h"
#include "pmDouble.h"
#include "pmDouble3d.h"
#include "xxParallelBase.h"     // Needed to get owning processor's rank

#include "CNTCell.h"			// needed for random number generator
#include "LogBuilderError.h"	// needed to output any error messages
#include "LogVelDistMessage.h"	// needed to output the initial velocity distribution



// Uncomment the next flag to recreate the previous (wrong) normalisation for the
// initial random velocities.

// #define PreVelocity3dNormalisation 1

// and uncomment the next one to recreate the initialisation without checking that
// the beads are situated within the SimBox boundaries.

// #define PreRandomBuilderSimBoxCheck 1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRandomBuilder::CRandomBuilder()
{

}

CRandomBuilder::~CRandomBuilder()
{

}

// Virtual function to assemble beads into a random initial configuration.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required initial configuration. The 
// beads are distributed randomly throughout the simulation box and 
// their velocities are taken from a Maxwellian distribution with a temperature
// set by the parameter kT.
//

bool CRandomBuilder::Assemble(CInitialState& riState)
{
	long i;	// loop counter used several times below. Its scope in for() loops
			// seems to be platform-dependent.

	// The CInitialState object has created the beads, bonds and polymers so 
	// we only have to assign their randomised coordinates here.

	// Assign the Wall bead coordinates first because they have fixed positions 
	// and may be bonded to beads in the bulk of the SimBox. We have to adjust 
	// the lengths of bonds in all bonded polymers so that they are not so large
	// as to generate huge forces that can screw up the integration.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Arrange other beads in a random configuration and set their velocities 
	// from a Maxwell distribution in each dimension with a mean value and 
	// variance fixed from the temperature.

	// The bead velocities are selected randomly from a much larger vector 
	// of velocities taken from a Maxwell distribution with unit mean and variance.
	// If the number of values needed exceeds 50000 then the actual number required
	// is used.

	zDoubleVector rvelDist = MaxwellDist(riState.GetBeadTotal());

	zDoubleVector velXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velZDist(riState.GetBeadTotal(), 0.0);

	zDoubleVector posXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector posYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector posZDist(riState.GetBeadTotal(), 0.0);

	long index;
	double vmag, vtheta, vphi;
	double xp[3];
	double vp[3];
	double vcm[3];

	vcm[0] = 0.0;
	vcm[1] = 0.0;
	vcm[2] = 0.0;

	// Assign bead positions, chosen randomly from within the whole
	// simulation box, the un-pbc coordinates and calculate the bead 
	// velocities from a Maxwell distribution and the CM velocity.
	// We ensure that the initial coordinates for all beads lie within
	// the upper edges of the SimBox by at least m_CoordErrorLimit.
	//
	// In 2d and 3d we choose the azimuthal angle, phi, randomly from
	// the range [0,2*pi], but in 3d we also choose the polar angle, theta,
	// in the range [0,pi]. We do this by picking a value for cos(theta)
	// randomly from the uniform distribution [-1,1] and taking the acos of it.
	// Because the return value of acos is positive (i.e., in the range [0,pi/2]
	// we just multiply by two.

	// debug bead positions
//	double offset = 0.5;
//	double beadX = offset;
//	double beadY = offset;
//	double beadZ = offset;

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
		vmag	= rvelDist.at(index);

		xp[0] = riState.GetWallXWidth() + riState.GetSimBoxBulkXLength()*CCNTCell::GetRandomNo();
		xp[1] = riState.GetWallYWidth() + riState.GetSimBoxBulkYLength()*CCNTCell::GetRandomNo();

// The following lines are included unless the variable PreRandomBuilderSimBoxCheck is defined.
// This checks that bead coordinates lie a small distance within the upper bounds of the
// SimBox. But for comparison with the standard 050 test case it needs to be commented out.

#ifndef PreRandomBuilderSimBoxCheck

		if(xp[0] > riState.GetSimBoxXLength())		// Bead outside box
		{
			new CLogBuilderError(0, "Bead "+ ToString(i) +" X coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxXLength() - xp[0] < m_CoordErrorLimit)	// Bead just within box
			xp[0] = 0.0;

		if(xp[1] > riState.GetSimBoxYLength())		// Bead outside box
		{
			new CLogBuilderError(0, "Bead "+ ToString(i) +" Y coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxYLength() - xp[1] < m_CoordErrorLimit)	// Bead just within box
			xp[1] = 0.0;
		
#endif

//		if(i>0)
//			beadX += riState.GetCNTXCellWidth();
//		if(i>0 && i % riState.GetCNTXCellNo() == 0)
//		{
//			beadX = offset;
//			beadY += riState.GetCNTYCellWidth();
//			if(i % (riState.GetCNTXCellNo()*riState.GetCNTYCellNo()) == 0)
//			{
//				beadX = offset;
//				beadY = offset;
//				beadZ += riState.GetCNTZCellWidth();
//			}
//		}
//		xp[0] = beadX;
//		xp[1] = beadY;
//		xp[2] = beadZ;
		
#if SimDimension == 3

		xp[2] = riState.GetWallZWidth() + riState.GetSimBoxBulkZLength()*CCNTCell::GetRandomNo();

#ifndef PreRandomBuilderSimBoxCheck

		if(xp[2] > riState.GetSimBoxZLength())		// Bead outside box
		{
			new CLogBuilderError(0, "Bead "+ ToString(i) +" Z coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxZLength() - xp[2] < m_CoordErrorLimit)	// Bead just within box
			xp[2] = 0.0;	
#endif

		vtheta	= acos(1.0-2.0*CCNTCell::GetRandomNo());
		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

		vp[0]   = vmag*sin(vtheta)*cos(vphi);
		vp[1]   = vmag*sin(vtheta)*sin(vphi);
		vp[2]   = vmag*cos(vtheta);

#elif SimDimension == 2

		xp[2]	= 0.0;

		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

		vp[0]   = vmag*cos(vphi);
		vp[1]   = vmag*sin(vphi);
		vp[2]   = 0.0;

#endif

		vcm[0] += vp[0];
		vcm[1] += vp[1];
		vcm[2] += vp[2];

		velXDist.at(i) = vp[0];
		velYDist.at(i) = vp[1];
		velZDist.at(i) = vp[2];

		posXDist.at(i) = xp[0];
		posYDist.at(i) = xp[1];
		posZDist.at(i) = xp[2];

// Set initial position coordinates. Note that the coordinates are set here for every
// bead and then altered for those that are bonded together in the next section

		(riState.GetBeads()).at(i)->SetXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetZPos(xp[2]);

// Copy initial coordinates into un-PBC coordinates array

		(riState.GetBeads()).at(i)->SetunPBCXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetunPBCYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetunPBCZPos(xp[2]);

// Store the initial position of the beads separately

		(riState.GetBeads()).at(i)->SetInitialXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetInitialYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetInitialZPos(xp[2]);
	}

	
	// Check that pairs of beads that are bonded together are not too far apart that
	// they generate huge bond forces. This arises because their positions are just
	// randomly selected from the simulation box and for boxes of 20 x 20 x 20 or more
	// this can lead to large separations. If they are further apart than "rbond",
	// move them along the line joining their centres so that they are only unit distance
	// apart.

	double rbond = 1.0;
	double rmag;
	double rdiff[3], runit[3];

	for(cPolymerVectorIterator iterPolymer=riState.GetPolymers().begin(); iterPolymer!=riState.GetPolymers().end(); iterPolymer++)
	{
		for(cBondVectorIterator iterBond=(*iterPolymer)->GetBonds().begin(); iterBond!= (*iterPolymer)->GetBonds().end(); iterBond++)
		{
			rdiff[0] = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
			rdiff[1] = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();
			rdiff[2] = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();

			rmag = sqrt(rdiff[0]*rdiff[0] + rdiff[1]*rdiff[1] + rdiff[2]*rdiff[2]);

			runit[0] = rdiff[0]/rmag;
			runit[1] = rdiff[1]/rmag;
			runit[2] = rdiff[2]/rmag;

#ifndef PreRandomBuilderSimBoxCheck
			if( fabs(rdiff[0]) > rbond )
#else
			if( abs(rdiff[0]) > rbond )
#endif
			{
				(*iterBond)->GetTail()->SetXPos(     (*iterBond)->GetHead()->GetXPos()+runit[0]);
				(*iterBond)->GetTail()->SetunPBCXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
				(*iterBond)->GetTail()->SetInitialXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
			}

#ifndef PreRandomBuilderSimBoxCheck
			if( fabs(rdiff[1]) > rbond )
#else
			if( abs(rdiff[1]) > rbond )
#endif
			{
				(*iterBond)->GetTail()->SetYPos(     (*iterBond)->GetHead()->GetYPos()+runit[1]);
				(*iterBond)->GetTail()->SetunPBCYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
				(*iterBond)->GetTail()->SetInitialYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
			}

#ifndef PreRandomBuilderSimBoxCheck
			if( fabs(rdiff[2]) > rbond )
#else
			if( abs(rdiff[2]) > rbond )
#endif
			{
				(*iterBond)->GetTail()->SetZPos(     (*iterBond)->GetHead()->GetZPos()+runit[2]);
				(*iterBond)->GetTail()->SetunPBCZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
				(*iterBond)->GetTail()->SetInitialZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
			}
		}
	}

  

	double vmean[3], v2mean[3], var[3];

	for(i=0; i<3; i++)
	{
		vcm[i]	   /= static_cast<double>(riState.GetBeadTotal());
		vmean[i]	= 0.0;
		v2mean[i]	= 0.0;
		var[i]		= 0.0;
	}

	// remove CM velocity from bead velocities

	double vtotal = 0.0;

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		velXDist.at(i)	-= vcm[0];
		velYDist.at(i)  -= vcm[1];
		velZDist.at(i)  -= vcm[2];

		vtotal = vtotal + velXDist.at(i)*velXDist.at(i) +
						  velYDist.at(i)*velYDist.at(i) +
						  velZDist.at(i)*velZDist.at(i);
	}

	// The following line contained an error in the normalising factor:
	// in 3d it was using 2. The flag PreVelocity2dNormalisation recreates
	// the original (flawed) case for debugging checks.

#ifndef PreVelocity3dNormalisation
	#if SimDimension == 2
		vtotal = sqrt(vtotal/static_cast<double>(2*riState.GetBeadTotal()));
	#elif SimDimension == 3
		vtotal = sqrt(vtotal/static_cast<double>(3*riState.GetBeadTotal()));
	#endif
#else
	vtotal = sqrt(vtotal/static_cast<double>(2*riState.GetBeadTotal()));
#endif

	// finally normalize the velocities to the required temperature, 
	// calculate the mean and variance, and assign the bead velocities

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		velXDist.at(i)	= sqrt(riState.GetkT())*velXDist.at(i)/vtotal;
		velYDist.at(i)	= sqrt(riState.GetkT())*velYDist.at(i)/vtotal;
		velZDist.at(i)  = sqrt(riState.GetkT())*velZDist.at(i)/vtotal;

		(riState.GetBeads()).at(i)->SetXMom(velXDist.at(i));
		(riState.GetBeads()).at(i)->SetYMom(velYDist.at(i));
		(riState.GetBeads()).at(i)->SetZMom(velZDist.at(i));

		vmean[0] += velXDist.at(i);
		vmean[1] += velYDist.at(i);
		vmean[2] += velZDist.at(i);

		v2mean[0] += velXDist.at(i)*velXDist.at(i);
		v2mean[1] += velYDist.at(i)*velYDist.at(i);
		v2mean[2] += velZDist.at(i)*velZDist.at(i);
	}

	for(i=0; i<3; i++)
	{
		vmean[i]  =  vmean[i]/static_cast<double>(riState.GetBeadTotal());
		v2mean[i] = v2mean[i]/static_cast<double>(riState.GetBeadTotal());
		var[i]    = v2mean[i] - vmean[i]*vmean[i];
	}

	// calculate total angular momentum

	double ltotal[3];

	ltotal[0] = 0.0;
	ltotal[1] = 0.0;
	ltotal[2] = 0.0;


	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		ltotal[0] +=  posYDist.at(i)*velZDist.at(i) - posZDist.at(i)*velYDist.at(i);
		ltotal[1] += -posXDist.at(i)*velZDist.at(i) + posZDist.at(i)*velXDist.at(i);
		ltotal[2] +=  posXDist.at(i)*velYDist.at(i) - posYDist.at(i)*velXDist.at(i);
	}

	// Write the initial velocity distribution to the CLogState object using
	// a CLogVelDistMessage object. And only write it to the screen in debug.

	new CLogVelDistMessage(0, riState.GetkT(), vmean, v2mean, var);

#ifdef TraceOn
	Trace("Initial vel distn");
	TraceDouble("Temperature   = ", riState.GetkT());
	TraceVector("Mean Vel      = ", vmean[0],  vmean[1],  vmean[2]);
	TraceVector("Mean Sq Vel   = ", v2mean[0], v2mean[1], v2mean[2]);
	TraceVector("Var Vel       = ", var[0],    var[1],    var[2]);
	TraceVector("Total Ang Mom = ", ltotal[0], ltotal[1], ltotal[2]);
	Trace("********************");
#endif

	return true;
}

// Virtual function to assemble beads into a random initial configuration for
// a parallel simulation. It differs from the above serial function in that 
// we assign the initial bead velocities from a Maxwell distribution defined for
// all of the beads in all of the processors, and normalise them using the 
// CM velocity of the whole processor Space not just each processor's Space 
// independently. But note that the position coordinates of the beads in each
// processor's Space range from 0 to the single-processor Box size.

bool CRandomBuilder::AssembleP(CInitialState& riState)
{
#if EnableParallelSimBox == SimMPSEnabled

    long i;	// loop counter used several times below. Its scope in for() loops
			// seems to be platform-dependent.

	// The CInitialState object has created the beads, bonds and polymers so 
	// we only have to assign their randomised coordinates here.

    // No wall is defined in the parallel initial state yet.

    // Arrange other beads in a random configuration and set their velocities 
	// from a Maxwell distribution in each dimension with a mean value and 
	// variance fixed from the temperature.

	// The bead velocities are selected randomly from a much larger vector 
	// of velocities taken from a Maxwell distribution with unit mean and variance.
	// If the number of values needed exceeds the constant CBuilder::m_MaxwellPointNo
	// then the actual number required is used.

	pmInteger beadTotalMsg1;
	pmInteger beadTotalMsg2;
	
	beadTotalMsg1.SetTag(1);
	beadTotalMsg2.SetTag(2);

	long globalBeadTotal = 0;

	globalBeadTotal = riState.GetBeadTotal();
		
//		std::cout << xxParallelBase::GlobalGetRank() << " has global bead total of " << globalBeadTotal << zEndl;

	// Now all processors have the same value for total beads

	zDoubleVector rvelDist = MaxwellDist(globalBeadTotal);

	zDoubleVector velXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velZDist(riState.GetBeadTotal(), 0.0);

	zDoubleVector posXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector posYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector posZDist(riState.GetBeadTotal(), 0.0);

	long index;
	double vmag, vtheta, vphi;
	double xp[3];
	double vp[3];
	double vcm[3];

	vcm[0] = 0.0;
	vcm[1] = 0.0;
	vcm[2] = 0.0;

	// Assign bead positions, chosen randomly from within the whole
	// simulation box, the un-pbc coordinates and calculate the bead 
	// velocities from a Maxwell distribution and the CM velocity.
	// We ensure that the initial coordinates for all beads lie within
	// the upper edges of the SimBox by at least m_CoordErrorLimit.
	//
	// In 2d and 3d we choose the azimuthal angle, phi, randomly from
	// the range [0,2*pi], but in 3d we also choose the polar angle, theta,
	// in the range [0,pi]. We do this by picking a value for cos(theta)
	// randomly from the uniform distribution [-1,1] and taking the acos of it.
	// Because the return value of acos is positive (i.e., in the range [0,pi/2]
	// we just multiply by two.

	// debug bead positions
//	double offset = 0.5;
//	double beadX = offset;
//	double beadY = offset;
//	double beadZ = offset;

	long ix, iy, iz;
	
	ix = 0;
	iy = 0;
	iz = 0;

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
		vmag	= rvelDist.at(index);

		xp[0] = riState.GetWallXWidth() + riState.GetSimBoxBulkXLength()*CCNTCell::GetRandomNo();
		xp[1] = riState.GetWallYWidth() + riState.GetSimBoxBulkYLength()*CCNTCell::GetRandomNo();

		if(xp[0] > riState.GetSimBoxXLength())		// Bead outside box
		{
			CLogBuilderError* pMsg = new CLogBuilderError(0, "Bead "+ ToString(i) +" X coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxXLength() - xp[0] < m_CoordErrorLimit)	// Bead just within box
			xp[0] = 0.0;

		if(xp[1] > riState.GetSimBoxYLength())		// Bead outside box
		{
			CLogBuilderError* pMsg = new CLogBuilderError(0, "Bead "+ ToString(i) +" Y coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxYLength() - xp[1] < m_CoordErrorLimit)	// Bead just within box
		{
			xp[1] = 0.0;
		}
		
//		if(i>0)
//			beadX += riState.GetCNTXCellWidth();
//		if(i>0 && i % riState.GetCNTXCellNo() == 0)
//		{
//			beadX = offset;
//			beadY += riState.GetCNTYCellWidth();
//			if(i % (riState.GetCNTXCellNo()*riState.GetCNTYCellNo()) == 0)
//			{
//				beadX = offset;
//				beadY = offset;
//				beadZ += riState.GetCNTZCellWidth();
//			}
//		}
//		xp[0] = beadX;
//		xp[1] = beadY;
//		xp[2] = beadZ;
		
#if SimDimension == 3

		xp[2] = riState.GetWallZWidth() + riState.GetSimBoxBulkZLength()*CCNTCell::GetRandomNo();

		if(xp[2] > riState.GetSimBoxZLength())		// Bead outside box
		{
			CLogBuilderError* pMsg = new CLogBuilderError(0, "Bead "+ ToString(i) +" Z coord outside box");
			return ErrorTrace("Error creating random initial state");
		}
		else if(riState.GetSimBoxZLength() - xp[2] < m_CoordErrorLimit)	// Bead just within box
			xp[2] = 0.0;	

		vtheta	= acos(1.0-2.0*CCNTCell::GetRandomNo());
		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

		vp[0]   = vmag*sin(vtheta)*cos(vphi);
		vp[1]   = vmag*sin(vtheta)*sin(vphi);
		vp[2]   = vmag*cos(vtheta);

#elif SimDimension == 2

		xp[2]	= 0.0;

		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

		vp[0]   = vmag*cos(vphi);
		vp[1]   = vmag*sin(vphi);
		vp[2]   = 0.0;

#endif

// Debug assignment of the bead positions to the centres of each CNT cell

/*
        xp[0] = 0.5 + static_cast<double>(ix)*riState.GetCNTXCellWidth();
        xp[1] = 0.5 + static_cast<double>(iy)*riState.GetCNTYCellWidth();
        xp[2] = 0.5 + static_cast<double>(iz)*riState.GetCNTZCellWidth();
		
		ix++;
		if(ix == riState.GetCNTXCellNo())
		{
		    ix = 0;
			iy++;
			
			if(iy == riState.GetCNTYCellNo())
			{
			   ix = 0;
			   iy = 0;
			   iz++;
			}
		}
*/

		vcm[0] += vp[0];
		vcm[1] += vp[1];
		vcm[2] += vp[2];

		velXDist.at(i) = vp[0];
		velYDist.at(i) = vp[1];
		velZDist.at(i) = vp[2];

		posXDist.at(i) = xp[0];
		posYDist.at(i) = xp[1];
		posZDist.at(i) = xp[2];

// Translate coordinates into the owning processor's Space: we do this after 
// storing the local coordinates in the posXDist, etc vectors as they are used
// to calculate the angular momentum in the owning processor's SimBox.

//        xp[0] += riState.GetSimBoxXOrigin();
//        xp[1] += riState.GetSimBoxYOrigin();
//        xp[2] += riState.GetSimBoxZOrigin();

// Set initial position coordinates. Note that the coordinates are set here for every
// bead and then altered for those that are bonded together in the next section

		(riState.GetBeads()).at(i)->SetXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetZPos(xp[2]);

// Copy initial coordinates into un-PBC coordinates array

		(riState.GetBeads()).at(i)->SetunPBCXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetunPBCYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetunPBCZPos(xp[2]);

// Store the initial position of the beads separately

		(riState.GetBeads()).at(i)->SetInitialXPos(xp[0]);
		(riState.GetBeads()).at(i)->SetInitialYPos(xp[1]);
		(riState.GetBeads()).at(i)->SetInitialZPos(xp[2]);
	}

	
	// Check that pairs of beads that are bonded together are not too far apart that
	// they generate huge bond forces. This arises because their positions are just
	// randomly selected from the simulation box and for boxes of 20 x 20 x 20 or more
	// this can lead to large separations. If they are further apart than "rbond",
	// move them along the line joining their centres so that they are only unit distance
	// apart.

	double rbond = 1.0;
	double rmag;
	double rdiff[3], runit[3];

	for(cPolymerVectorIterator iterPolymer=riState.GetPolymers().begin(); iterPolymer!=riState.GetPolymers().end(); iterPolymer++)
	{
		for(cBondVectorIterator iterBond=(*iterPolymer)->GetBonds().begin(); iterBond!= (*iterPolymer)->GetBonds().end(); iterBond++)
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
				(*iterBond)->GetTail()->SetXPos(     (*iterBond)->GetHead()->GetXPos()+runit[0]);
				(*iterBond)->GetTail()->SetunPBCXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
				(*iterBond)->GetTail()->SetInitialXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
			}

			if( fabs(rdiff[1]) > rbond )
			{
				(*iterBond)->GetTail()->SetYPos(     (*iterBond)->GetHead()->GetYPos()+runit[1]);
				(*iterBond)->GetTail()->SetunPBCYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
				(*iterBond)->GetTail()->SetInitialYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
			}

			if( fabs(rdiff[2]) > rbond )
			{
				(*iterBond)->GetTail()->SetZPos(     (*iterBond)->GetHead()->GetZPos()+runit[2]);
				(*iterBond)->GetTail()->SetunPBCZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
				(*iterBond)->GetTail()->SetInitialZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
			}
		}
	}

  

	double vmean[3], v2mean[3], var[3];

	for(i=0; i<3; i++)
	{
		vcm[i]	   /= static_cast<double>(riState.GetBeadTotal());
		vmean[i]	= 0.0;
		v2mean[i]	= 0.0;
		var[i]		= 0.0;
	}
	
//	std::cout << xxParallelBase::GlobalGetRank() << " has local CM velocity " << vcm[0] << " " <<  vcm[1] << " " <<  vcm[2] << " " <<  zEndl;

	// Now we have the local CM velocity, each PN sends its value to P0 so that
	// it can form the global CM velocity. We already have the global bead total
	// from above.

	double globalVCM[3];

		globalVCM[0] = vcm[0];
		globalVCM[1] = vcm[1];
		globalVCM[2] = vcm[2];

	// remove the global CM velocity from each processors' set of bead velocities,
	// accumulate the total velocity on each processor and send it to P0 so that
	// it can normalize it to the defined temperature.

	double vtotal = 0.0;
	double globalVTotal = 0.0;

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		velXDist.at(i)	-= globalVCM[0];
		velYDist.at(i)  -= globalVCM[1];
		velZDist.at(i)  -= globalVCM[2];

		vtotal = vtotal + velXDist.at(i)*velXDist.at(i) +
						  velYDist.at(i)*velYDist.at(i) +
						  velZDist.at(i)*velZDist.at(i);
	}

    globalVTotal = vtotal;
	
	// Normalise to the temperature
	#if SimDimension == 2
		globalVTotal = sqrt(globalVTotal/static_cast<double>(2*globalBeadTotal));
	#elif SimDimension == 3
		globalVTotal = sqrt(globalVTotal/static_cast<double>(3*globalBeadTotal));
	#endif

//	std::cout << xxParallelBase::GlobalGetRank() << " has globalBeadTotal and globalVTotal " << globalBeadTotal << "  " << globalVTotal <<  zEndl;

	// finally normalize the velocities to the required temperature, 
	// calculate the mean and variance, and assign the bead velocities

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		velXDist.at(i)	= sqrt(riState.GetkT())*velXDist.at(i)/globalVTotal;
		velYDist.at(i)	= sqrt(riState.GetkT())*velYDist.at(i)/globalVTotal;
		velZDist.at(i)  = sqrt(riState.GetkT())*velZDist.at(i)/globalVTotal;

//		velXDist.at(i) = 0.0;
//		velYDist.at(i) = 0.0;
//		velZDist.at(i) = 0.0;

		(riState.GetBeads()).at(i)->SetXMom(velXDist.at(i));
		(riState.GetBeads()).at(i)->SetYMom(velYDist.at(i));
		(riState.GetBeads()).at(i)->SetZMom(velZDist.at(i));

		(riState.GetBeads()).at(i)->SetOldXMom(0.0);
		(riState.GetBeads()).at(i)->SetOldYMom(0.0);
		(riState.GetBeads()).at(i)->SetOldZMom(0.0);

		vmean[0] += velXDist.at(i);
		vmean[1] += velYDist.at(i);
		vmean[2] += velZDist.at(i);

		v2mean[0] += velXDist.at(i)*velXDist.at(i);
		v2mean[1] += velYDist.at(i)*velYDist.at(i);
		v2mean[2] += velZDist.at(i)*velZDist.at(i);
	}

	for(i=0; i<3; i++)
	{
		vmean[i]  =  vmean[i]/static_cast<double>(globalBeadTotal);
		v2mean[i] = v2mean[i]/static_cast<double>(globalBeadTotal);
	}

	// All PN pass their mean velocities back to P0 for summation and output

	pmDouble3d  vmeanMsg;
	pmDouble3d  v2meanMsg;

	vmeanMsg.SetTag(7);
	v2meanMsg.SetTag(8);
	

//	std::cout << xxParallelBase::GlobalGetRank() << " has local vmean " << vmean[0] << " " <<  vmean[1] << " " <<  vmean[2] << " " <<  zEndl;

	if(xxParallelBase::GlobalGetRank() == 0)
	{
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			vmeanMsg.Receive();
			v2meanMsg.Receive();
			
			vmean[0] += vmeanMsg.GetXValue();
			vmean[1] += vmeanMsg.GetYValue();
			vmean[2] += vmeanMsg.GetZValue();

			v2mean[0] += v2meanMsg.GetXValue();
			v2mean[1] += v2meanMsg.GetYValue();
			v2mean[2] += v2meanMsg.GetZValue();
					}

		var[0] = v2mean[0] - vmean[0]*vmean[0];
		var[1] = v2mean[1] - vmean[1]*vmean[1];
		var[2] = v2mean[2] - vmean[2]*vmean[2];

		// P0 writes the initial velocity distribution to the CLogState object using
		// a CLogVelDistMessage object.

		CLogVelDistMessage* pMsg = new CLogVelDistMessage(0, riState.GetkT(), vmean, v2mean, var);
	}
	else
	{
		// Send our mean velocity to P0 

		vmeanMsg.SetMessageData(vmean[0], vmean[1], vmean[2]);
		vmeanMsg.SendP0();

		v2meanMsg.SetMessageData(v2mean[0], v2mean[1], v2mean[2]);
		v2meanMsg.SendP0();
	}

#ifdef TraceOn
	// calculate total angular momentum

	double ltotal[3];

	ltotal[0] = 0.0;
	ltotal[1] = 0.0;
	ltotal[2] = 0.0;

	for(i=0; i<riState.GetBeadTotal(); i++)
	{
		ltotal[0] +=  posYDist.at(i)*velZDist.at(i) - posZDist.at(i)*velYDist.at(i);
		ltotal[1] += -posXDist.at(i)*velZDist.at(i) + posZDist.at(i)*velXDist.at(i);
		ltotal[2] +=  posXDist.at(i)*velYDist.at(i) - posYDist.at(i)*velXDist.at(i);
	}

	Trace("Initial vel distn");
	TraceDouble("Temperature   = ", riState.GetkT());
	TraceVector("Mean Vel      = ", vmean[0],  vmean[1],  vmean[2]);
	TraceVector("Mean Sq Vel   = ", v2mean[0], v2mean[1], v2mean[2]);
	TraceVector("Var Vel       = ", var[0],    var[1],    var[2]);
	TraceVector("Total Ang Mom = ", ltotal[0], ltotal[1], ltotal[2]);
	Trace("********************");
#endif

#endif
	return true;
}
