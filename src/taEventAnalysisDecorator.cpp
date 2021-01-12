/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taEventAnalysisDecorator.cpp: implementation of the taEventAnalysisDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taEventAnalysisDecorator.h"
#include "IEAD.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. The wrapped object is stored in the m_pInnerDecorator member variable 
// of the CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().

taEventAnalysisDecorator::taEventAnalysisDecorator(const zString label, CCommandTargetNode* const pDec, 
                                                   long samples) : CCommandTargetNode(label, pDec),
                                                   m_pIEAD(0), m_SamplesPerAnalysis(samples), m_Counter(0)
                                                   
{
    pDec->SetOuterDecorator(this);

}

taEventAnalysisDecorator::~taEventAnalysisDecorator()
{
    // Destroy the enclosed interface pointer if it exists

    if(m_pIEAD)
    {
        delete m_pIEAD;
        m_pIEAD = 0;
    }
}

// Forwarding functions to the enclosed concrete CCommandTargetNode instance.

bool taEventAnalysisDecorator::IsComposite() const
{
	return GetInnerDecorator()->IsComposite();
}

bool taEventAnalysisDecorator::IsBeadTypeInTarget(long type) const
{
	return GetInnerDecorator()->IsBeadTypeInTarget(type);
}

long taEventAnalysisDecorator::GetBeadTotalForType(long type) const
{
	return GetInnerDecorator()->GetBeadTotalForType(type);
}

long taEventAnalysisDecorator::GetBeadTotal() const
{
	return GetInnerDecorator()->GetBeadTotal();
}

BeadVector taEventAnalysisDecorator::GetBeads() const
{
	return GetInnerDecorator()->GetBeads();
}

long taEventAnalysisDecorator::GetCurrentBeadType() const
{
	return GetInnerDecorator()->GetCurrentBeadType();
}

long taEventAnalysisDecorator::GetPreviousBeadType(long newType) const
{
	return GetInnerDecorator()->GetPreviousBeadType(newType);
}

long taEventAnalysisDecorator::GetOriginalBeadType() const
{
	return GetInnerDecorator()->GetOriginalBeadType();
}

bool taEventAnalysisDecorator::AddBeadTypePair(long newType, long oldType)
{
	return GetInnerDecorator()->AddBeadTypePair(newType, oldType);
}

bool taEventAnalysisDecorator::RemoveBeadTypePair(long newType)
{
	return GetInnerDecorator()->RemoveBeadTypePair(newType);
}

bool taEventAnalysisDecorator::ClearBeadTypePairs()
{
	return GetInnerDecorator()->ClearBeadTypePairs();
}

// Functions implemented by CCommandTargetComposite

long taEventAnalysisDecorator::GetTargetTotal() const
{
	return GetInnerDecorator()->GetTargetTotal();
}

bool taEventAnalysisDecorator::AddTarget(CCommandTargetNode *const pTarget)
{
	return GetInnerDecorator()->AddTarget(pTarget);
}

bool taEventAnalysisDecorator::RemoveTarget(const zString label)
{
	return GetInnerDecorator()->RemoveTarget(label);
}

CCommandTargetNode* taEventAnalysisDecorator::GetTarget(const zString label) const
{
	return GetInnerDecorator()->GetTarget(label);
}

// Protected function used by derived classes to control how many samples they
// take before analysing their data.

void taEventAnalysisDecorator::IncrementCounter()
{
    m_Counter++;
}

// Protected function used by derived classes to control how many samples they
// take before analysing their data.

bool taEventAnalysisDecorator::IsTimeToNormalise()
{
    if(m_Counter == m_SamplesPerAnalysis)
    {
        m_Counter = 0;
        return true;
    }
    else
    {
        return false;
    }
}

