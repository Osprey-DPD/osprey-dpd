/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInCylinderImpl.cpp: implementation of the ccSelectBeadTypeInCylinderImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectBeadTypeInCylinderImpl.h"
#include "ccSelectBeadTypeInCylinder.h"
#include "SimBox.h"
#include "Bead.h"
#include "Slice.h"
#include "CommandTarget.h"
#include "LogSelectBeadTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInCylinderImpl::ccSelectBeadTypeInCylinderImpl()
{

}

ccSelectBeadTypeInCylinderImpl::~ccSelectBeadTypeInCylinderImpl()
{

}

// Function to implement a ccSelectBeadTypeInRegion command that collects 
// beads of the (single) specified type whose coordinates are within a 
// given cylinderical region of the SimBox. 
//
// The cylinder centre, length and inner and outer radii are specified 
// as fractions of the SimBox dimensions.
//
// The beads are packaged into a CCommandTarget aggregate that can 
// receive subsequent ctApplyCommand commands.
//
// We assume that the data has been validated prior to this routine.

void ccSelectBeadTypeInCylinderImpl::SelectBeadTypeInCylinder(const xxCommand* const pCommand)
{    
#if EnableTargetCommand == SimCommandEnabled
    
	const ccSelectBeadTypeInCylinder* const pCmd = dynamic_cast<const ccSelectBeadTypeInCylinder*>(pCommand);
    
	// Data needed to define an aggregate
    
	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString beadName		= pCmd->GetBeadName();		// Name of bead type
    
	const long xNormal			= pCmd->GetXNormal();
	const long yNormal			= pCmd->GetYNormal();		//	Cylinder normal
	const long zNormal			= pCmd->GetZNormal();
	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Cylinder centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double halfLength		= pCmd->GetHalfLength();	// Half cylinder length
	const double innerRadius	= pCmd->GetInnerRadius();	// Inner radius of cylinder
	const double outerRadius	= pCmd->GetOuterRadius();	// Inner radius of cylinder
    
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
    
	// Find the integer type of the selected beads from their name
    
	const long beadType = pSimBox->GetBeadTypeFromName(beadName);
    
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
    
	// The GetBeads() function does not seem to work when used inside the 
	// for-loop statements, so I copy it to a local variable before looping.
    
	BeadVector vTargetBeads;
    
	vTargetBeads.clear();
    
	for(long sliceIndex=lowerIndex; sliceIndex<=upperIndex; sliceIndex++)
	{
		CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, pSimBox->GetISimBox());
        
		BeadList sliceBeads = targetSlice.GetBeads();
        
		for(cBeadListIterator iterBead=sliceBeads.begin(); iterBead!=sliceBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == beadType)
			{
				CBead* const pBead = dynamic_cast<CBead*>(*iterBead);
                
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
								vTargetBeads.push_back(pBead);
						}
					}
					else if(yNormal == 1)
					{
						if(yBead > yMin && yBead < yMax)
						{
							rDistSq = (zBead - zAxis)*(zBead - zAxis) + 
                            (xBead - xAxis)*(xBead - xAxis);
                            
							if(rDistSq > rMinSq && rDistSq < rMaxSq)
								vTargetBeads.push_back(pBead);
						}
					}
					else 
					{
						if(zBead > zMin && zBead < zMax)
						{
							rDistSq = (xBead - xAxis)*(xBead - xAxis) + 
                            (yBead - yAxis)*(yBead - yAxis);
                            
							if(rDistSq > rMinSq && rDistSq < rMaxSq)
								vTargetBeads.push_back(pBead);
						}
					}
				}
			}
		}
	}
    
	const long beadTotal = vTargetBeads.size();
    
	// Create a CCommandTarget to hold the beads so that commands may be
	// targetted to them later. We pass in the bead type in case the 
	// targetBeads container is empty as we still want to know what type 
	// of bead was specified.
    
	if(pSimBox->CreateCommandTarget(label, beadType, vTargetBeads))
	{
		 new CLogSelectBeadTypeInRegion(pSimBox->GetCurrentTime(), label, "Cylinder", beadName, beadType, beadTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
    
#endif
}

