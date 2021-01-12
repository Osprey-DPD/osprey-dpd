/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInSliceImpl.cpp: implementation of the ccSelectBeadTypeInSliceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectBeadTypeInSliceImpl.h"
#include "ccSelectBeadTypeInSlice.h"
#include "SimBox.h"
#include "Bead.h"
#include "Slice.h"
#include "CommandTarget.h"
#include "LogSelectBeadTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInSliceImpl::ccSelectBeadTypeInSliceImpl()
{

}

ccSelectBeadTypeInSliceImpl::~ccSelectBeadTypeInSliceImpl()
{

}

// Function to implement a ccSelectBeadTypeInRegion command that collects 
// beads of the (single) specified type whose coordinates lie within a
// rectangular slice of the SimBox. Note that the slice does not have to
// span the SimBox in any dimension and its half-width in any dimension
// cannot be greater than one-half of the SimBox size in that dimension.
//
// The slice normal is irrelevant in this command, but may be used in
// later commands that allow an unrestricted slice orientation.
//
// The beads are packaged into a CCommandTarget aggregate that can 
// receive subsequent ctApplyCommand commands.
//
// We assume that the command data has been validated prior to this routine.

void ccSelectBeadTypeInSliceImpl::SelectBeadTypeInSlice(const xxCommand* const pCommand)
{    
#if EnableTargetCommand == SimCommandEnabled
    
    const ccSelectBeadTypeInSlice* const pCmd = dynamic_cast<const ccSelectBeadTypeInSlice*>(pCommand);
    
	// Data needed to define an aggregate
    
	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString beadName		= pCmd->GetBeadName();		// Name of bead type
    
//	const long xNormal			= pCmd->GetXNormal();
//	const long yNormal			= pCmd->GetYNormal();		//	Slice normal
//	const long zNormal			= pCmd->GetZNormal();
	const double xCentre		= pCmd->GetXCentre();
	const double yCentre		= pCmd->GetYCentre();		//	Slice centre (fraction)
	const double zCentre		= pCmd->GetZCentre();
	const double xHalfWidth 	= pCmd->GetXHalfWidth();
	const double yHalfWidth		= pCmd->GetYHalfWidth();	// Slice half widths (fraction)	
	const double zHalfWidth		= pCmd->GetZHalfWidth();
    
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
    
	// Find the integer type of the selected beads from their name
    
	const long beadType = pSimBox->GetBeadTypeFromName(beadName);
    
	// Iterate over slices through the SimBox with a Z normal. Because we
	// check all bead coordinates in all three dimensions it is unimportant
	// which axis we use as the normal. We use the fact that the centre and 
	// widths of the slice are specified as fractions of the SimBox side lengths to
	// get the lower and upper slice indices corresponding to each dimension. 
	// To be sure we get all the beads, we increase the
	// indices by one checking that this does not cause them to exceed
	// the SimBox size.
    
	long lowerIndex = static_cast<long>((zCentre - zHalfWidth)*pSimBox->GetSimBoxZLength());
	long upperIndex = static_cast<long>((zCentre + zHalfWidth)*pSimBox->GetSimBoxZLength());
    
	// Adjust the slice widths for the 0th and (N-1)th CNTCell slices
    
	if(lowerIndex > 0)
		lowerIndex--;
    
	if(upperIndex == pSimBox->GetCNTZCellNo())
		upperIndex--;
	else if(upperIndex < (pSimBox->GetCNTZCellNo()-1))
		upperIndex++;
    
	double xMin = (xCentre - xHalfWidth)*pSimBox->GetSimBoxXLength();
	double xMax = (xCentre + xHalfWidth)*pSimBox->GetSimBoxXLength();
	double yMin = (yCentre - yHalfWidth)*pSimBox->GetSimBoxYLength();
	double yMax = (yCentre + yHalfWidth)*pSimBox->GetSimBoxYLength();
	double zMin = (zCentre - zHalfWidth)*pSimBox->GetSimBoxZLength();
	double zMax = (zCentre + zHalfWidth)*pSimBox->GetSimBoxZLength();
    
	// The GetBeads() function does not seem to work when used inside the 
	// for-loop statements, so I copy it to a local variable before looping.
    
	BeadVector vTargetBeads;
    
	vTargetBeads.clear();
    
	for(long sliceIndex=lowerIndex; sliceIndex<=upperIndex; sliceIndex++)
	{
		CSlice targetSlice(sliceIndex, 0, 0, 1, pSimBox->GetISimBox());
        
		BeadList sliceBeads = targetSlice.GetBeads();
        
		for(cBeadListIterator iterBead=sliceBeads.begin(); iterBead!=sliceBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == beadType)
			{
				CBead* const pBead = dynamic_cast<CBead*>(*iterBead);
                
				if(pBead)
				{
					double xBead = pBead->GetXPos();
					double yBead = pBead->GetYPos();
					double zBead = pBead->GetZPos();
                    
					if(xBead > xMin && xBead < xMax &&
					   yBead > yMin && yBead < yMax &&
					   zBead > zMin && zBead < zMax)
					{
						vTargetBeads.push_back(pBead);
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
		 new CLogSelectBeadTypeInRegion(pSimBox->GetCurrentTime(), label, "Slice", beadName, beadType, beadTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
    
#endif
}
