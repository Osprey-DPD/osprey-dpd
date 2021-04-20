/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTargetNode.cpp: implementation of the CCommandTargetNode class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CommandTargetNode.h"
#include "xxDecoratorState.h"

// Static member variable holding the number of command targets created.

long CCommandTargetNode::m_CommandTargetTotal = 0;

long CCommandTargetNode::GetCommandTargetTotal()
{
	return m_CommandTargetTotal;
}

// Static member function to reset the number of command targets to zero.

void CCommandTargetNode::ZeroCommandTargetTotal()
{
	m_CommandTargetTotal = 0;
}

// Function to write out the target's data to file. We pass the
// stream output operator to the contained CCommandTargetNode-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//
// We write out the type of target and its unique string label as well as its
// inner and outer decorator labels if they exist. If the target is not wrapped,
// we write out its own label again to avoid having blank fields.
// Particular data for each target class are serialized in the class'
//  put() and get() functions. The endline must only be written in the 
// derived classes not here: even if there is no data needed.

zOutStream& operator<<(zOutStream& os, const CCommandTargetNode& target)
{
    os << zEndl; // Start with a newline
	os << target.GetTargetType() << "  " << target.GetLabel();

    if(target.GetInnerDecorator())
    {
        os << " " << target.GetInnerDecorator()->GetLabel();
    }
    else
    {
        os << " " << target.GetLabel();
    }

    if(target.GetOuterDecorator())
    {
        os << " " << target.GetOuterDecorator()->GetLabel();
    }
    else
    {
        os << " " << target.GetLabel();
    }

	return target.Write(os);
}

// Function to read the target's data from file. Because the type of target
// and its string label have already been read by the initial state so that 
// it can create the appropriate target using the target class factory,
// we only have to read in the inner and outer decorator labels and the
// target-specific data.We call the derived class' Read() function for the latter.
// Note that these labels must be supplied: if the target has no inner or outer
// decorators. then its own label is used instead.

