/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// WallData.cpp: implementation of the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "WallData.h"


//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the wall data to file. This is included for
// completeness but we don't actually write CWallData to any file.

zOutStream& operator<<(zOutStream& os, const CWallData& wall)
{
	os << "Wall   " << zEndl;
	os << "	      " << "Polymer   " << wall.m_PolymerName	<< zEndl;
	os << "	      " << "Density   " << wall.m_Density		<< zEndl;

	os << "       " << "Thickness " << wall.m_XWidth		<< " "
								 << wall.m_YWidth		<< " "
								 << wall.m_ZWidth		<< zEndl;
	return os;
}

// Function to read the wall data from file. Note that the keyword "Wall"
// has already been read. We also assume that the data have been checked for
// validity in CInputDPDFile.
// Note also that if the simulation is two-dimensional we still read in values
// for the z dimension of the wall but they are ignored.

zInStream& operator>>(zInStream& is, CWallData& wall)
{
	zString token;
	zString name;
	double density;
	long xcells, ycells, zcells;

	is >> token;
	if(!is.good() || token != "Polymer")
	{
		wall.SetDataValid(false);
		return is;
	}
	else
	{
		is >> name;
		if(!is.good() || name.length() == 0)
		{
			wall.SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Density")
	{
		wall.SetDataValid(false);
		return is;
	}
	else
	{
		is >> density;
		if(!is.good() || density <= 0.0)
		{
			wall.SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Thickness")
	{
		wall.SetDataValid(false);
		return is;
	}
	else
	{
		is >> xcells >> ycells >> zcells;
		if(!is.good() || (xcells == 0 && ycells == 0 && zcells == 0) || 
			             (xcells < 0  || ycells < 0  || zcells < 0))
		{
			wall.SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so pass it to the member variables

	wall.m_PolymerName	= name;
	wall.m_Density		= density;
	wall.m_XWidth		= xcells;
	wall.m_YWidth		= ycells;
	wall.m_ZWidth		= zcells;

	return is;
}

// Global function to compare the data members of two CWallData objects 
// and return true if they are identical.

bool operator ==(const CWallData &a, const CWallData &b)
{
	if( a.m_Density		== b.m_Density		&&
		a.m_PolymerName == b.m_PolymerName	&&
		a.m_XWidth      == b.m_XWidth		&&
		a.m_YWidth      == b.m_YWidth		&&
		a.m_ZWidth      == b.m_ZWidth )
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We create the CWallData object with zero values for the wall polymer
// name, the density of beads in the wall, and the wall thickness in all three
// dimensions. These data are overwritten by that read from the control data file.
//

CWallData::CWallData() : xxConstraint(true),
						 m_PolymerName(""), m_Density(0.0),
						 m_XWidth(0),
						 m_YWidth(0),
						 m_ZWidth(0)
						 
{
}

CWallData::~CWallData()
{
}

CWallData::CWallData(const CWallData& oldWD) : xxConstraint(oldWD),
											   m_PolymerName(oldWD.m_PolymerName),
											   m_Density(oldWD.m_Density),												
											   m_XWidth(oldWD.m_XWidth),
											   m_YWidth(oldWD.m_YWidth),
											   m_ZWidth(oldWD.m_ZWidth)
											   
{
}


CWallData& CWallData::operator=(const CWallData& rWD)
{
	if(this != &rWD)
	{
		m_PolymerName	= rWD.m_PolymerName;
		m_Density		= rWD.m_Density;
		m_XWidth		= rWD.m_XWidth;
		m_YWidth		= rWD.m_YWidth;
		m_ZWidth		= rWD.m_ZWidth;
		m_bDataValid	= rWD.m_bDataValid;
	}

	return *this;
}

const zString CWallData::GetWallPolymerName() const
{
	return m_PolymerName;
}

double CWallData::GetWallBeadDensity() const
{
	return m_Density;
}

long CWallData::GetWallXWidth() const
{
	return m_XWidth;
}

long CWallData::GetWallYWidth() const
{
	return m_YWidth;
}

long CWallData::GetWallZWidth() const
{
	return m_ZWidth;
}
