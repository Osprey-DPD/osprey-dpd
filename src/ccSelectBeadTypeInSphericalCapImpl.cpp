/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInSphericalCapImpl.cpp: implementation of the ccSelectBeadTypeInSphericalCapImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectBeadTypeInSphericalCapImpl.h"
#include "ccSelectBeadTypeInSphericalCap.h"
#include "SimBox.h"
#include "Bead.h"
#include "CommandTarget.h"
#include "LogSelectBeadTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInSphericalCapImpl::ccSelectBeadTypeInSphericalCapImpl()
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

ccSelectBeadTypeInSphericalCapImpl::~ccSelectBeadTypeInSphericalCapImpl()
{

}

// Command handler to create a command target out of all beads of a given type
// whose centres of mass lie within a given spherical cap of a sphere in the SimBox.
//

void ccSelectBeadTypeInSphericalCapImpl::SelectBeadTypeInSphericalCap(const xxCommand* const pCommand)
{
	const ccSelectBeadTypeInSphericalCap* const pCmd = dynamic_cast<const ccSelectBeadTypeInSphericalCap*>(pCommand);

	const zString label			= pCmd->GetTargetLabel();	  // Label of command target
	const zString beadName		= pCmd->GetBeadName();		  // Name of bead type

	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		  // Sphere's centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double innerRadius	= pCmd->GetInnerRadius();	  // Inner radius of shell
	const double outerRadius	= pCmd->GetOuterRadius();	  // Outer radius of shell
	const double theta	        = pCmd->GetPolarAngle();	  // Polar angle of cap
	const double phi	        = pCmd->GetAzimuthalAngle();  // Azimuthal angle of cap
	const double gamma	        = pCmd->GetHalfAngle();	      // Half angle of cap

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the selected beads from their name

	const long beadType = pSimBox->GetBeadTypeFromName(beadName);

	const double xc		  = xCentre*pSimBox->GetSimBoxXLength();
	const double yc		  = yCentre*pSimBox->GetSimBoxYLength();
	const double zc		  = zCentre*pSimBox->GetSimBoxZLength();
	const double rinsq    = innerRadius*innerRadius;
	const double routsq   = outerRadius*outerRadius;
	const double cosgamma = cos(gamma);
	const double xnorm    = sin(theta)*cos(phi);
	const double ynorm    = sin(theta)*sin(phi);
	const double znorm    = cos(theta);

	// Loop over all the beads in the SimBox and store pointers to those whose
	// type matches the specified type, and whose coordinates lie within the 
	// specified spherical capregion. Note that this may be a sphere or a shell.
	// We use the square of distances to avoid having to take square roots.
	
	// We check that the vector passing through the particles CM lies within an angle gamma of the normal vector to the cap.
	// This is just the condition that their product should define an angle whose cosine is greater than that of cos(gamma).
	// We split the test into two pieces to avoid having to calculate the square root for all beads that are the wrong type anyway, 
	// or outside the spherical region.

	BeadVector vTargetBeads;

	vTargetBeads.clear();

    AbstractBeadVector vAllBeads = pSimBox->GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		const double dx = (*iterBead)->GetXPos() - xc;
		const double dy = (*iterBead)->GetYPos() - yc;
		const double dz = (*iterBead)->GetZPos() - zc;

		const double rsq = dx*dx + dy*dy + dz*dz;
		
		if((*iterBead)->GetType() == beadType && rinsq < rsq && rsq < routsq)
        {		
		    // Calculate the dot product of the particle's normal vector and that of the spherical cap
		
		    double cosalpha = 0.0;
            cosalpha = (xnorm*dx + ynorm*dy + znorm*dz)/sqrt(rsq);
		
//		    std::cout << dx/rsq << " " << dy/rsq << " " << dz/rsq  << " " << cosalpha << " " << cosgamma << zEndl;

		    if(cosalpha > cosgamma)
		    {
			    vTargetBeads.push_back(dynamic_cast<CBead*>(*iterBead));
		    }
		}
	}

	const long beadTotal = vTargetBeads.size();

		// Create and store a CCommandTarget to hold the beads and receive commands.
		// We pass in the bead type in case the targetBeads container is empty as
		// we still want to know what type of bead was specified.

		if(pSimBox->CreateCommandTarget(label, beadType, vTargetBeads))
		{
			 new CLogSelectBeadTypeInRegion(pSimBox->GetCurrentTime(), label, "SphericalCap", beadName, beadType, beadTotal);
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

void ccSelectBeadTypeInSphericalCapImpl::CalculateRotationMatrix(double theta, double phi)
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


