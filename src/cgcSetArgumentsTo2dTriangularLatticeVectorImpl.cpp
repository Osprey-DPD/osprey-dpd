/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentsTo2dTriangularLatticeVectorImpl.cpp: implementation of the cgcSetArgumentsTo2dTriangularLatticeVectorImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentsTo2dTriangularLatticeVectorImpl.h"
#include "cgcSetArgumentsTo2dTriangularLatticeVector.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "tguCommandGroup.h"
#include "LogcgcSetArgumentsTo2dTriangularLatticeVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentsTo2dTriangularLatticeVectorImpl::cgcSetArgumentsTo2dTriangularLatticeVectorImpl()
{
}

cgcSetArgumentsTo2dTriangularLatticeVectorImpl::~cgcSetArgumentsTo2dTriangularLatticeVectorImpl()
{
}

// Command handler to assign values to two arguments (not necessarily from the same
// command) taken from the components of a vector in a 2d rectangular lattice.

void cgcSetArgumentsTo2dTriangularLatticeVectorImpl::SetArgumentsTo2dTriangularLatticeVector(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

    const cgcSetArgumentsTo2dTriangularLatticeVector* const pCmd = dynamic_cast<const cgcSetArgumentsTo2dTriangularLatticeVector*>(pCommand);

    const zString groupName   = pCmd->GetCommandGroupName();
    const long    xCmdIndex   = pCmd->GetXCommandIndex();	    
    const long    yCmdIndex   = pCmd->GetXCommandIndex();	    
    const zString xArgName    = pCmd->GetXArgName();	    
    const zString yArgName    = pCmd->GetYArgName();	 
    const long    xmax        = pCmd->GetXMax();      
    const long    ymax        = pCmd->GetYMax();      
    const double  ox          = pCmd->GetXOrigin();
    const double  oy          = pCmd->GetYOrigin();
    const double  lx          = pCmd->GetLX();
    const double  ly          = pCmd->GetLY();
    
    CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


	// Retrieve the group and the command name (for the log message), noting that
    // the container is zero-indexed, and modify the command's argument

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        pGroup->SetArgumentsTo2dTriangularLatticeVector(xCmdIndex, yCmdIndex, xArgName, yArgName, xmax, ymax, ox, oy, lx, ly);

        const zString xCmdName = pGroup->GetCommandName(xCmdIndex);
        const zString yCmdName = pGroup->GetCommandName(yCmdIndex);

        new CLogcgcSetArgumentsTo2dTriangularLatticeVector(pSimBox->GetCurrentTime(), groupName, xCmdName, yCmdName, 
                                                                    xArgName, yArgName, xCmdIndex, yCmdIndex, xmax, ymax, ox, oy, lx, ly);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}
