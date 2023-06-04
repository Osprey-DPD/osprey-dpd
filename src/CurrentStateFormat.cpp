/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CurrentStateFormat.cpp: implementation of the CCurrentStateFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CurrentStateFormat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We hold data here that is common to all the formats used to output the
// current state data. 

CCurrentStateFormat::CCurrentStateFormat(double lx, double ly, double lz, bool bDisplayBox,
							  const long beadTypeTotal) : m_SimBoxXLength(lx), 
														  m_SimBoxYLength(ly), 
														  m_SimBoxZLength(lz),
														  m_bDisplayBox(bDisplayBox),
														  m_BeadTypeTotal(beadTypeTotal)
{
	// Default size of bounding box is the SimBox size

	m_BLBoxCoord[0] = 0.0;
	m_BLBoxCoord[1] = 0.0;
	m_BLBoxCoord[2] = 0.0;

	m_TRBoxCoord[0] = m_SimBoxXLength;
	m_TRBoxCoord[1] = m_SimBoxYLength;
	m_TRBoxCoord[2] = m_SimBoxZLength;

	// Define colours for the beads in the output file. We define 9 distinct
	// colours, 19 shades of grey and make all the rest red.

	m_BeadColours.clear();

	m_BeadColours.push_back("Red");
	m_BeadColours.push_back("Orange");
	m_BeadColours.push_back("Yellow");
	m_BeadColours.push_back("Green");
	m_BeadColours.push_back("Blue");
	m_BeadColours.push_back("BlueViolet");
	m_BeadColours.push_back("Violet");
	m_BeadColours.push_back("Cyan");
	m_BeadColours.push_back("Magenta");

	m_BeadColours.push_back("Gray05");
	m_BeadColours.push_back("Gray10");
	m_BeadColours.push_back("Gray15");
	m_BeadColours.push_back("Gray20");
	m_BeadColours.push_back("Gray25");
	m_BeadColours.push_back("Gray30");
	m_BeadColours.push_back("Gray35");
	m_BeadColours.push_back("Gray40");
	m_BeadColours.push_back("Gray45");
	m_BeadColours.push_back("Gray50");
	m_BeadColours.push_back("Gray55");
	m_BeadColours.push_back("Gray60");
	m_BeadColours.push_back("Gray65");
	m_BeadColours.push_back("Gray70");
	m_BeadColours.push_back("Gray75");
	m_BeadColours.push_back("Gray80");
	m_BeadColours.push_back("Gray85");
	m_BeadColours.push_back("Gray90");
	m_BeadColours.push_back("Gray95");

	if(m_BeadTypeTotal > static_cast<long>(m_BeadColours.size()))
	{
		for(long beadNo=m_BeadColours.size(); beadNo<m_BeadTypeTotal; beadNo++)
		{
			m_BeadColours.push_back("Scarlet");
		}
	}

}

CCurrentStateFormat::~CCurrentStateFormat()
{

}

// Default behaviour is that formats don't need to know about bonds
bool CCurrentStateFormat::UsesBonds() const
{
	return false;
}

// Protected function that returns a named colour given a numeric bead type.
// If the type is larger than the colour array, we return the first entry.

const zString CCurrentStateFormat::GetBeadColourFromType(long beadType) const
{
    if(beadType < static_cast<long>(m_BeadColours.size()))
    {
        return m_BeadColours.at(beadType);
    }
    else
    {
        return m_BeadColours.at(0);
    }
}


// Do-nothing implementation of the function to write out snapshot information 
// following the bead data. Not all formats require this, so we make it a
// non-pure virtual function. Any derived class that wants to use it must
// override this function.

void CCurrentStateFormat::SerializeFooter(zOutStream& os, const long beadTotal)
{
}

// Do-nothing implementation.
void CCurrentStateFormat::SerializeBond(zOutStream& os, const CPolymer &polymer, const CBond &bond)
{
}