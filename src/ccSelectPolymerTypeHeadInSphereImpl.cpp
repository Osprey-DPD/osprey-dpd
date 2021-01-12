/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInSphereImpl.cpp: implementation of the ccSelectPolymerTypeHeadInSphereImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeHeadInSphereImpl.h"
#include "ccSelectPolymerTypeHeadInSphere.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInSphereImpl::ccSelectPolymerTypeHeadInSphereImpl()
{
}

ccSelectPolymerTypeHeadInSphereImpl::~ccSelectPolymerTypeHeadInSphereImpl()
{

}

// Function to implement a ccSelectPolymerTypeInRegion-derived command that 
// collects polymers whose head beads are of the (single) specified type with
// coordinates within a specified spherical (or sphereical-shell) region of 
// the SimBox. 
//
// Note that if the polymer's head bead type occurs in other polymers they 
// may also be selected if they lie within the specified region.
// The head bead type may occur elsewhere in the polymer's shape string,
// because we first collect all polymers of the specified type and then 
// search for their uniquely-defined head beads. This is in contrast to 
// the simpler method of searching for all beads with the same type as the
// polymer's head bead.
//
// The sphere's centre and inner and outer radii are specified 
// as fractions of the SimBox dimensions.
//
// The polymers are packaged into a CCommandTargetPolymer aggregate that can 
// receive subsequent ctApplyCommand commands.

void ccSelectPolymerTypeHeadInSphereImpl::SelectPolymerTypeHeadInSphere(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ccSelectPolymerTypeHeadInSphere* const pCmd = dynamic_cast<const ccSelectPolymerTypeHeadInSphere*>(pCommand);

	// Data needed to define an aggregate

	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString polyName		= pCmd->GetPolymerName();	// Name of polymer type

	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Sphere centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double innerRadius	= pCmd->GetInnerRadius();	// Inner radius of sphere (bead diameters)
	const double outerRadius	= pCmd->GetOuterRadius();	// Inner radius of sphere (bead diameters)

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the specified polymers and their head bead type

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);
//	const long beadType = pSimBox->GetPolymerHeadType(polyType);

	const double xSphereCentre = xCentre*pSimBox->GetSimBoxXLength();
	const double ySphereCentre = yCentre*pSimBox->GetSimBoxYLength();
	const double zSphereCentre = zCentre*pSimBox->GetSimBoxZLength();

	// Store the inner and outer radii squared to save time when 
	// comparing bead distances from the centre: to be added to the 
	// target, the distance between a bead's centre and the sphere's centre 
	// must lie within the inner and outer radii.

	const double rMinSq	= innerRadius*innerRadius;
	const double rMaxSq	= outerRadius*outerRadius;

	// Collect the polymers whose type matches that specified and whose head
	// beads lie within the spherical region specified

	PolymerVectorIterator iterPoly;

	PolymerVector vSourcePolymers;
	vSourcePolymers.clear();
	
	PolymerVector vAllPolymers = pSimBox->GetAllPolymers();

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
			double xBead = pBead->GetXPos();
			double yBead = pBead->GetYPos();
			double zBead = pBead->GetZPos();

			double rDistSq =  (xBead - xSphereCentre)*(xBead - xSphereCentre) + 
							  (yBead - ySphereCentre)*(yBead - ySphereCentre) +
							  (zBead - zSphereCentre)*(zBead - zSphereCentre);

			if(rDistSq > rMinSq && rDistSq < rMaxSq)
			{
				vTargetPolymers.push_back(pPolymer);
				vTargetBeads.push_back(pBead);
			}
		}
	}

	const long polyTotal = vTargetPolymers.size();

//	cout << "Total polymers = " << vSourcePolymers.size() << zEndl;
//	cout << "Target (sphere) polymers = " << polyTotal << zEndl;
//	cout << "Target (sphere) head beads = " << vTargetBeads.size() << zEndl;

	// Create a CCommandTargetPolymer to hold the polymers so that commands 
	// may be targetted to them later. We pass in the polymer type in case the 
	// vTargetPolymers container is empty as we still want to know what type 
	// of polymer was specified.

	if(pSimBox->CreateCommandTargetPolymer(label, polyType, vTargetPolymers))
	{
		 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "Sphere", polyName, polyType, polyTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}


