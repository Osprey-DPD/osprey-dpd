/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SolventFreeFormat.cpp: implementation of the CSolventFreeFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SolventFreeFormat.h"
#include "ISimBox.h"
#include "Polymer.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the file extension for this 
// visualisation format class.

const zString CSolventFreeFormat::m_FileExtension = ".rst";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// We store a pointer to the ISimBox so we can access all the bead and polymer information needed.

CSolventFreeFormat::CSolventFreeFormat(double lx, double ly, double lz, bool bDisplayBox,
                                       const long beadTypeTotal, const ISimBox* pISimBox) : CCurrentStateFormat(lx, ly, lz, bDisplayBox, beadTypeTotal), m_pISimBox(pISimBox)
{
}

CSolventFreeFormat::~CSolventFreeFormat()
{

}

// Function used to write out header data for the current format class.
// The output stream object is passed in from the CCurrentState object that uses the format class.
//
// Because the standard CCurrentState serialisation function does not allow me to get polymer data from a bead pointer, I write out
// all the bead data in the header and ignore the loop in the SerializeBead function. This is inefficient as the current state object
// will still iterate over all beads, but I cannot alter this without changing the CCurrentState class or creating a new type of
// xxState-derived object.
void CSolventFreeFormat::SerializeHeader(zOutStream& os, const long beadTotal)
{
    for(cPolymerVectorIterator iterPoly=m_pISimBox->GetPolymers().begin(); iterPoly!=m_pISimBox->GetPolymers().end(); iterPoly++ )
    {
        for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
            {
                if((*iterBead)->GetVisible())
            {
                os << (*iterPoly)->GetId()			<< " " << (*iterPoly)->GetType()		<< " ";
                os << (*iterBead)->GetId()			<< " " << (*iterBead)->GetType()		<< " ";
                os << (*iterBead)->GetRadius()		<< " ";
                os << (*iterBead)->GetXPos()		<< " " << (*iterBead)->GetYPos()		<< " " << (*iterBead)->GetZPos()        << " ";
                os << (*iterBead)->GetunPBCXPos()   << " " << (*iterBead)->GetunPBCYPos()   << " " << (*iterBead)->GetunPBCZPos()   << " ";
                os << (*iterBead)->GetXMom()		<< " " << (*iterBead)->GetYMom()		<< " " << (*iterBead)->GetZMom()		<< " " << zEndl;
            }
        }
    }
}

// Function to write out the coordinates of a single bead.

void CSolventFreeFormat::SerializeBead(zOutStream& os, const zString name, const long type, const double radius,
								  const double x, const double y, const double z)
{
}
