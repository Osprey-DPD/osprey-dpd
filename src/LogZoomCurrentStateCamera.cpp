/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogZoomCurrentStateCamera.cpp: implementation of the CLogZoomCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogZoomCurrentStateCamera.h"



//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogZoomCurrentStateCamera& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>"  << zEndl;
	os << "<Name>ZoomCurrentStateCamera</Name>" << zEndl;
	os << "<Text>"  << zEndl;
	os << "<Camera>"	<< zEndl;
	os << "  <XC>"		<< rMsg.m_Camera[0] << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_Camera[1] << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_Camera[2] << "</ZC>" << zEndl;
	os << "</Camera>"	<< zEndl;
	os << "<Target>"    << zEndl;
	os << "  <XC>"		<< rMsg.m_Target[0] << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_Target[1] << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_Target[2] << "</ZC>" << zEndl;
	os << "</Target>"     << zEndl;
	os << "<ScaleFactor>"		<< rMsg.m_ScaleFactor	<< "</ScaleFactor>"		<< zEndl;
	os << "<Duration>"			<< rMsg.m_Duration		<< "</Duration>"		<< zEndl;
	os << "<FrameRate>"			<< rMsg.m_FrameRate		<< "</FrameRate>"		<< zEndl;
	os << "<StepsPerFrame>"		<< rMsg.m_StepsPerFrame << "</StepsPerFrame>"	<< zEndl;
	os << "</Text>" << zEndl;	
	os << "</Body>" << zEndl;	

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Zooming camera at ";
	os << rMsg.m_Camera[0] << " ";
	os << rMsg.m_Camera[1] << " ";
	os << rMsg.m_Camera[2] << " (";
	os << rMsg.m_Target[0] << " ";
	os << rMsg.m_Target[1] << " ";
	os << rMsg.m_Target[2] << ") ";
	os << "by a factor " << rMsg.m_ScaleFactor << "(per snapshot) for " << rMsg.m_Duration << " sec, " << rMsg.m_FrameRate << " frames/sec, ";
	os << rMsg.m_StepsPerFrame << " steps/frame";

#endif

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogZoomCurrentStateCamera::CLogZoomCurrentStateCamera(long time, const long duration, const long frames, 
														const long steps, const double scaleFactor, 
														const double camera[3], const double target[3]) : CLogInfoMessage(time),
														m_Duration(duration), m_FrameRate(frames),
														m_StepsPerFrame(steps), m_ScaleFactor(scaleFactor)

{
	for(short int i=0; i<3; i++)
	{
		m_Camera[i] = camera[i];
		m_Target[i] = target[i];
	}
}

CLogZoomCurrentStateCamera::~CLogZoomCurrentStateCamera()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogZoomCurrentStateCamera::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << *this;
}

