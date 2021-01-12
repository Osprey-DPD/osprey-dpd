/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInEllipsoidImpl.cpp: implementation of the ccSelectPolymerTypeHeadInEllipsoidImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeHeadInEllipsoidImpl.h"
#include "ccSelectPolymerTypeHeadInEllipsoid.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInEllipsoidImpl::ccSelectPolymerTypeHeadInEllipsoidImpl()
{
	// Zero the rotation matrix 
	
	for(short int i=0; i<3; i++)
	{
		for(short int j=0; j<3; j++)
		{
			m_RNormal[i][j] = 0.0;
		}
	}

}

ccSelectPolymerTypeHeadInEllipsoidImpl::~ccSelectPolymerTypeHeadInEllipsoidImpl()
{

}

// Command handler to create a command target out of all polymers of a given type
// whose head beads lie within a given ellipsoidal region of the SimBox.
//
// We first select all polymers whose head beads lie within the ellipsoid's 
// bounding sphere to speed up the selection, and then refine the set to contain 
// only head beads within the ellipsoid. Note that the ellipsoid's semi-major axis 
// lies along its Z direction, the first semi-minor axis along its X direction 
// and the second along its Y direction. Also, the semi-major axis must be greater 
// or equal to the first semi-minor axis which must itself be greater or equal to 
// the second semi-minor axis. Finally, note that the ellipsoid's bounding sphere 
// is not allowed to overlap the SimBox edges: this is more restrictive than 
// checking if the ellipsoid overlaps the edges but much simpler.

