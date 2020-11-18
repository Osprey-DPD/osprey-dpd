/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogVesicleBindsWall.cpp: implementation of the CLogVesicleBindsWall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogVesicleBindsWall.h"

// STL using declarations

	using std::setw;
	using std::setprecision;

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogVesicleBindsWall& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>VesicleBindsWall</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "Vesicle (id " << rMsg.m_Type << ")" << " with centre separation ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_CentreSeparation << zEndl;
	os << " bound to wall at a distance of ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_EdgeSeparation;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Vesicle (id " << rMsg.m_Type << ")" << " with centre separation ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_CentreSeparation << zEndl;
	os << " bound to wall at a distance of ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_EdgeSeparation;

#endif

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogVesicleBindsWall::CLogVesicleBindsWall(long time, long id, double d1, double d2) : CLogEventMessage(time),
											m_Type(id), m_CentreSeparation(d1), m_EdgeSeparation(d2)
{
}

CLogVesicleBindsWall::~CLogVesicleBindsWall()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogVesicleBindsWall::Serialize(zOutStream& os) const
{
	CLogEventMessage::Serialize(os);

	os << *this;
}

