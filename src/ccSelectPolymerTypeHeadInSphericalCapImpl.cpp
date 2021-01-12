/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInSphericalCapImpl.cpp: implementation of the ccSelectPolymerTypeHeadInSphericalCapImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeHeadInSphericalCapImpl.h"
#include "ccSelectPolymerTypeHeadInSphericalCap.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInSphericalCapImpl::ccSelectPolymerTypeHeadInSphericalCapImpl()
{
}

ccSelectPolymerTypeHeadInSphericalCapImpl::~ccSelectPolymerTypeHeadInSphericalCapImpl()
{
}

// Command handler to create a command target out of all polymers of a given type
// whose head beads lie within a given planar slice region of the SimBox.
//

void ccSelectPolymerTypeHeadInSphericalCapImpl::SelectPolymerTypeHeadInSphericalCap(const xxCommand* const pCommand)
{
	const ccSelectPolymerTypeHeadInSphericalCap* const pCmd = dynamic_cast<const ccSelectPolymerTypeHeadInSphericalCap*>(pCommand);

	// Data needed to define an aggregate

	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString polyName		= pCmd->GetPolymerName();	// Name of polymer type

	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Sphere centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double innerRadius	= pCmd->GetInnerRadius();	// Inner radius of sphere (bead diameters)
	const double outerRadius	= pCmd->GetOuterRadius();	// Inner radius of sphere (bead diameters)
	const double theta	        = pCmd->GetPolarAngle();	  // Polar angle of cap
	const double phi	        = pCmd->GetAzimuthalAngle();  // Azimuthal angle of cap
	const double gamma	        = pCmd->GetHalfAngle();	      // Half angle of cap

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the selected polymers and their head bead type,
	// then store all polymers of the specified type.

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);
//	const long beadType = pSimBox->GetPolymerHeadType(polyType);

	const double xc = xCentre*pSimBox->GetSimBoxXLength();
	const double yc = yCentre*pSimBox->GetSimBoxYLength();
	const double zc = zCentre*pSimBox->GetSimBoxZLength();

	// Store the inner and outer radii squared to save time when 
	// comparing bead distances from the centre: to be added to the 
	// target, the distance between a bead's centre and the sphere's centre 
	// must lie within the inner and outer radii.

	const double rinsq    = innerRadius*innerRadius;
	const double routsq   = outerRadius*outerRadius;
	const double cosgamma = cos(gamma);
	const double xnorm    = sin(theta)*cos(phi);
	const double ynorm    = sin(theta)*sin(phi);
	const double znorm    = cos(theta);

	// Collect the polymers whose type matches that specified and whose head
	// beads lie within the spherical region specified

	PolymerVectorIterator iterPoly;
	
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

			if(rsq > rinsq && rsq < routsq)
			{
		        // Calculate the dot product of the particle's normal vector and that of the spherical cap
		
		        double cosalpha = 0.0;
                cosalpha = (xnorm*dx + ynorm*dy + znorm*dz)/sqrt(rsq);
		
//		        std::cout << cosalpha << " " << cosgamma << zEndl;

		        if(cosalpha > cosgamma)
		        {
				    vTargetPolymers.push_back(pPolymer);
				    vTargetBeads.push_back(pBead);
		        }
			}
		}
	}

	const long polyTotal = vTargetPolymers.size();

//	std::cout << "Total polymers = " << vSourcePolymers.size() << zEndl;
//	std::cout << "Target (sphere) polymers = " << polyTotal << zEndl;
//	std::cout << "Target (sphere) head beads = " << vTargetBeads.size() << zEndl;

	// Create a CCommandTargetPolymer to hold the polymers so that commands 
	// may be targetted to them later. We pass in the polymer type in case the 
	// vTargetPolymers container is empty as we still want to know what type 
	// of polymer was specified.

	if(pSimBox->CreateCommandTargetPolymer(label, polyType, vTargetPolymers))
	{
		 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "SphericalCap", polyName, polyType, polyTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}