void ccSelectPolymerTypeHeadInEllipsoidImpl::SelectPolymerTypeHeadInEllipsoid(const xxCommand* const pCommand)
{
	const ccSelectPolymerTypeHeadInEllipsoid* const pCmd = dynamic_cast<const ccSelectPolymerTypeHeadInEllipsoid*>(pCommand);

	const zString label			= pCmd->GetTargetLabel();	 // Label of command target
	const zString polyName		= pCmd->GetPolymerName();	 // Name of polymer type

	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		 //	Ellipsoid's centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double boundingRadius	= pCmd->GetBoundingRadius();       // Radius of bounding sphere
	const double sma	        = pCmd->GetSemiMajorAxis();        // Semi-major axis of ellipsoid
	const double smb	        = pCmd->GetFirstSemiMinorAxis();   // Semi-minor axis of ellipsoid
	const double smc	        = pCmd->GetSecondSemiMinorAxis();  // Semi-minor axis of ellipsoid
	const double theta	        = pCmd->GetPolarAngle();           // Polar angle of ellipsoid's  (radians)
	const double phi	        = pCmd->GetAzimuthalAngle();       // Azimuthal angle of ellipsoid's SMA (radians)

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	const double xc = xCentre*pSimBox->GetSimBoxXLength();
	const double yc	= yCentre*pSimBox->GetSimBoxYLength();
	const double zc	= zCentre*pSimBox->GetSimBoxZLength();
	const double boundsq = boundingRadius*boundingRadius;

	// Find the integer type of the selected polymers and their head bead type

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);
	const long beadType = pSimBox->GetPolymerHeadType(polyType);

	// If any of the ellipsoid's dimensions are zero, or its bounding sphere overlaps
	// the SimBox edges, the command fails

	if(boundingRadius >= sma && sma >= smb && smb >= smc && smc > 0.0 &&
		xc + boundingRadius <= pSimBox->GetSimBoxXLength() &&
		yc + boundingRadius <= pSimBox->GetSimBoxYLength() &&
		zc + boundingRadius <= pSimBox->GetSimBoxZLength() )
	{
		// Loop over all the polymers in the SimBox and store pointers to those 
		// whose type matches that specified, and whose coordinates lie within the 
		// bounding spherical region. Note that we do not allow the region to overlap
		// the SimBox boundaries. We use the square of distances to avoid having to 
		// take square roots. If a polymer's head bead lies within the bounding 
		// sphere, we then check to see if also lies within the ellipsoid.

		// First calculate the rotation matrix that takes each point into the 
		// coordinate frame in which the ellipdoid's semi-major axis lies 
		// along the Z direction

		CalculateRotationMatrix(theta, phi);

		PolymerVectorIterator iterPoly;	// Iterator used several times below

		PolymerVector vAllPolymers = pSimBox->GetISimBox()->GetPolymers();

		PolymerVector vSourcePolymers;
		vSourcePolymers.clear();

		for(iterPoly=vAllPolymers.begin(); iterPoly!=vAllPolymers.end(); iterPoly++)
		{
			if((*iterPoly)->GetType() == polyType)
				vSourcePolymers.push_back(*iterPoly);
		}

		PolymerVector vTargetPolymers;
		vTargetPolymers.clear();

		BeadVector vTargetBeads;
		vTargetBeads.clear();

		for(iterPoly=vSourcePolymers.begin(); iterPoly!=vSourcePolymers.end(); iterPoly++)
		{
			CPolymer* const pPolymer = *iterPoly;
			CBead* const pBead = dynamic_cast<CBead*>(pPolymer->GetHead());
			
			if(pBead)
			{
				const double dx = pBead->GetXPos() - xc;
				const double dy = pBead->GetYPos() - yc;
				const double dz = pBead->GetZPos() - zc;

				const double rsq = dx*dx + dy*dy + dz*dz;

				if(pBead->GetType() == beadType && rsq < boundsq)
				{
					// Find the coordinates of the point in the ellipsoid's frame

					const double dx1 = m_RNormal[0][0]*dx + m_RNormal[0][1]*dy + m_RNormal[0][2]*dz;
					const double dy1 = m_RNormal[1][0]*dx + m_RNormal[1][1]*dy + m_RNormal[1][2]*dz;
					const double dz1 = m_RNormal[2][0]*dx + m_RNormal[2][1]*dy + m_RNormal[2][2]*dz;

					// Check if the point lies within the volume of the ellipsoid

					const double dx1norm = (dx1/sma);  // Z direction
					const double dx2norm = (dy1/smb);  // X direction
					const double dx3norm = (dz1/smc);  // Y direction

					const double rsq2 = dx1norm*dx1norm + dx2norm*dx2norm + dx3norm*dx3norm;

					if(rsq2 <= 1.0)
					{
						vTargetPolymers.push_back(pPolymer);
						vTargetBeads.push_back(pBead);
					}
				}
			}
		}

		const long polyTotal = vTargetPolymers.size();
//		const long beadTotal = vTargetBeads.size();

		// Create and store a CCommandTargetPolymer that can receive commands.
		// We pass in the polymer type in case the vTargetPolymers container 
	    // is empty as we still want to know what type of polymer was specified.

		if(pSimBox->CreateCommandTargetPolymer(label, polyType, vTargetPolymers))
		{
			 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "Ellipsoid", polyName, polyType, polyTotal);
		}
		else
		{
			 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
		}
	}
	else
	{
			 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}

// Private helper function to calculate the rotation matrix that takes a point
// in the original coordinate frame to its equivalent point in the frame
// with the ellipsoid's semi-major axis along the Z direction.
// Note that the angles must be in radians.

void ccSelectPolymerTypeHeadInEllipsoidImpl::CalculateRotationMatrix(double theta, double phi)
{
	m_RNormal[0][0] = cos(theta)*cos(phi);
	m_RNormal[0][1] = cos(theta)*sin(phi);
	m_RNormal[0][2] = -sin(theta);

	m_RNormal[1][0] = -sin(phi);
	m_RNormal[1][1] = cos(phi);
	m_RNormal[1][2] = 0.0;

	m_RNormal[2][0] = sin(theta)*cos(phi);
	m_RNormal[2][1] = sin(theta)*sin(phi);
	m_RNormal[2][2] = cos(theta);
}


