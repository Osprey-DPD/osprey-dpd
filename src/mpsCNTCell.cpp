/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsCNTCell.cpp: implementation of the mpsCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "mpsCNTCell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mpsCNTCell::mpsCNTCell() : CAbstractCell(), m_bExternal(false)
{

}

mpsCNTCell::mpsCNTCell(long index, long coords[3], bool flag)
{
	// Initialise the member variables of CAbstractCell 

	m_id			= index;

	m_BLIndex[0]	= coords[0];
	m_BLIndex[1]	= coords[1];
	m_BLIndex[2]	= coords[2];
/*
	m_BLCoord[0]	= static_cast<double>(coords[0])*m_CNTXCellWidth;
	m_BLCoord[1]	= static_cast<double>(coords[1])*m_CNTYCellWidth;
	m_BLCoord[2]	= static_cast<double>(coords[2])*m_CNTZCellWidth;

	m_TRCoord[0]	= m_BLCoord[0] + m_CNTXCellWidth;
	m_TRCoord[1]	= m_BLCoord[1] + m_CNTYCellWidth;
	m_TRCoord[2]	= m_BLCoord[2] + m_CNTZCellWidth;
*/

	// Now the local member variables

	m_bExternal		= flag;

}

mpsCNTCell::~mpsCNTCell()
{

}



