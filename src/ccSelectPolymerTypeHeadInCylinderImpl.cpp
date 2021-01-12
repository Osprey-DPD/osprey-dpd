/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInCylinderImpl.cpp: implementation of the ccSelectPolymerTypeHeadInCylinderImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeHeadInCylinderImpl.h"
#include "ccSelectPolymerTypeHeadInCylinder.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInCylinderImpl::ccSelectPolymerTypeHeadInCylinderImpl()
{

}

ccSelectPolymerTypeHeadInCylinderImpl::~ccSelectPolymerTypeHeadInCylinderImpl()
{

}

// Function to implement a ccSelectPolymerTypeInRegion-derived command that 
// collects polymers whose head beads are of the (single) specified type with
// coordinates within a specified cylindrical region of the SimBox. 
//
// Note that if the polymer's head bead type occurs in other polymers they 
// may also be selected if they lie within the specified cylindrical region.
// The head bead type may occur elsewhere in the polymer's shape string,
// because we first collect all polymers of the specified type and then 
// search for their uniquely-defined head beads. This is in contrast to 
// the simpler method of searching for all beads with the same type as the
// polymer's head bead.
//
// The cylinder centre, length and inner and outer radii are specified 
// as fractions of the SimBox dimensions.
//
// The polymers are packaged into a CCommandTargetPolymer aggregate that can 
// receive subsequent ctApplyCommand commands.

