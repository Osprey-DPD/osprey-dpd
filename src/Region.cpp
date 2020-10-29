/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Region.cpp: implementation of the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Region.h"
#include "AbstractBead.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We set the data on the region to zero except for the id which must be specified
// when creating a CRegion-derived object.

CRegion::CRegion(long id) : m_id(id), m_Width(0.0), m_Depth(0.0), m_Height(0.0), m_Volume(0.0)
{
	m_lBeads.clear();
}

// Constructor for use when the size of the region is known

CRegion::CRegion(long id, double width, double depth, double height) : m_id(id), m_Width(width), m_Depth(depth), m_Height(height), m_Volume(width*depth*height)
{
	m_lBeads.clear();
}

// Copy constructor

CRegion::CRegion(const CRegion& oldRegion)
{
	m_id		= oldRegion.m_id;
	m_Width     = oldRegion.m_Width;
	m_Depth     = oldRegion.m_Depth;
	m_Height    = oldRegion.m_Height;
	m_Volume    = oldRegion.m_Volume;
	m_lBeads    = oldRegion.m_lBeads;
}

CRegion::~CRegion()
{
}
