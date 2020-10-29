/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxParallelBase.cpp: implementation of the xxParallelBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxParallelBase.h"

// ****************************************
// Global members to return information about a parallel experiment

long   xxParallelBase::m_World = 0;
long   xxParallelBase::m_Rank  = 0;
double xxParallelBase::m_SimBoxXOrigin  = 0.0;
double xxParallelBase::m_SimBoxYOrigin  = 0.0;
double xxParallelBase::m_SimBoxZOrigin  = 0.0;

long xxParallelBase::GlobalGetWorld()
{
    return xxParallelBase::m_World;
}

long xxParallelBase::GlobalGetRank()
{
    return xxParallelBase::m_Rank;
}

double xxParallelBase::GlobalGetSimBoxXOrigin()
{
    return xxParallelBase::m_SimBoxXOrigin;
}

double xxParallelBase::GlobalGetSimBoxYOrigin()
{
    return xxParallelBase::m_SimBoxYOrigin;
}

double xxParallelBase::GlobalGetSimBoxZOrigin()
{
    return xxParallelBase::m_SimBoxZOrigin;
}

// Boolean function to indicate if the experiment is parallel or not.

bool xxParallelBase::GlobalIsParallel()
{
    return (xxParallelBase::m_World > 1);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We define some member variables here that can be used by derived classes
// in looping over processors. This avoids every class having to redefine its
// own counters.

xxParallelBase::xxParallelBase() : m_ProcId(0), m_ReceiverRank(0), m_SenderRank(0)
{
   
}

// Copy constructor

xxParallelBase::xxParallelBase(const xxParallelBase& oldBase) : xxBase(oldBase), m_ProcId(oldBase.m_ProcId), m_ReceiverRank(oldBase.m_ReceiverRank), m_SenderRank(oldBase.m_SenderRank)
{
   
}

xxParallelBase::~xxParallelBase()
{

}

// Boolean member function to show if the experiment is parallel or not.

bool xxParallelBase::IsParallel() const
{
    return (xxParallelBase::m_World > 1);
}

// Member function that returns the number of processors.

long xxParallelBase::GetWorld() const
{
    return xxParallelBase::m_World;
}

// Member function that returns the rank of the current instance.

long xxParallelBase::GetRank() const
{
    return xxParallelBase::m_Rank;
}

// Boolean function to allow an instance to determine if it is process 0 in order
// to carry out P0 tasks.

bool xxParallelBase::IsProcessZero() const
{
    return (xxParallelBase::m_Rank == 0);
}

// Functions to return the coordinates of the owning processor's SimBox origin
// in the whole simulation Space.

double xxParallelBase::GetSimBoxXOrigin() const
{
    return xxParallelBase::m_SimBoxXOrigin;
}

double xxParallelBase::GetSimBoxYOrigin() const
{
    return xxParallelBase::m_SimBoxYOrigin;
}

double xxParallelBase::GetSimBoxZOrigin() const
{
    return xxParallelBase::m_SimBoxZOrigin;
}

// Function to store the procId of the processor that creates this message. This is not needed by all messages, but is sometimes useful.

void xxParallelBase::SetSenderRank(long pid)
{
	if(pid >= 0 && pid < GetWorld())
	{
		m_SenderRank = pid;
	}
	else
	{
		m_SenderRank = -1; // error value will be trapped by Validate() 
	}
}