void ccSelectPolymerTypeHeadInCylinderImpl::SelectPolymerTypeHeadInCylinder(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

    const ccSelectPolymerTypeHeadInCylinder* const pCmd = dynamic_cast<const ccSelectPolymerTypeHeadInCylinder*>(pCommand);

	// Data needed to define an aggregate

	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString polyName		= pCmd->GetPolymerName();	// Name of polymer type

	const long xNormal			= pCmd->GetXNormal();
	const long yNormal			= pCmd->GetYNormal();		//	Cylinder normal
//	const long zNormal			= pCmd->GetZNormal();
	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Cylinder centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double halfLength		= pCmd->GetHalfLength();	// Half cylinder length
	const double innerRadius	= pCmd->GetInnerRadius();	// Inner radius of cylinder
	const double outerRadius	= pCmd->GetOuterRadius();	// Inner radius of cylinder

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the specified polymers and their head bead type

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);
//	const long beadType = pSimBox->GetPolymerHeadType(polyType);

	// Store the inner and outer radii squared to save time when 
	// comparing bead distances from the centre

	const double rMinSq = innerRadius*innerRadius;
	const double rMaxSq = outerRadius*outerRadius;

	// Iterate over slices through the SimBox with the normal defined by
	// the cylinder, and use their ability to select the beads contained 
	// within the slice. We use the fact that the centre and length of the
	// cylinder are specified as fractions of the SimBox side lengths to
	// get the lower and upper slice indices corresponding to the 
	// cylinder length. To be sure we get all the beads, we increase the
	// indices by one checking that this does not cause them to exceed
	// the SimBox size.
	//
	// The distance of the bead's centre from the 
	// cylinder axis must lie within the inner and outer radii.

	long lowerIndex = 0;
	long upperIndex = 0;

	double xAxis	= 0.0;
	double yAxis	= 0.0;
	double zAxis	= 0.0;
	double xMin		= 0.0;
	double yMin		= 0.0;
	double zMin		= 0.0;
	double xMax		= 0.0;
	double yMax		= 0.0;
	double zMax		= 0.0;

	if(xNormal == 1)
	{
		lowerIndex = static_cast<long>((xCentre - halfLength)*pSimBox->GetSimBoxXLength());
		upperIndex = static_cast<long>((xCentre + halfLength)*pSimBox->GetSimBoxXLength());

		if(upperIndex == pSimBox->GetCNTXCellNo())
			upperIndex--;
		else if(upperIndex < (pSimBox->GetCNTXCellNo()-1))
			upperIndex++;

		yAxis = yCentre*pSimBox->GetSimBoxYLength();
		zAxis = zCentre*pSimBox->GetSimBoxZLength();

		xMin = (xCentre - halfLength)*pSimBox->GetSimBoxXLength();
		xMax = (xCentre + halfLength)*pSimBox->GetSimBoxXLength();
	}
	else if(yNormal == 1)
	{
		lowerIndex = static_cast<long>((yCentre - halfLength)*pSimBox->GetSimBoxYLength());
		upperIndex = static_cast<long>((yCentre + halfLength)*pSimBox->GetSimBoxYLength());

		if(upperIndex == pSimBox->GetCNTYCellNo())
			upperIndex--;
		else if(upperIndex < (pSimBox->GetCNTYCellNo()-1))
			upperIndex++;

		zAxis = zCentre*pSimBox->GetSimBoxZLength();
		xAxis = xCentre*pSimBox->GetSimBoxXLength();

		yMin = (yCentre - halfLength)*pSimBox->GetSimBoxYLength();
		yMax = (yCentre + halfLength)*pSimBox->GetSimBoxYLength();
	}
	else
	{
		lowerIndex = static_cast<long>((zCentre - halfLength)*pSimBox->GetSimBoxZLength());
		upperIndex = static_cast<long>((zCentre + halfLength)*pSimBox->GetSimBoxZLength());

		if(upperIndex == pSimBox->GetCNTZCellNo())
			upperIndex--;
		else if(upperIndex < (pSimBox->GetCNTZCellNo()-1))
			upperIndex++;

		xAxis = xCentre*pSimBox->GetSimBoxXLength();
		yAxis = yCentre*pSimBox->GetSimBoxYLength();

		zMin = (zCentre - halfLength)*pSimBox->GetSimBoxZLength();
		zMax = (zCentre + halfLength)*pSimBox->GetSimBoxZLength();
	}

	if(lowerIndex > 0)		// common to all normals
		lowerIndex--;

	double xBead	= 0.0;
	double yBead	= 0.0;
	double zBead	= 0.0;
	double rDistSq	= 0.0;	// distance of bead from cylinder axis

	// Collect all polymers of the specified type, and their head beads

	PolymerVectorIterator iterPoly;

	PolymerVector vSourcePolymers;
	vSourcePolymers.clear();

	PolymerVector vAllPolymers = pSimBox->GetAllPolymers();

	for(iterPoly=vAllPolymers.begin(); iterPoly!=vAllPolymers.end(); iterPoly++)
	{
		if((*iterPoly)->GetType() == polyType)
			vSourcePolymers.push_back(*iterPoly);
	}


	// The GetBeads() function does not seem to work when used inside the 
	// for-loop statements, so I copy it to a local variable before looping.

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
			xBead = pBead->GetXPos();
			yBead = pBead->GetYPos();
			zBead = pBead->GetZPos();

			if(xNormal == 1)
			{
				if(xBead > xMin && xBead < xMax)
				{
					rDistSq = (yBead - yAxis)*(yBead - yAxis) + 
							  (zBead - zAxis)*(zBead - zAxis);

					if(rDistSq > rMinSq && rDistSq < rMaxSq)
					{
						vTargetPolymers.push_back(pPolymer);
						vTargetBeads.push_back(pBead);
					}
				}
			}
			else if(yNormal == 1)
			{
				if(yBead > yMin && yBead < yMax)
				{
					rDistSq = (zBead - zAxis)*(zBead - zAxis) + 
							  (xBead - xAxis)*(xBead - xAxis);

					if(rDistSq > rMinSq && rDistSq < rMaxSq)
					{
						vTargetPolymers.push_back(pPolymer);
						vTargetBeads.push_back(pBead);
					}
				}
			}
			else 
			{
				if(zBead > zMin && zBead < zMax)
				{
					rDistSq = (xBead - xAxis)*(xBead - xAxis) + 
							  (yBead - yAxis)*(yBead - yAxis);

					if(rDistSq > rMinSq && rDistSq < rMaxSq)
					{
						vTargetPolymers.push_back(pPolymer);
						vTargetBeads.push_back(pBead);
					}
				}
			}
		}
	}

	const long polyTotal = vTargetPolymers.size();

//	cout << "Total polymers = " << vSourcePolymers.size() << zEndl;
//	cout << "Target polymers = " << polyTotal << zEndl;

	// Create a CCommandTargetPolymer to hold the polymers so that commands 
	// may be targetted to them later. We pass in the polymer type in case the 
	// vTargetPolymers container is empty as we still want to know what type 
	// of polymer was specified.

	if(pSimBox->CreateCommandTargetPolymer(label, polyType, vTargetPolymers))
	{
		 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "Cylinder", polyName, polyType, polyTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}



