/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taForceDecorator.cpp: implementation of the taForceDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taForceDecorator.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class.

taForceDecorator::taForceDecorator(const zString label, CCommandTargetNode* const pDec) : CCommandTargetNode(label, pDec), m_pBead(0)

{

}

// Constructor that is used when reading an instance from a file. We pass the
// decorator's label to the base class, and all remaining data must be set in 
// the Read() function.

taForceDecorator::taForceDecorator(const zString label) : CCommandTargetNode(label), m_pBead(0)
{

}

taForceDecorator::~taForceDecorator()
{

}

// Forwarding functions to the enclosed concrete CCommandTargetNode instance.

bool taForceDecorator::IsComposite() const
{
	return GetInnerDecorator()->IsComposite();
}

bool taForceDecorator::IsBeadTypeInTarget(long type) const
{
	return GetInnerDecorator()->IsBeadTypeInTarget(type);
}

long taForceDecorator::GetBeadTotalForType(long type) const
{
	return GetInnerDecorator()->GetBeadTotalForType(type);
}

long taForceDecorator::GetBeadTotal() const
{
	return GetInnerDecorator()->GetBeadTotal();
}

BeadVector taForceDecorator::GetBeads() const
{
	return GetInnerDecorator()->GetBeads();
}

long taForceDecorator::GetCurrentBeadType() const
{
	return GetInnerDecorator()->GetCurrentBeadType();
}

long taForceDecorator::GetPreviousBeadType(long newType) const
{
	return GetInnerDecorator()->GetPreviousBeadType(newType);
}

long taForceDecorator::GetOriginalBeadType() const
{
	return GetInnerDecorator()->GetOriginalBeadType();
}

bool taForceDecorator::AddBeadTypePair(long newType, long oldType)
{
	return GetInnerDecorator()->AddBeadTypePair(newType, oldType);
}

bool taForceDecorator::RemoveBeadTypePair(long newType)
{
	return GetInnerDecorator()->RemoveBeadTypePair(newType);
}

bool taForceDecorator::ClearBeadTypePairs()
{
	return GetInnerDecorator()->ClearBeadTypePairs();
}

// Functions implemented by CCommandTargetComposite

long taForceDecorator::GetTargetTotal() const
{
	return GetInnerDecorator()->GetTargetTotal();
}

bool taForceDecorator::AddTarget(CCommandTargetNode *const pTarget)
{
	return GetInnerDecorator()->AddTarget(pTarget);
}

bool taForceDecorator::RemoveTarget(const zString label)
{
	return GetInnerDecorator()->RemoveTarget(label);
}

CCommandTargetNode* taForceDecorator::GetTarget(const zString label) const
{
	return GetInnerDecorator()->GetTarget(label);
}

// Function that stores a bead pointer allowing derived classes to calculate
// forces that depend on the bead's coordinates. It is used, for example, by
// taRadialForce. We do not take ownership of the bead pointer here so it
// must NOT be destroyed from within this class.

void taForceDecorator::SetBeadPointer(const CBead *const pBead)
{
	m_pBead = pBead;
}
