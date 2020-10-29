/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// tguTriangularLatticeType.cpp: implementation of the tguTriangularLatticeType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "tguTriangularLatticeType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 2d constructor: we set the Z dimension to 1 so that the zero-indexed counters have the value 0 for the 2d lattice

tguTriangularLatticeType::tguTriangularLatticeType(long xmax, long ymax, 
                              double ox, double oy, 
                              double lx, double ly) : tguLatticeVectorType(xmax, ymax, 1, ox, oy, 0.0, lx, ly, 0.0)
{
    // Set the triangular lattice's non-zero unit vector components that 
    // are stored in the base class; this is ok because the base class is 
    // initialised first.

    m_e1[0] = 1.0;
    m_e2[0] = 0.5;
    m_e2[1] = 0.8660;

}

// 3d constructor

tguTriangularLatticeType::tguTriangularLatticeType(long xmax, long ymax, long zmax,
                              double ox, double oy, double oz,
                              double lx, double ly, double lz) : tguLatticeVectorType(xmax, ymax, zmax, ox, oy, oz, lx, ly, lz)
{
    m_e1[0] = 1.0;
    m_e2[0] = 0.5;
    m_e2[1] = 0.8660;
    m_e3[2] = 1.0;
}

tguTriangularLatticeType::~tguTriangularLatticeType()
{
}