zInStream& operator>>(zInStream& is, CCommandTargetNode& target)
{
    zString innerLabel, outerLabel;

    is >> innerLabel >> outerLabel;

    target.SetInnerDecoratorName(innerLabel);
    target.SetOuterDecoratorName(outerLabel);

	return target.Read(is);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor used by the concrete command target classes that don't know
// about decorators. It is also required by the acfTargetFactory factory class
// to create an instance of the appropriate target type when serialising one
// from file.

CCommandTargetNode::CCommandTargetNode(const zString label) : m_pState(0),
                                                              m_id(++CCommandTargetNode::m_CommandTargetTotal), 
										                      m_Label(label),
                                                              m_InnerDecoratorName(""),
                                                              m_OuterDecoratorName(""),
															  m_pInnerDecorator(0),
															  m_pOuterDecorator(0)
{
    m_ProxyIds.clear();
}

// Constructor used by decorator classes to store a pointer to the inner
// decorator object, or the innermost command target, that is wrapped 
// by the created decorator object.
//
// The outer decorator pointer is set by a call to the inner object's
// SetOuterDecorator() in the newly-created outer decorator's constructor.

CCommandTargetNode::CCommandTargetNode(const zString label, CCommandTargetNode* const pDec) : m_pState(0),
                                                                m_id(++CCommandTargetNode::m_CommandTargetTotal),
                                                                m_Label(label),
                                                                m_InnerDecoratorName(""),
                                                                m_OuterDecoratorName(""),
																m_pInnerDecorator(pDec),
																m_pOuterDecorator(0)
{
    m_ProxyIds.clear();
}

CCommandTargetNode::~CCommandTargetNode()
{
	// Delete the xxDecoratorState if it exists

	if(m_pState)
	{
		delete m_pState;
		m_pState = 0;
	}
}

// Protected function to allow derived classes to store an xxDecoratorState pointer
// and serialize their data to file.

void CCommandTargetNode::SetState(xxDecoratorState *pState)
{
	m_pState = pState;
}

// Protected function to tell the enclosed xxDecoratorState object to
// write its data to the decorator state file. If there is no state object,
// e.g., because the decorator has no persistent state, we just return true.

bool CCommandTargetNode::SaveState() const
{
	if(m_pState)
	{
		return m_pState->Serialize();
	}
	else
		return true;
}

// ****************************************
// Do-nothing functions that provide implementations of operations 
// for those derived classes that cannot implement the functions themselves.
// All functions in the base class must be implemented by all derived classes
// to provide a uniform interface to the command targets. Because some 
// operations only make sense for simple target or composite targets, we
// provide default implementations here. We group the functions according to
// the derived class that implements the function. Other functions are 
// implemented by all derived classes.

// VF to indicate if the concrete class is a composite target. All derived classes
// except the composite target class defer to this implementation.

bool CCommandTargetNode::IsComposite() const
{
	return false;
}

// VF to indicate if at least one simple bead target is contained in a composite
// target. All derived classes except the composite target class defer to this
// implementation.

bool CCommandTargetNode::IsSimpleBeadTargetInComposite() const
{
    return false;
}

// ****************************************
// Functions implemented by CCommandTarget that holds beads.

aaVector CCommandTargetNode::GetCM() const
{
    aaVector cm(0.0, 0.0, 0.0);

    return cm;
}

bool CCommandTargetNode::IsBeadTypeInTarget(long type) const
{
	return false;
}

long CCommandTargetNode::GetBeadTotalForType(long type) const
{
	return 0;
}

long CCommandTargetNode::GetBeadTotal() const
{
	return 0;
}

// VF to return the type of the beads in the first simple bead target contained
// in a composite target or the current bead type in a simple bead target.
// All other target types defer to this implementation and return an illegal value.

long CCommandTargetNode::GetFirstBeadType() const
{
    return -1;
}

BeadVector CCommandTargetNode::GetBeads() const
{
	// Although VS6 allows you to return 0 for this function, Borland 6 does not.

	BeadVector vBeads;
	vBeads.clear();

	return vBeads;
}

long CCommandTargetNode::GetCurrentBeadType() const
{
	return -1;
}

// Functions relating to commands that change the type of beads held
// in a target. The history of the changes, in the form of (newType,oldType)
// pairs is stored so that they changes can be undone layer.

long CCommandTargetNode::GetPreviousBeadType(long newType) const
{
	return -1;
}

long CCommandTargetNode::GetOriginalBeadType() const
{
	return -1;
}

bool CCommandTargetNode::AddBeadTypePair(long newType, long oldType)
{
	return false;
}

bool CCommandTargetNode::RemoveBeadTypePair(long newType)
{
	return false;
}

bool CCommandTargetNode::ClearBeadTypePairs()
{
	return false;
}

// ****************************************
// Functions implemented by CCommandTargetPolymer that holds polymers.

bool CCommandTargetNode::IsPolymerTypeInTarget(long type) const
{
	return false;
}

long CCommandTargetNode::GetPolymerTotalForType(long type) const
{
	return 0;
}

long CCommandTargetNode::GetPolymerTotal() const
{
	return 0;
}

// Note that, unlike beads, the type of polymers held in a target 
// cannot be changed by command.

long CCommandTargetNode::GetOriginalPolymerType() const
{
	return -1;
}

PolymerVector CCommandTargetNode::GetPolymers() const
{
	// Although VS6 allows you to return 0 for this function, Borland 6 does not.

	PolymerVector vPolymers;
	vPolymers.clear();

	return vPolymers;
}

void CCommandTargetNode::AddPolymer(CPolymer* pPolymer)
{
}

void CCommandTargetNode::RemovePolymer(CPolymer* pPolymer)
{
}

// ****************************************
// Functions implemented by CCommandTargetComposite

long CCommandTargetNode::GetTargetTotal() const
{
	return 0;
}

CommandTargetSequence CCommandTargetNode::GetTargets() const
{
    CommandTargetSequence targets;
    targets.clear();

    return targets;
}

bool CCommandTargetNode::AddTarget(CCommandTargetNode *const pTarget)
{
	return false;
}

bool CCommandTargetNode::RemoveTarget(const zString label)
{
	return false;
}

CCommandTargetNode* CCommandTargetNode::GetTarget(const zString label) const
{
	return 0;
}

CCommandTargetNode* CCommandTargetNode::GetTargetRecursively(const zString label) const
{
	return 0;
}

// ****************************************
// Do-nothing function that stops the chain of execution of all the decorator
// objects that wrapp the command target.

void CCommandTargetNode::Execute(long simTime)
{

}

// Functions used by decorator objects that wrap a command target so that
// commands can be passed along a chain of decorator instances.

void CCommandTargetNode::SetInnerDecorator(CCommandTargetNode *const pDec)
{
	m_pInnerDecorator = pDec;
}

void CCommandTargetNode::SetOuterDecorator(CCommandTargetNode* const pDec)
{
	m_pOuterDecorator = pDec;
}

// Function to return the command target node that is wrapped by the
// decorator instances. It recurses through all the wrapper objects until
// it finds the innermost one whose GetInnerDecorator() function returns NULL.
// If called on a command target, it just returns its this pointer.

CCommandTargetNode* CCommandTargetNode::GetActiveCommandTarget()
{
	if(GetInnerDecorator())
	{
		return GetInnerDecorator()->GetActiveCommandTarget();
	}
	else
	{
		return this;
	}
}

// Function to return the outermost decorator instance wrapping a command target.
// It recurses through all the wrapper objects until it finds the outermost 
// one whose GetOuterDecorator() function returns NULL. If there are no
// decorators, it returns the command target itself.

CCommandTargetNode* CCommandTargetNode::GetOutermostDecorator()
{
	if(GetOuterDecorator())
	{
		return GetOuterDecorator()->GetOutermostDecorator();
	}
	else
	{
		return this;
	}
}

// Function to return the number of decorators wrapping a command target.
// If there are no decorator instances, it returns 0.
//
// It does not matter whether this function is invoked on the command
// target itself, or one of its decorator instances, it returns the
// same number. If invoked on a composite command target, it returns the 
// number of decorators wrapping the composite target only: not those
// wrapping targets contained within the composite.

long CCommandTargetNode::GetDecoratorTotal()
{
	CCommandTargetNode* pDecorator = GetActiveCommandTarget();

	long total = 0;

	while(pDecorator->GetOuterDecorator())
	{
		total++;
		pDecorator = pDecorator->GetOuterDecorator();
	}

	return total;
}

// Function to return a decorator object given its label if it currently wraps
// the same command target as the calling decorator or target. It recurses inwards
// until it reaches the innermost decorator and returns NULL if the decorator has
// not been found. Note that it should always be called on the outermost decorator
// object to ensure that all wrapping decorators are checked. We don't check in both
// directions to speed up the test.

CCommandTargetNode* CCommandTargetNode::GetDecorator(const zString decLabel)
{
	if(decLabel == m_Label)
		return this;
	else if(m_pInnerDecorator)
		return m_pInnerDecorator->GetDecorator(decLabel);
	else 
		return 0;
}

// Functions to store string identifiers as a proxy for the target's inner
// and outer decorator instances. These are used when reading targets from a 
// restart state file, and connecting them to their wrapping decorators.
// If the name is the empty string, we use the target's own label, which must
// exist and be non-empty as the constructor requires it.

void CCommandTargetNode::SetInnerDecoratorName(const zString name)
{
    if(!name.empty())
    {
        m_InnerDecoratorName = name;
    }
    else
    {
        m_InnerDecoratorName = GetLabel();
    }
}

void CCommandTargetNode::SetOuterDecoratorName(const zString name)
{
    if(!name.empty())
    {
        m_OuterDecoratorName = name;
    }
    else
    {
        m_OuterDecoratorName = GetLabel();
    }
}

// Do nothing virtual function that allows target decorators to ignore this function.
// All container classes that actually hold pointers to entities such as beads
// or polymers, must override this function so that they can replace their integer
// proxies with pointers to entities.

bool CCommandTargetNode::ReplaceProxyIds(CInitialState* const pIS)
{
    return true;
}

zInStream& CCommandTargetNode::Read(zInStream& is)
{
	return is;
}

// Function to write this class' dynamically-created data to a restart state. 

zOutStream& CCommandTargetNode::Write(zOutStream& os) const
{
    os << zEndl; // Start with a newline
	os << GetTargetType() << "  " << GetLabel();

    if(GetInnerDecorator())
    {
        os << " " << GetInnerDecorator()->GetLabel();
    }
    else
    {
        os << " " << GetLabel();
    }

    if(GetOuterDecorator())
    {
        os << " " << GetOuterDecorator()->GetLabel();
    }
    else
    {
        os << " " << GetLabel();
    }
    os << zEndl; // End with a newline

    return os;
}

// VF that is over-ridden by derived classes that contain only a single
// bead type that can have its type changed by command. Simple bead targets
// return true, all others defer to this implementation and return false,
// except that a composite target returns the boolean AND of the return values
// from all its contained targets.

bool CCommandTargetNode::ChangeNamedBeadType(long newType)
{
    return false;
}


