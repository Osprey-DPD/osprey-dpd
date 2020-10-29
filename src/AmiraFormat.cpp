/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AmiraFormat.cpp: implementation of the CAmiraFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "AmiraFormat.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the file extension for this 
// visualisation format class.

const zString CAmiraFormat::m_FileExtension = ".psi";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmiraFormat::CAmiraFormat(double lx, double ly, double lz, bool bDisplayBox,
						   const long beadTypeTotal) : CCurrentStateFormat(lx, ly, lz, bDisplayBox, beadTypeTotal),
						   m_AVar1(1), m_AVar2(1), m_AVar3(1), m_AVar4(1), m_AVar5(1)
{

}

CAmiraFormat::~CAmiraFormat()
{

}

// Function used to write out header data for the current format class.
// the output stream object is passed in from the CCurrentState object
// that uses the format class
//
// Here we set up the columns of data and the number of beads and some
// other parameters possibly used by Amira.

void CAmiraFormat::SerializeHeader(zOutStream& os, const long beadTotal)
{
	os << "# PSI Format 1.0							" << zEndl;
	os << "#										" << zEndl;
	os << "# column[0] = \"x\"						" << zEndl;
	os << "# column[1] = \"y\"						" << zEndl;
	os << "# column[2] = \"z\"						" << zEndl;
	os << "# column[3] = \"Energy\"					" << zEndl;
	os << "# column[4] = \"Grain\"					" << zEndl;
	os << "# column[5] = \"Id\"						" << zEndl;
	os << "# column[6] = \"Coordination Number\"	" << zEndl;
	os << "# column[7] = \"Crystallographic Class\"	" << zEndl;
	os << "# type[3]   = float						" << zEndl;
	os << "# type[4]   = float						" << zEndl;
	os << "# type[6]   = float						" << zEndl;
	os << "# type[7]   = float						" << zEndl;
	os << "#					" << zEndl;
	os << "#					" << zEndl;
	os << "#					" << zEndl << zEndl;

	os << beadTotal << " " << m_AVar1 << " " << m_AVar2 << zEndl << zEndl;

	if(m_bDisplayBox)
	{
		os <<  m_BLBoxCoord[0] << " "; 
		os <<  m_BLBoxCoord[1] << " ";
		os <<  m_BLBoxCoord[2] << zEndl;
		os <<  m_TRBoxCoord[0] << " ";
		os <<  m_TRBoxCoord[1] << " ";
		os <<  m_TRBoxCoord[2] << zEndl;
		os <<  m_AVar1 << " " << m_AVar1 << " " << m_AVar1 << zEndl << zEndl;
	}
	else
	{
		os <<  m_AVar1 << " " << m_AVar1 << " " << m_AVar1 << zEndl;
		os <<  m_AVar1 << " " << m_AVar1 << " " << m_AVar1 << zEndl;
		os <<  m_AVar1 << " " << m_AVar1 << " " << m_AVar1 << zEndl << zEndl;
	}

}

// Function to write out the coordinates of a single bead. The bead is drawn
// as a sphere with a given radius and centre coordinates.

void CAmiraFormat::SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								  const double x, const double y, const double z)
{
	os << x   << " "     << y   << " "     << z;
	os << " " << radius  << " " << m_AVar3 << " " << type;
	os << " " << m_AVar4 << " " << m_AVar5 << zEndl;
}
