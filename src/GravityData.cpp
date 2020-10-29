/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// GravityData.cpp: implementation of the CGravityData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "GravityData.h"


//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the gravity data to file. This is included for
// completeness but we don't actually write CGravityData to any file.

zOutStream& operator<<(zOutStream& os, const CGravityData& gravity)
{

	os << "Gravity" << zEndl;
	os << "       " << "Beads ";

	for(cStringSequenceIterator iterBead=gravity.m_vBeads.begin(); iterBead!=gravity.m_vBeads.end(); iterBead++)
	{
		os << *iterBead->c_str() << " ";
	}
	os << zEndl;

	os << "       " << "Force  " << gravity.m_XForce	<< " "
								 << gravity.m_YForce	<< " "
								 << gravity.m_ZForce	<< zEndl;
	return os;
}

// Function to read the gravity data from file. Note that the keyword "Gravity"
// has already been read. We also assume that the data have been checked for
// validity in CInputDPDFile.
// Note also that if the simulation is two-dimensional we still read in a value
// for the z component of the force but it is ignored.

zInStream& operator>>(zInStream& is, CGravityData& gravity)
{
	zString token;
	zString name;
	StringSequence vBeads;
	double xforce, yforce, zforce;

	vBeads.clear();		// ensure bead name vector is empty

	is >> token;
	if(!is.good() || token != "Beads")
	{
		gravity.SetDataValid(false);
		return is;
	}
	else
	{
		is >> name;
		if(name == "Force")
		{
			gravity.SetDataValid(false);
			return is;
		}
		else
		{
			while(name != "Force")
			{
				if(!is.good())
				{
					gravity.SetDataValid(false);
					return is;
				}
				vBeads.push_back(name);
				is >> name;
			}
		}
	}

	if(!is.good() || name != "Force")
	{
		gravity.SetDataValid(false);
		return is;
	}
	else
	{
		is >> xforce >> yforce >> zforce;
		if(!is.good())
		{
			gravity.SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so pass it to the member variables

	gravity.m_vBeads	= vBeads;
	gravity.m_XForce	= xforce;
	gravity.m_YForce	= yforce;
	gravity.m_ZForce	= zforce;

	return is;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We create the CGravityData object with zero values for the external force.
// This is overwritten by the data read from the control data file.
//

CGravityData::CGravityData() : xxConstraint(true),
							   m_XForce(0.0), m_YForce(0.0), m_ZForce(0.0)						   
{
	m_vBeads.clear();	// ensure vector is empty
}

CGravityData::~CGravityData()
{
}

CGravityData::CGravityData(const CGravityData& oldGD) : xxConstraint(oldGD),
														m_vBeads(oldGD.m_vBeads),
														m_XForce(oldGD.m_XForce),
														m_YForce(oldGD.m_YForce),
														m_ZForce(oldGD.m_ZForce)									   
{
}


CGravityData& CGravityData::operator=(const CGravityData& rGD)
{
	if(this != &rGD)
	{
		m_vBeads		= rGD.m_vBeads;
		m_XForce		= rGD.m_XForce;
		m_YForce		= rGD.m_YForce;
		m_ZForce		= rGD.m_ZForce;
		m_bDataValid	= rGD.m_bDataValid;
	}

	return *this;
}


double CGravityData::GetXForce() const
{
	return m_XForce;
}

double CGravityData::GetYForce() const
{
	return m_YForce;
}

double CGravityData::GetZForce() const
{
	return m_ZForce;
}

StringSequence CGravityData::GetBeads() const
{
	return m_vBeads;
}
