/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogBondStiffnessChanged.cpp: implementation of the CLogBondStiffnessChanged class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogBondStiffnessChanged.h"


// STL using declarations

	using std::setw;
	using std::setprecision;

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogBondStiffnessChanged& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SetBondStiffness</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<BondPairName>" << rMsg.m_Name  << "</BondPairName>" << zEndl;
	os << "<Type>"         << rMsg.m_Type  << "</Type>" << zEndl;
	os << "<BendingModulus>" << setw(8) << setprecision(4) << zLeft << rMsg.m_BendingModulus << "</BendingModulus>" << zEndl;
	os << "<PreferredAngle>" << setw(8) << setprecision(4) << zLeft << rMsg.m_Phi0           << "</PreferredAngle>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "BondPair " << rMsg.m_Name << " (type " << rMsg.m_Type << ")" << " stiffness is now ";
	os << setw(8) << setprecision(4) << zLeft;
	os << rMsg.m_BendingModulus << "  ";
	os << setw(8) << setprecision(4) << zLeft;
	os << rMsg.m_Phi0;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogBondStiffnessChanged::CLogBondStiffnessChanged(long time, 
												   const zString name, long type, 
												   double modulus, double phi0) : CLogConstraintMessage(time),
												   m_Name(name), m_Type(type),
												   m_BendingModulus(modulus),
												   m_Phi0(phi0)
{

}

CLogBondStiffnessChanged::~CLogBondStiffnessChanged()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogBondStiffnessChanged::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << *this;
}

