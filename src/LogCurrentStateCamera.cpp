/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogCurrentStateCamera.cpp: implementation of the CLogCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogCurrentStateCamera.h"


// STL using declarations

	using std::setw;
	using std::setprecision;


//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogCurrentStateCamera& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SetCurrentStateCamera</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<Camera>"	<< zEndl;
	os << "  <XC>"		<< rMsg.m_CameraX << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_CameraY << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_CameraZ << "</ZC>" << zEndl;
	os << "</Camera>"	<< zEndl;
	os << "<Target>"	<< zEndl;
	os << "  <XC>"		<< rMsg.m_TargetX << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_TargetY << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_TargetZ << "</ZC>" << zEndl;
	os << "</Target>"	<< zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Camera coordinates are now ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_CameraX << "  ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_CameraY << "  ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_CameraZ << "  ";
	os << "and target coordinates are ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_TargetX << "  ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_TargetY << "  ";
	os << setw(6) << setprecision(4) << zLeft;
	os << rMsg.m_TargetZ;

#endif

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogCurrentStateCamera::CLogCurrentStateCamera(long time, const double cx, const double cy, const double cz,
														  const double tx, const double ty, const double tz) : CLogInfoMessage(time),
														  m_CameraX(cx), m_CameraY(cy), m_CameraZ(cz),
														  m_TargetX(tx), m_TargetY(ty), m_TargetZ(tz)
{
}

CLogCurrentStateCamera::~CLogCurrentStateCamera()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogCurrentStateCamera::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << *this;
}

