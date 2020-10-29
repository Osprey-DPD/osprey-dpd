/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccAddActiveCellNetwork.cpp: implementation of the ccAddActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccAddActiveCellNetwork.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// No registration of this command is provided as only derived classes
// can be instantiated.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccAddActiveCellNetwork::ccAddActiveCellNetwork(long executionTime) : xxCommand(executionTime),
													   m_XCellNo(0),
													   m_YCellNo(0),
													   m_ZCellNo(0)
{
}

ccAddActiveCellNetwork::ccAddActiveCellNetwork(const ccAddActiveCellNetwork& oldCommand) : xxCommand(oldCommand),
													   m_XCellNo(oldCommand.m_XCellNo),
													   m_YCellNo(oldCommand.m_YCellNo),
													   m_ZCellNo(oldCommand.m_ZCellNo)
{
}


ccAddActiveCellNetwork::~ccAddActiveCellNetwork()
{
}

// Member functions to read/write the data specific to the command.
// Because this command is the base class of more specific commands,
// we do not write the start and end tags here: it is the responsibility 
// of the derived classes to supply them.
//
// Arguments
// *********
//
//	m_XCellNo
//	m_YCellNo	- Number of cells in the active network
//	m_ZCellNo

zOutStream& ccAddActiveCellNetwork::put(zOutStream& os) const
{
#if EnableACNCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output - start and end tags provided by derived classes
	os << "<XCellNo>" << m_XCellNo << "</XCellNo>" << zEndl;
	os << "<YCellNo>" << m_YCellNo << "</YCellNo>" << zEndl;
	os << "<ZCellNo>" << m_ZCellNo << "</ZCellNo>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << m_XCellNo << " " << m_YCellNo << " " << m_ZCellNo;

#endif
#endif
	return os;
}

zInStream& ccAddActiveCellNetwork::get(zInStream& is)
{
#if EnableACNCommand == SimCommandEnabled
	is >> m_XCellNo >> m_YCellNo >> m_ZCellNo;

	if(!is.good())
	   SetCommandValid(false);
#endif
	return is;
}

// Function to check that the data defining the active network is valid.
// We check that the number of CNT cells in each active cell in each dimension 
// is greater than zero and less than or equal to the number of CNT cells in the 
// CSimBox. We also ensure that the number of CNT cells per active cell is a
// whole number in each dimension.
//
// Note that a value of 1 means that an active cell contains exactly 1 CNT cell.

bool ccAddActiveCellNetwork::IsDataValid(const CInputData& riData) const
{
	if(m_XCellNo < 1 || m_YCellNo < 1 || m_ZCellNo < 1)
		return ErrorTrace("Active network has less than 1 cell per dimension");
	else if(m_XCellNo > riData.GetCNTXCellNo() ||
			m_YCellNo > riData.GetCNTYCellNo() ||
			m_ZCellNo > riData.GetCNTZCellNo())
		return ErrorTrace("Active network has more cells than CNT network");
	else if(riData.GetCNTXCellNo() % m_XCellNo != 0 ||
		    riData.GetCNTYCellNo() % m_YCellNo != 0 ||
		    riData.GetCNTZCellNo() % m_ZCellNo != 0)
		return ErrorTrace("Active network size not a divisor of CNT network size");

	return true;
}
