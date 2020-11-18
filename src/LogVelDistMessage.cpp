/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogVelDistMessage.cpp: implementation of the CLogVelDistMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogVelDistMessage.h"

// STL using declarations

	using std::setw;
	using std::setprecision;

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogVelDistMessage& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>InitialVelocityDistribution</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<Temperature>" << rMsg.m_Temperature	<< "</Temperature>"	<< zEndl;
	os << "<MeanVelocity>"	<< zEndl;
	os << "  <XC>"		<< rMsg.m_MeanVel[0] << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_MeanVel[1] << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_MeanVel[2] << "</ZC>" << zEndl;
	os << "</MeanVelocity>"	<< zEndl;
	os << "<MeanSquareVelocity>" << zEndl;
	os << "  <XC>"		<< rMsg.m_Mean2Vel[0] << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_Mean2Vel[1] << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_Mean2Vel[2] << "</ZC>" << zEndl;
	os << "</MeanSquareVelocity>" << zEndl;
	os << "<VarianceVelocity>" << zEndl;
	os << "  <XC>"		<< rMsg.m_VarVel[0] << "</XC>" << zEndl;
	os << "  <YC>"		<< rMsg.m_VarVel[1] << "</YC>" << zEndl;
	os << "  <ZC>"		<< rMsg.m_VarVel[2] << "</ZC>" << zEndl;
	os << "</VarianceVelocity>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Velocity distn" << zEndl;
	os << "Temperature = " << rMsg.m_Temperature << zEndl;

	os << std::setw(10) << setprecision(6) << zLeft;
	os << "Mean Vel    = " << rMsg.m_MeanVel[0] << " " 
						   << rMsg.m_MeanVel[1] << " " 
						   << rMsg.m_MeanVel[2] << " " << zEndl;

	os << std::setw(10) << setprecision(6) << zLeft;
	os << "Mean Sq Vel = " << rMsg.m_Mean2Vel[0] << " " 
						   << rMsg.m_Mean2Vel[1] << " " 
						   << rMsg.m_Mean2Vel[2] << " " << zEndl;

	os << std::setw(10) << setprecision(6) << zLeft;
	os << "Var Vel     = " << rMsg.m_VarVel[0] << " " 
						   << rMsg.m_VarVel[1] << " " 
						   << rMsg.m_VarVel[2];

#endif

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogVelDistMessage::CLogVelDistMessage(long time, double temp, double vmean[3], double v2mean[3], 
									   double var[3]) : CLogInfoMessage(time), m_Temperature(temp)
{
	// Store mean velocity components

	for(short int i=0; i<3; i++)
	{
		m_MeanVel[i]	= vmean[i];
		m_Mean2Vel[i]	= v2mean[i];
		m_VarVel[i]		= var[i];
	}
}

CLogVelDistMessage::~CLogVelDistMessage()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogVelDistMessage::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << *this;
}

