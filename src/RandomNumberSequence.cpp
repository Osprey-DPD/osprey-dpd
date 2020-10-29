/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RandomNumberSequence.cpp: implementation of the CRandomNumberSequence class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RandomNumberSequence.h"


// **********************************************************************
// Global Functions and members
//
bool CRandomNumberSequence::m_bReadOnceOnly = true;  // Read file in once by default

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandomNumberSequence::CRandomNumberSequence(const zString fileName, long sequenceSize) : CFileReader(fileName),
                                             m_bReadFileAgain(true),
                                             m_SequenceSize(sequenceSize),
                                             m_Pointer(0)
{
    m_Numbers.clear();
}


CRandomNumberSequence::~CRandomNumberSequence()
{

}

// Function to read/write the bead coordinate data and other information from a 
// previously-saved CRandomNumberSequence. 

bool CRandomNumberSequence::Serialize()
{
    double x;

    long n = 0;
    bool bFileState = true;

    if(m_bReadFileAgain)
    {
        if(m_bReadOnceOnly)
        {
            m_bReadFileAgain = false;  // Prevent another reading of the file
        }

        while(n < m_SequenceSize && bFileState)
        {
            n++;
		    m_inStream >> x;

	        if(m_inStream.good())
            {
                m_Numbers.push_back(x);
            }
            else
            {
                bFileState = false;
            }
        }
    }

	return bFileState;
}

// Function to set the pointer to the next number that will be retrieved to the
// beginning of the sequence.

void CRandomNumberSequence::ResetPointer()
{
    m_Pointer = 0;
}

// Function to return the first number in the sequence and reset the pointer

double CRandomNumberSequence::GetFirstNumber()
{
    ResetPointer();
    return m_Numbers.at(0);
}

// Function to return the next number in the sequence and increment the pointer.
// If we reach the end of the sequence, we return -1. As this should never be
// in the sequence of numbers, we can check for it in the calling routine.

double CRandomNumberSequence::GetNextNumber()
{
    if(m_Pointer < m_Numbers.size())
    {
        return m_Numbers.at(m_Pointer++);
    }
    else
    {
        return -1;
    }        
}
