/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondOffRate.cpp: implementation of the aeBondOffRate class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeBondOffRate.h"
#include "aeActiveBond.h"
#include "CNTCell.h"  // Needed for the RNG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to set the detachment probabilities for both head and tail
// to the same value. The bond off-rate is determined by a fixed probability.
// If a random number drawn from the uniform distribution [0,1]
// is less than the specified probability the bond is broken.
// An invalid value results in a zero probability being stored.


aeBondOffRate::aeBondOffRate(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_HeadProbability = prob;
        m_TailProbability = prob;
    }
    else
    {
        m_HeadProbability = 0.0;
        m_TailProbability = 0.0;
    }
}

// Constructor to set the detachment probabilities for head and tail
// to different values.

aeBondOffRate::aeBondOffRate(double headProb, double tailProb)
{
    if(headProb >= 0.0 && headProb <= 1.0)
    {
        m_HeadProbability = headProb;
    }
    else
    {
        m_HeadProbability = 0.0;
    }

    if(tailProb >= 0.0 && tailProb <= 1.0)
    {
        m_TailProbability = tailProb;
    }
        else
    {
        m_TailProbability = 0.0;
    }
}

aeBondOffRate::~aeBondOffRate()
{

}

// Function to check whether an active bond at the tail end of a polymer 
// can dissociate from the adjacent bond connected to its head. We draw 
// a random number from the uniform distribution [0,1] and compare it to
// the stored probability. If it is less than the stored breakage probability
// the bond is broken.
//
// Note that the "Head" here refers to the bond's head not the polymer's head.
// This function is called when the monomer at the TAIL end of a polymer
// is attempting to detach.

bool aeBondOffRate::ActivateHead(const aeActiveBond* const pEnd) const
{
    if(CCNTCell::GetRandomNo() < m_HeadProbability)
        return true;
    else
        return false;
}

// Function to check whether an active bond at the head end of a polymer 
// can dissociate from the adjacent bond connected to its tail.We use the
// same scheme as for ActivateHead() above.
//
// Note that the "Tail" here refers to the bond's tail not the polymer's tail.
// This function is called when the monomer at the HEAD end of a polymer
// is attempting to detach.

bool aeBondOffRate::ActivateTail(const aeActiveBond* const pEnd) const
{
    if(CCNTCell::GetRandomNo() < m_TailProbability)
        return true;
    else
        return false;
}

// Function to set the probability of a monomer detaching from its neighbour
// via its head connection. An invalid value leaves the probability unchanged.

void aeBondOffRate::SetHeadProbability(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_HeadProbability = prob;
    }
}

// Function to set the probability of a monomer detaching from its neighbour
// via its tail connection. An invalid value leaves the probability unchanged.

void aeBondOffRate::SetTailProbability(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_TailProbability = prob;
    }
}

