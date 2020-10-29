/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogCNTBeadError.cpp: implementation of the CLogCNTBeadError class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogCNTBeadError.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogCNTBeadError& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>CNT Bead Error</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "Bead error in CNT cell " << rMsg.m_CellXNo << " " << rMsg.m_CellYNo << " " << rMsg.m_CellZNo << zEndl;
	os << "Bead " << rMsg.m_pBead->GetId() << zEndl;
	os << "  has position " << rMsg.m_pBead->GetXPos()   << " " << rMsg.m_pBead->GetYPos()   << " " << rMsg.m_pBead->GetZPos()   << zEndl;
	os << "  and momentum " << rMsg.m_pBead->GetXMom()   << " " << rMsg.m_pBead->GetYMom()   << " " << rMsg.m_pBead->GetZMom()   << zEndl;
	os << "  and force    " << rMsg.m_pBead->GetXForce() << " " << rMsg.m_pBead->GetYForce() << " " << rMsg.m_pBead->GetZForce() << zEndl;
	os << "Expected index " << rMsg.m_ExpectIndex << " but actual index is " << rMsg.m_ActualIndex;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Bead error in CNT cell " << rMsg.m_CellXNo << " " << rMsg.m_CellYNo << " " << rMsg.m_CellZNo << zEndl;
	os << "Bead " << rMsg.m_pBead->GetId() << zEndl;
	os << "  has position " << rMsg.m_pBead->GetXPos()   << " " << rMsg.m_pBead->GetYPos()   << " " << rMsg.m_pBead->GetZPos()   << zEndl;
	os << "  and momentum " << rMsg.m_pBead->GetXMom()   << " " << rMsg.m_pBead->GetYMom()   << " " << rMsg.m_pBead->GetZMom()   << zEndl;
	os << "  and force    " << rMsg.m_pBead->GetXForce() << " " << rMsg.m_pBead->GetYForce() << " " << rMsg.m_pBead->GetZForce() << zEndl;
	os << "Expected index " << rMsg.m_ExpectIndex << " but actual index is " << rMsg.m_ActualIndex;

#endif

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogCNTBeadError::CLogCNTBeadError(long time, long ix, long iy, long iz, 
								   long expectIndex, long actualIndex, 
								   const CAbstractBead* const pBead) : CLogWarningMessage(time),
																	   m_CellXNo(ix),
																	   m_CellYNo(iy),
																	   m_CellZNo(iz),
																	   m_ExpectIndex(expectIndex),
																	   m_ActualIndex(actualIndex),
																	   m_pBead(pBead)
{

}

CLogCNTBeadError::~CLogCNTBeadError()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogCNTBeadError::Serialize(zOutStream& os) const
{
	CLogWarningMessage::Serialize(os);

	os << (*this);
}

