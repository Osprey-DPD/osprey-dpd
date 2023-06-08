/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// PovrayFormat.cpp: implementation of the CPovrayFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "PovrayFormat.h"

#include "Bond.h"
#include "Polymer.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the file extension for this 
// visualisation format class.

const zString CPovrayFormat::m_FileExtension = ".pov";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPovrayFormat::CPovrayFormat(double lx, double ly, double lz, bool bDisplayBox,
							const long beadTypeTotal,
							const double camera[3], const double target[3],
							zDoubleVector vLightX, 
							zDoubleVector vLightY, 
							zDoubleVector vLightZ,
							bool showBonds
							) : CCurrentStateFormat(lx, ly, lz, bDisplayBox, beadTypeTotal), 
													 m_vLightX(vLightX),
													 m_vLightY(vLightY),
													 m_vLightZ(vLightZ),
													 m_ShowBonds(showBonds)
{
	// Set the default data needed to save CCurrentState data in PovRay format.
	// This requires the camera and target coordinates to be specified 
	// in absolute coordinates. The commands specify the coordinates as multiples 
	// of the SimBox dimensions (for ease of changing SimBox size without
	// having to change the camera coordinates), and these are converted into 
	// absolute coordinates in CMonitor and in the command handlers, 
	// SetCurrentStateCamera() and SetCurrentStateLight().  We hold the 
	// coordinates of all SetCurrentStateLight() commands received so that 
	// we can implement removal of lights later.

	m_Camera[0]	= camera[0];
	m_Camera[1]	= camera[1];
	m_Camera[2]	= camera[2];

	m_Target[0]	=  target[0];
	m_Target[1]	=  target[1];
	m_Target[2]	=  target[2];
}

CPovrayFormat::~CPovrayFormat()
{

}

// Function used to write out header data for the current format class.
// the output stream object is passed in from the CCurrentState object
// that uses the format class
//
// Here we set up the camera and light coordinates.

void CPovrayFormat::SerializeHeader(zOutStream& os, const long beadTotal)
{
	os << "#include \"colors.inc\"					" << zEndl;
	os << "#include \"stones.inc\"					" << zEndl;
	os << "background {color White}				    " << zEndl;

	os << "camera{ location < ";
	os << m_Camera[0] << ", ";
	os << m_Camera[1] << ", ";
	os << m_Camera[2] << " >" << zEndl;

	os << "        look_at  < ";
	os << m_Target[0] << ", ";
	os << m_Target[1] << ", ";
	os << m_Target[2] << " >	}" << zEndl << zEndl;

	for(long unsigned int lno = 0; lno < m_vLightX.size(); lno++)
	{
		os << "light_source{ < ";
		os << m_vLightX.at(lno) << ", ";
		os << m_vLightY.at(lno) << ", ";
		os << m_vLightZ.at(lno) << " > color White shadowless	" << zEndl;
		os << "            }									" << zEndl << zEndl;
	}

	if(m_bDisplayBox)
	{
		os << "box{ < ";
		os <<  m_BLBoxCoord[0] << ","; 
		os <<  m_BLBoxCoord[1] << ",";
		os <<  m_BLBoxCoord[2] << " > < ";
		os <<  m_TRBoxCoord[0] << ",";
		os <<  m_TRBoxCoord[1] << ",";
		os <<  m_TRBoxCoord[2] << " >" << zEndl;
		os << "     texture{ pigment{ color rgbf < 0.9,0.9,0.9,0.9 > } } }" << zEndl;
	}

	if(m_ShowBonds){
		os << "#declare BeadRadiusScale = 0.4; " << zEndl;
		os << "#declare BondStickColour = Black; " << zEndl;
		os << "#declare BondStickRadius = 0.09; " << zEndl;
	}

	os << zEndl << zEndl;	
}

// Function to write out the coordinates of a single bead. The bead is drawn
// as a sphere with a given radius and centre coordinates.

void CPovrayFormat::SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								  const double x, const double y, const double z)
{
	std::string r=std::to_string(radius);
	if(m_ShowBonds){
		r = "BeadRadiusScale * "+r;
	}

	os << "sphere { " << "< "	<< x << ", "
								<< y << ", "
								<< z << " >, "
								<< r
							    << " texture { pigment {color " << GetBeadColourFromType(type);
	os << "} } }" << zEndl;

}


void CPovrayFormat::SerializeBond(zOutStream& os, const CPolymer &polymer, const CBond &bond)
{
	const auto &head=*bond.GetHead();
	const auto &tail=*bond.GetTail();

	double hx=head.GetXPos(), hy=head.GetYPos(), hz=head.GetZPos();
	double tx=tail.GetXPos(), ty=tail.GetYPos(), tz=tail.GetZPos();
	double dx[3]={ hx-tx, hy-ty, hz-tz };
	double lends[3] = { m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength };

	bool wrapped=false;
	for(int d=0; d<3; d++){
		if(dx[d] < -lends[d]/2){
			dx[d] += lends[d];
			wrapped=true;
		}else if(dx[d] > lends[d]/2){
			dx[d] -= lends[d];
			wrapped=true;
		}
	}

	if(!wrapped){
		os << "cylinder { ";
		os << " < "<< hx << " " << hy << " " << hz << " >, ";
		os << " < "<< tx << " " << ty << " " << tz << " >, ";
		os << "BondStickRadius open texture { pigment {color BondStickColour } } ";
		os << " } " << zEndl;
	}else{
		// This is an attempt to have little stalks going off the edge.
		// However, it results in cylinders sticking vertically up through z,
		// as well as lots of little stalks
		/*os << "cylinder { ";
		os << " < "<< hx << " " << hy << " " << hz << " >, ";
		os << " < "<< hx-dx[0] << " " << hy-dx[1] << " " << hz-dx[2] << " >, ";
		os << "BondStickRadius open texture { pigment {color BondStickColour } } ";
		os << " } " << zEndl;

		os << "cylinder { ";
		os << " < "<< tx+dx[0] << " " << ty+dx[1] << " " << tz+dx[2] << " >, ";
		os << " < "<< tx << " " << ty << " " << tz << " >, ";
		os << "BondStickRadius open texture { pigment {color BondStickColour } } ";
		os << " } " << zEndl;
		*/
	}
}
