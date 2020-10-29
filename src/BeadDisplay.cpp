/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BeadDisplay.cpp: implementation of the CBeadDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BeadDisplay.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor to manually store the position coordinates and identity of a bead.

CBeadDisplay::CBeadDisplay(long id, long type, 
                           double x, double y, double z) : m_id(id), m_Type(type), 
                           m_X(x), m_Y(y), m_Z(z)
{

}

// Constructor to retrieve bead data and automatically store it in an instance.
// Note that we do NOT keep the bead pointer in this class, only some of its data,
// so there is nothing to delete in this class' destructor.

CBeadDisplay::CBeadDisplay(const CBead* const pBead) : m_id(pBead->GetId()), 
                                                       m_Type(pBead->GetType()),
                                                       m_X(pBead->GetXPos()),
                                                       m_Y(pBead->GetYPos()),
                                                       m_Z(pBead->GetZPos())
{

}


// Copy constructor

CBeadDisplay::CBeadDisplay(const CBeadDisplay& oldBead) : m_id(oldBead.m_id), 
                                                          m_Type(oldBead.m_Type), 
                                                          m_X(oldBead.m_X), 
                                                          m_Y(oldBead.m_Y), 
                                                          m_Z(oldBead.m_Z) 
{

}

CBeadDisplay::~CBeadDisplay()
{

}
