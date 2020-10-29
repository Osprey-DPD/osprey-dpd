/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taLabelDecorator.cpp: implementation of the taLabelDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taLabelDecorator.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. The wrapped object is stored in the m_pInnerDecorator member variable 
// of the CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().

taLabelDecorator::taLabelDecorator(const zString label, CCommandTargetNode* const pDec) : CCommandTargetNode(label, pDec)

{
    pDec->SetOuterDecorator(this);
}

taLabelDecorator::~taLabelDecorator()
{

}

// Forwarding functions to the enclosed concrete CCommandTargetNode instance.

bool taLabelDecorator::IsComposite() const
{
	return GetInnerDecorator()->IsComposite();
}

bool taLabelDecorator::IsBeadTypeInTarget(long type) const
{
	return GetInnerDecorator()->IsBeadTypeInTarget(type);
}

long taLabelDecorator::GetBeadTotalForType(long type) const
{
	return GetInnerDecorator()->GetBeadTotalForType(type);
}

long taLabelDecorator::GetBeadTotal() const
{
	return GetInnerDecorator()->GetBeadTotal();
}

BeadVector taLabelDecorator::GetBeads() const
{
	return GetInnerDecorator()->GetBeads();
}

long taLabelDecorator::GetCurrentBeadType() const
{
	return GetInnerDecorator()->GetCurrentBeadType();
}

long taLabelDecorator::GetPreviousBeadType(long newType) const
{
	return GetInnerDecorator()->GetPreviousBeadType(newType);
}

long taLabelDecorator::GetOriginalBeadType() const
{
	return GetInnerDecorator()->GetOriginalBeadType();
}

bool taLabelDecorator::AddBeadTypePair(long newType, long oldType)
{
	return GetInnerDecorator()->AddBeadTypePair(newType, oldType);
}

bool taLabelDecorator::RemoveBeadTypePair(long newType)
{
	return GetInnerDecorator()->RemoveBeadTypePair(newType);
}

bool taLabelDecorator::ClearBeadTypePairs()
{
	return GetInnerDecorator()->ClearBeadTypePairs();
}

// Functions implemented by CCommandTargetComposite

long taLabelDecorator::GetTargetTotal() const
{
	return GetInnerDecorator()->GetTargetTotal();
}

bool taLabelDecorator::AddTarget(CCommandTargetNode *const pTarget)
{
	return GetInnerDecorator()->AddTarget(pTarget);
}

bool taLabelDecorator::RemoveTarget(const zString label)
{
	return GetInnerDecorator()->RemoveTarget(label);
}

CCommandTargetNode* taLabelDecorator::GetTarget(const zString label) const
{
	return GetInnerDecorator()->GetTarget(label);
}
