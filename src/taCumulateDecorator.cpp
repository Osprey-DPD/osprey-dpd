/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taCumulateDecorator.cpp: implementation of the taCumulateDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taCumulateDecorator.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class.

taCumulateDecorator::taCumulateDecorator(const zString label, CCommandTargetNode* const pDec,
										 const zString wrappedLabel, long start, long end) : CCommandTargetNode(label, pDec),
										 m_bWrapFailure(false),
										 m_WrappedLabel(wrappedLabel),
										 m_Start(start), m_End(end)
{

}

taCumulateDecorator::~taCumulateDecorator()
{

}

// Forwarding functions to the enclosed concrete CCommandTargetNode instance.

bool taCumulateDecorator::IsComposite() const
{
	return GetInnerDecorator()->IsComposite();
}

bool taCumulateDecorator::IsBeadTypeInTarget(long type) const
{
	return GetInnerDecorator()->IsBeadTypeInTarget(type);
}

long taCumulateDecorator::GetBeadTotalForType(long type) const
{
	return GetInnerDecorator()->GetBeadTotalForType(type);
}

long taCumulateDecorator::GetBeadTotal() const
{
	return GetInnerDecorator()->GetBeadTotal();
}

BeadVector taCumulateDecorator::GetBeads() const
{
	return GetInnerDecorator()->GetBeads();
}

long taCumulateDecorator::GetCurrentBeadType() const
{
	return GetInnerDecorator()->GetCurrentBeadType();
}

long taCumulateDecorator::GetPreviousBeadType(long newType) const
{
	return GetInnerDecorator()->GetPreviousBeadType(newType);
}

long taCumulateDecorator::GetOriginalBeadType() const
{
	return GetInnerDecorator()->GetOriginalBeadType();
}

bool taCumulateDecorator::AddBeadTypePair(long newType, long oldType)
{
	return GetInnerDecorator()->AddBeadTypePair(newType, oldType);
}

bool taCumulateDecorator::RemoveBeadTypePair(long newType)
{
	return GetInnerDecorator()->RemoveBeadTypePair(newType);
}

bool taCumulateDecorator::ClearBeadTypePairs()
{
	return GetInnerDecorator()->ClearBeadTypePairs();
}

// Functions implemented by CCommandTargetComposite

long taCumulateDecorator::GetTargetTotal() const
{
	return GetInnerDecorator()->GetTargetTotal();
}

bool taCumulateDecorator::AddTarget(CCommandTargetNode *const pTarget)
{
	return GetInnerDecorator()->AddTarget(pTarget);
}

bool taCumulateDecorator::RemoveTarget(const zString label)
{
	return GetInnerDecorator()->RemoveTarget(label);
}

CCommandTargetNode* taCumulateDecorator::GetTarget(const zString label) const
{
	return GetInnerDecorator()->GetTarget(label);
}

