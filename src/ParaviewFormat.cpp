/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ParaviewFormat.cpp: implementation of the CParaviewFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ParaviewFormat.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the file extension for this 
// visualisation format class.

const zString CParaviewFormat::m_FileExtension = ".vtk";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParaviewFormat::CParaviewFormat(double lx, double ly, double lz, bool bDisplayBox,
						   const long beadTypeTotal) : CCurrentStateFormat(lx, ly, lz, bDisplayBox, beadTypeTotal)
{
    m_vBeadTypes.clear();
}

CParaviewFormat::~CParaviewFormat()
{

}

// Function used to write out header data for the current format class.
// the output stream object is passed in from the CCurrentState object
// that uses the format class
//
// We are using the vtk format for Paraview, and the following data are
// required:
//
//  # vtk DataFile Version 2.0
//  data
//  ASCII
//  DATASET POLYDATA
//  POINTS N double
//  0.0  0.0  0.0
//  ..... N lines of data here
//  VERTICES  2 4
//  1 0
//  1 1
//  POINT_DATA  2
//  SCALARS  ptype float 1
//  LOOKUP_TABLE default
//  0
//  1
//  VECTORS  orientation double   // This section is not used yet.
//  0.0  0.0  0.0
//  1.0  1.0  1.0

void CParaviewFormat::SerializeHeader(zOutStream& os, const long beadTotal)
{
	os << "# vtk DataFile Version 2.0			    " << zEndl;
	os << "data										" << zEndl;
	os << "ASCII									" << zEndl;
	os << "DATASET POLYDATA						    " << zEndl;
	os << "POINTS " << beadTotal << " double		" << zEndl;
}

// Second function that can be invoked after the bead data has been written.

void CParaviewFormat::SerializeFooter(zOutStream& os, const long beadTotal)
{
	os << "VERTICES  " << beadTotal	<< " " << 2*beadTotal  << zEndl;

    long i;
    for(i=0; i<beadTotal; i++)
    {
        os << "1 " << i << zEndl;
    }
    os << "POINT_DATA " << beadTotal << zEndl;
    os << "SCALARS ptype float 1"    << zEndl;
    os << "LOOKUP_TABLE  default"    << zEndl;

    for(i=0; i<beadTotal; i++)
    {
        os << m_vBeadTypes.at(i) << zEndl;
    }
}

// Function to write out the coordinates of a single bead. The bead is drawn
// as a sphere with a given radius and centre coordinates. Because the vtk format
// requires the bead types to be written in a different place than the coordinates,
// we store all displayIds here but write them out in the footer function.

void CParaviewFormat::SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								  const double x, const double y, const double z)
{
    m_vBeadTypes.push_back(type);
	os << x << " " << y << " " << z << zEndl;
}

