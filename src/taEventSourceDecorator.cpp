/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taEventSourceDecorator.cpp: implementation of the taEventSourceDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taEventSourceDecorator.h"
#include "taEventAnalysisDecorator.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. The wrapped object is stored in the m_pInnerDecorator member variable 
// of the CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().


taEventSourceDecorator::taEventSourceDecorator(const zString label, CCommandTargetNode* const pDec) : CCommandTargetNode(label, pDec),
                                                m_SuccessTime(0), m_FailureTime(0)

{
    pDec->SetOuterDecorator(this);

    // Ensure the container of analysis decorators is initially empty

    m_mEventAnalysisFromType.clear();
}

taEventSourceDecorator::~taEventSourceDecorator()
{

}

// Forwarding functions to the enclosed concrete CCommandTargetNode instance.

bool taEventSourceDecorator::IsComposite() const
{
	return GetInnerDecorator()->IsComposite();
}

bool taEventSourceDecorator::IsBeadTypeInTarget(long type) const
{
	return GetInnerDecorator()->IsBeadTypeInTarget(type);
}

long taEventSourceDecorator::GetBeadTotalForType(long type) const
{
	return GetInnerDecorator()->GetBeadTotalForType(type);
}

long taEventSourceDecorator::GetBeadTotal() const
{
	return GetInnerDecorator()->GetBeadTotal();
}

BeadVector taEventSourceDecorator::GetBeads() const
{
	return GetInnerDecorator()->GetBeads();
}

long taEventSourceDecorator::GetCurrentBeadType() const
{
	return GetInnerDecorator()->GetCurrentBeadType();
}

long taEventSourceDecorator::GetPreviousBeadType(long newType) const
{
	return GetInnerDecorator()->GetPreviousBeadType(newType);
}

long taEventSourceDecorator::GetOriginalBeadType() const
{
	return GetInnerDecorator()->GetOriginalBeadType();
}

bool taEventSourceDecorator::AddBeadTypePair(long newType, long oldType)
{
	return GetInnerDecorator()->AddBeadTypePair(newType, oldType);
}

bool taEventSourceDecorator::RemoveBeadTypePair(long newType)
{
	return GetInnerDecorator()->RemoveBeadTypePair(newType);
}

bool taEventSourceDecorator::ClearBeadTypePairs()
{
	return GetInnerDecorator()->ClearBeadTypePairs();
}

// Functions implemented by CCommandTargetComposite

long taEventSourceDecorator::GetTargetTotal() const
{
	return GetInnerDecorator()->GetTargetTotal();
}

bool taEventSourceDecorator::AddTarget(CCommandTargetNode *const pTarget)
{
	return GetInnerDecorator()->AddTarget(pTarget);
}

bool taEventSourceDecorator::RemoveTarget(const zString label)
{
	return GetInnerDecorator()->RemoveTarget(label);
}

CCommandTargetNode* taEventSourceDecorator::GetTarget(const zString label) const
{
	return GetInnerDecorator()->GetTarget(label);
}

// ****************************************
// IEventSourceDecorator interface
//
// Implementation of the generic ESD interface that allows any EAD to register
// itself with an ESD and receive a single data stream: the time at which the
// specified event type instances succeed.
// Each EAD registers itself using its corresponding ESD's unique name as the key.


bool taEventSourceDecorator::AddEventAnalysis(const zString key, taEventAnalysisDecorator* const pAnalysis)
{
    if(pAnalysis)
    {
        // Store an interface pointer to the ESD using the IEAD-derived classes.
        // Each class derived from taEventAnalysisDecorator knows what interface
        // it requires and creates it using its ConnectESDInterface() function.

        pAnalysis->ConnectESDInterface(this);

//        std::cout <<"Size of event analysis map before addition " << m_mEventAnalysisFromType.size() << zEndl;
        m_mEventAnalysisFromType.insert(zPairSEAD(key, pAnalysis));
//        std::cout <<"Size of event analysis map after addition " << m_mEventAnalysisFromType.size() << zEndl;
        return true;
    }
    else
    {
        return false;
    }
}

// Function to remove all analysis decorator instances that correspond to a given
// key from the source decorator instance. If no instances are found we return false.
// Because there may be many instances of the same ESD class attached to different
// event types, we use the ESDs' unique names as the key and not their class names.

bool taEventSourceDecorator::RemoveEventAnalysis(const zString sourceType)
{
    StringEventAnalysisIterator iterEAD = m_mEventAnalysisFromType.find(sourceType);

    if(iterEAD != m_mEventAnalysisFromType.end())
    {
//        std::cout <<"Size of event analysis map before erase " << m_mEventAnalysisFromType.size() << zEndl;
        m_mEventAnalysisFromType.erase(sourceType);
//        std::cout <<"Size of event analysis map after erase " << m_mEventAnalysisFromType.size() << zEndl;
        return true;
    }
    else
    {
        return false;
    }
}

// Function to return the last simulation time at which the event succeeded.

long taEventSourceDecorator::GetSuccessTime() const
{
    return m_SuccessTime;
}

// Function to return the last simulation time at which the event failed.

long taEventSourceDecorator::GetFailureTime() const
{
    return m_FailureTime;
}

// Protected function to store the last simulation time at which the event succeeded.

void taEventSourceDecorator::SetSuccessTime(long time)
{
    if(time > 0)
    {
        m_SuccessTime = time;
    }
    else
    {
        m_SuccessTime = 0;
    }
}

// Protected function to store the last simulation time at which the event failed.

void taEventSourceDecorator::SetFailureTime(long time)
{
    if(time > 0)
    {
        m_FailureTime = time;
    }
    else
    {
        m_FailureTime = 0;
    }
}
