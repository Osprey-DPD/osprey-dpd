/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInSphereImpl.cpp: implementation of the ccSelectBeadTypeInSphereImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectBeadTypeInSphereImpl.h"
#include "ccSelectBeadTypeInSphere.h"
#include "SimBox.h"
#include "Bead.h"
#include "CommandTarget.h"
#include "LogSelectBeadTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInSphereImpl::ccSelectBeadTypeInSphereImpl()
{

}

ccSelectBeadTypeInSphereImpl::~ccSelectBeadTypeInSphereImpl()
{

}

// Function to implement a ccSelectBeadTypeInRegion command that collects 
// beads of the (single) specified type whose coordinates lie within a
// sphere, or spherical shell if the inner radius is non-zero.
//
// The beads are packaged into a CCommandTarget aggregate that can 
// receive subsequent ctApplyCommand commands.
//
// We assume that the command data has been validated prior to this routine.

void ccSelectBeadTypeInSphereImpl::SelectBeadTypeInSphere(const xxCommand* const pCommand)
{    
#if EnableTargetCommand == SimCommandEnabled
        
    const ccSelectBeadTypeInSphere* const pCmd = dynamic_cast<const ccSelectBeadTypeInSphere*>(pCommand);
        
    const zString label			= pCmd->GetTargetLabel();	// Label of command target
    const zString beadName		= pCmd->GetBeadName();		// Name of bead type
        
    const double xCentre		= pCmd->GetXCentre();
    const double yCentre		= pCmd->GetYCentre();		//	Sphere's centre (fraction)
    const double zCentre		= pCmd->GetZCentre();
    const double innerRadius	= pCmd->GetInnerRadius();	// Inner radius of shell
    const double outerRadius	= pCmd->GetOuterRadius();	// Inner radius of shell
        
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
    
	// Find the integer type of the selected beads from their name
    
	const long beadType = pSimBox->GetBeadTypeFromName(beadName);
        
	const double xc		  = xCentre*pSimBox->GetSimBoxXLength();
	const double yc		  = yCentre*pSimBox->GetSimBoxYLength();
	const double zc		  = zCentre*pSimBox->GetSimBoxZLength();
    const double rinsq    = innerRadius*innerRadius;
    const double routsq   = outerRadius*outerRadius;
        
    // Loop over all the beads in the SimBox and store pointers to those whose
    // type matches the specified type, and whose coordinates lie within the 
    // specified spherical region. Note that this may be a sphere or a shell.
    // We use the square of distances to avoid having to take square roots.
        
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
            vTargetBeads.push_back(dynamic_cast<CBead*>(*iterBead));
        }
    }
        
    const long beadTotal = vTargetBeads.size();
        
    // Create and store a CCommandTarget to hold the beads and receive commands.
    // We pass in the bead type in case the targetBeads container is empty as
    // we still want to know what type of bead was specified.
        
    if(pSimBox->CreateCommandTarget(label, beadType, vTargetBeads))
    {
         new CLogSelectBeadTypeInRegion(pSimBox->GetCurrentTime(), label, "Sphere", beadName, beadType, beadTotal);
    }
    else
    {
         new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
    }
        
#endif
}

