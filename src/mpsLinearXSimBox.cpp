/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsLinearXSimBox.cpp: implementation of the mpsLinearXSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpsLinearXSimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This class represents the parallel SimBox for processors with a
// linear arrangement in Space for which only the X dimension contains
// more than one processor. This means that in the Y, Z dimensions each 
// processor only has to communicate with itself.

mpsLinearXSimBox::mpsLinearXSimBox(long px, long py, long pz, long lx, long ly, long lz,
                                   double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
								   PolymerVector& rvBulkPolymers) : mpsSimBox(1, 1, px, py, pz, lx, ly, lz, dx, dy, dz, rvCNTCells, rvBulkPolymers)
{

}

// Copy constructor

mpsLinearXSimBox::mpsLinearXSimBox(const mpsLinearXSimBox& oldSimBox) : mpsSimBox(oldSimBox)
{

}


mpsLinearXSimBox::~mpsLinearXSimBox()
{

}

void mpsLinearXSimBox::PartitionSpace()
{

}

void mpsLinearXSimBox::UpdateBorderPos()
{

}

void mpsLinearXSimBox::UpdateBorderForce()
{

}




