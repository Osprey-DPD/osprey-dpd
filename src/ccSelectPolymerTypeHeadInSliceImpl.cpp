/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInSliceImpl.cpp: implementation of the ccSelectPolymerTypeHeadInSliceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeHeadInSliceImpl.h"
#include "ccSelectPolymerTypeHeadInSlice.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInSliceImpl::ccSelectPolymerTypeHeadInSliceImpl()
{
}

ccSelectPolymerTypeHeadInSliceImpl::~ccSelectPolymerTypeHeadInSliceImpl()
{
}

// Command handler to create a command target out of all polymers of a given type
// whose head beads lie within a given planar slice region of the SimBox.
//

void ccSelectPolymerTypeHeadInSliceImpl::SelectPolymerTypeHeadInSlice(const xxCommand* const pCommand)
{
	const ccSelectPolymerTypeHeadInSlice* const pCmd = dynamic_cast<const ccSelectPolymerTypeHeadInSlice*>(pCommand);

	const zString label		= pCmd->GetTargetLabel();	 // Label of command target
	const zString polyName		= pCmd->GetPolymerName();	 // Name of polymer type

//	const long xNormal		= pCmd->GetXNormal();
//	const long yNormal		= pCmd->GetYNormal();		//	Slice normal
//	const long zNormal		= pCmd->GetZNormal();
	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Slice centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double xHalfWidth 	= pCmd->GetXHalfWidth();
	const double yHalfWidth	= pCmd->GetYHalfWidth();	// Slice half widths (fraction)	
	const double zHalfWidth	= pCmd->GetZHalfWidth();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the selected polymers and their head bead type,
	// then store all polymers of the specified type.

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);
//	const long beadType = pSimBox->GetPolymerHeadType(polyType);

	// Store the minimum and maximum coordinates of the planar region

	double xMin = (xCentre - xHalfWidth)*pSimBox->GetSimBoxXLength();
	double xMax = (xCentre + xHalfWidth)*pSimBox->GetSimBoxXLength();
	double yMin = (yCentre - yHalfWidth)*pSimBox->GetSimBoxYLength();
	double yMax = (yCentre + yHalfWidth)*pSimBox->GetSimBoxYLength();
	double zMin = (zCentre - zHalfWidth)*pSimBox->GetSimBoxZLength();
	double zMax = (zCentre + zHalfWidth)*pSimBox->GetSimBoxZLength();


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

	// Iterate over all polymers of the specified type and collect those whose
	// head beads lie within the specified planar region.

	for(iterPoly=vSourcePolymers.begin(); iterPoly!=vSourcePolymers.end(); iterPoly++)
	{
		CPolymer* const pPolymer = *iterPoly;
		CBead* const pBead = dynamic_cast<CBead*>(pPolymer->GetHead());
		
		if(pBead)
		{
			double xBead = pBead->GetXPos();
			double yBead = pBead->GetYPos();
			double zBead = pBead->GetZPos();

			if(xBead > xMin && xBead < xMax &&
			   yBead > yMin && yBead < yMax &&
			   zBead > zMin && zBead < zMax)
			{
				vTargetPolymers.push_back(pPolymer);
				vTargetBeads.push_back(pBead);
			}
		}
	}

	const long polyTotal = vTargetPolymers.size();
//	const long beadTotal = vTargetBeads.size();

	// Create and store a CCommandTargetPolymer that can receive commands.
	// We pass in the polymer type in case the vTargetPolymers container 
	// is empty as we still want to know what type of polymer was specified.

	if(pSimBox->CreateCommandTargetPolymer(label, polyType, vTargetPolymers))
	{
		 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "Slice", polyName, polyType, polyTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}

