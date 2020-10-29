/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogcgcSetArgumentsTo2dRectangularLatticeVector.cpp: implementation of the CLogcgcSetArgumentsTo2dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogcgcSetArgumentsTo2dRectangularLatticeVector.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentsTo2dRectangularLatticeVector& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<Name>SetArgumentsTo2dRectangularLatticeVector</Name>" << zEndl;
    os << "<CommandGroupName>" << rMsg.m_GroupName     << "</CommandGroupName>" << zEndl;
	os << "<XCommandName>"     << rMsg.m_XCommandName  << "</XCommandName>"    << zEndl;
	os << "<YCommandName>"     << rMsg.m_YCommandName  << "</YCommandName>"    << zEndl;
	os << "<XCommandIndex>"    << rMsg.m_XCommandIndex << "</XCommandIndex>"    << zEndl;
	os << "<YCommandIndex>"    << rMsg.m_YCommandIndex << "</YCommandIndex>"    << zEndl;
	os << "<XArgumentName>"    << rMsg.m_XArgName      << "</XArgumentName>"    << zEndl;
	os << "<YArgumentName>"    << rMsg.m_YArgName      << "</YArgumentName>"    << zEndl;
	os << "<XMax>"             << rMsg.m_XMax          << "</XMax>"             << zEndl;
	os << "<YMax>"             << rMsg.m_YMax          << "</YMax>"             << zEndl;
	os << "<XOrigin>"          << rMsg.m_XOrigin       << "</XOrigin>"          << zEndl;
	os << "<YOrigin>"          << rMsg.m_YOrigin       << "</YOrigin>"          << zEndl;
	os << "<LX>"               << rMsg.m_LX            << "</LX>"               << zEndl;
	os << "<LY>"               << rMsg.m_LY            << "</LY>"               << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
    os << "Setting arguments " << rMsg.m_XArgName << " " << rMsg.m_YArgName << " of commands ";
    os << rMsg.m_XCommandName << " (index " << rMsg.m_XCommandNo << ") ";
    os << rMsg.m_YCommandName << " (index " << rMsg.m_YCommandNo << ") ";
    os << "to the X,Y components of vectors taken from a 2d rectangular lattice with size ";
    os << rMsg.m_XMax << " " << rMsg.m_YMax << " origin " << rMsg.m_XOrigin << " " << rMsg.m_YOrigin << " and spacing " << rMsg.m_LX << " " << rMsg.m_LY;
    os << " in group " << rMsg.m_GroupName;
        
#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogcgcSetArgumentsTo2dRectangularLatticeVector::CLogcgcSetArgumentsTo2dRectangularLatticeVector(long time, const zString groupName, 
                                                 const zString xCmdName, const zString yCmdName, 
                                                 const zString xArgName, const zString yArgName, 
                                                 long xCmdNo, long yCmdNo, long xmax, long ymax,
                                                 double xorigin, double yorigin, double lx, double ly) : CLogInfoMessage(time), 
                                                 m_GroupName(groupName),
                                                 m_XCommandName(xCmdName), m_YCommandName(yCmdName),
                                                 m_XArgName(xArgName), m_YArgName(yArgName),
                                                 m_XCommandNo(xCmdNo), m_YCommandNo(yCmdNo), 
                                                 m_XMax(xmax), m_YMax(ymax),
                                                 m_XOrigin(xorigin), m_YOrigin(yorigin),
                                                 m_LX(lx), m_LY(ly)
                                                   
{
}

CLogcgcSetArgumentsTo2dRectangularLatticeVector::~CLogcgcSetArgumentsTo2dRectangularLatticeVector()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogcgcSetArgumentsTo2dRectangularLatticeVector::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

