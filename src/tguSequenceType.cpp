/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// tguSequenceType.cpp: implementation of the tguSequenceType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "tguSequenceType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Overridden constructors to allow storage of any argument type.

// We store the initial value here but copy the same value to the base class
// for storage as the current value. Subsequently, the current value is
// updated using the increment.

tguSequenceType::tguSequenceType(long initial, long delta) : tguArgumentType(initial),
                                 m_InitialZ(initial), m_InitialR(0.0), m_InitialS(""),
                                 m_DZ(delta), m_DR(0.0),
                                 m_Counter(0)
{

}

tguSequenceType::tguSequenceType(double initial, double delta) : tguArgumentType(initial),
                                 m_InitialZ(0), m_InitialR(initial), m_InitialS(""),
                                 m_DZ(0), m_DR(delta),
                                 m_Counter(0)
{

}


tguSequenceType::tguSequenceType(zString initial) : tguArgumentType(initial),
                                 m_InitialZ(0), m_InitialR(0.0), m_InitialS(initial),
                                 m_DZ(0), m_DR(0.0),
                                 m_Counter(0)
{

}

tguSequenceType::~tguSequenceType()
{
}

// Overidden functions to return the next value in a sequence for each of the 
// possible types. Each function automatically increments the counter; if this is 
// not desired, the base class' GetValue() functions should be used as these 
// just return the current value.
//
// Note that the first value returned by this function is the stored value,
// which is incremented after returning the value.

void tguSequenceType::GetNextValue(long *pValue)
{
    ++m_Counter;
    *pValue = m_Integer;
    m_Integer += m_DZ;
}

// Note that the first value returned by this function is the initial value;
// which is incremented after returning the value.

void tguSequenceType::GetNextValue(double *pValue)
{
    ++m_Counter;
    *pValue = m_Real;
    m_Real += m_DR;
}

// Unlike the integer and real sequences above, the string suffix starts at 1.

void tguSequenceType::GetNextValue(zString *pValue)
{
    ++m_Counter;
    m_String = m_InitialS + ToString(m_Counter);
    *pValue = m_String;
}
