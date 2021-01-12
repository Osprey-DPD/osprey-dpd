/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentsTo3dRectangularLatticeVectorImpl.cpp: implementation of the cgcSetArgumentsTo3dRectangularLatticeVectorImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentsTo3dRectangularLatticeVectorImpl.h"
#include "cgcSetArgumentsTo3dRectangularLatticeVector.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "tguCommandGroup.h"
#include "LogcgcSetArgumentsTo3dRectangularLatticeVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentsTo3dRectangularLatticeVectorImpl::cgcSetArgumentsTo3dRectangularLatticeVectorImpl()
{
}

cgcSetArgumentsTo3dRectangularLatticeVectorImpl::~cgcSetArgumentsTo3dRectangularLatticeVectorImpl()
{
}

// Command handler to assign values to two arguments (not necessarily from the same
// command) taken from the components of a vector in a 3d rectangular lattice.

void cgcSetArgumentsTo3dRectangularLatticeVectorImpl::SetArgumentsTo3dRectangularLatticeVector(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgcSetArgumentsTo3dRectangularLatticeVector* const pCmd = dynamic_cast<const cgcSetArgumentsTo3dRectangularLatticeVector*>(pCommand);

	const zString groupName	  = pCmd->GetCommandGroupName();
	const long    xCmdIndex   = pCmd->GetXCommandIndex();	    
	const long    yCmdIndex   = pCmd->GetXCommandIndex();	    
	const long    zCmdIndex   = pCmd->GetZCommandIndex();	    
	const zString xArgName	  = pCmd->GetXArgName();	    
	const zString yArgName	  = pCmd->GetYArgName();	 
	const zString zArgName	  = pCmd->GetZArgName();	 
    const long    xmax        = pCmd->GetXMax();      
    const long    ymax        = pCmd->GetYMax();      
    const long    zmax        = pCmd->GetZMax();      
    const double  ox          = pCmd->GetXOrigin();
    const double  oy          = pCmd->GetYOrigin();
    const double  oz          = pCmd->GetZOrigin();
    const double  lx          = pCmd->GetLX();
    const double  ly          = pCmd->GetLY();
    const double  lz          = pCmd->GetLZ();
    
    CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


	// Retrieve the group and the command name (for the log message), noting that
    // the container is zero-indexed, and modify the command's argument

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        pGroup->SetArgumentsTo3dRectangularLatticeVector(xCmdIndex, yCmdIndex, zCmdIndex, xArgName, yArgName, zArgName, xmax, ymax, zmax, ox, oy, oz, lx, ly, lz);

        const zString xCmdName = pGroup->GetCommandName(xCmdIndex);
        const zString yCmdName = pGroup->GetCommandName(yCmdIndex);
        const zString zCmdName = pGroup->GetCommandName(zCmdIndex);

        new CLogcgcSetArgumentsTo3dRectangularLatticeVector(pSimBox->GetCurrentTime(), groupName, 
                                                                    xCmdName, yCmdName, zCmdName, xArgName, yArgName, zArgName, 
                                                                    xCmdIndex, yCmdIndex, zCmdIndex, xmax, ymax, zmax, ox, oy, oz, lx, ly, lz);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}
