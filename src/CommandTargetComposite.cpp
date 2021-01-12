/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTargetComposite.cpp: implementation of the CCommandTargetComposite class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CommandTargetComposite.h"
#include "CommandTarget.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString CCommandTargetComposite::m_Type = "CompositeTarget";

const zString CCommandTargetComposite::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CCommandTargetNode* Create(const zString label) {return new CCommandTargetComposite(label);}

	const zString id = CCommandTargetComposite::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor used by the target factory class when serialising a target
// from a file.

CCommandTargetComposite::CCommandTargetComposite(const zString label) : CCommandTargetNode(label)
{
	m_mTargetMap.clear();
}

CCommandTargetComposite::~CCommandTargetComposite()
{
	// Delete all associated decorator instances from the innermost
	// to the outermost. But don't delete any contained targets as they
	// have been created independently of this composite target and will
	// be deleted by the CSimBox when its destructor is called.

	CCommandTargetNode * pDec = GetOuterDecorator();

	while(pDec)
	{
		CCommandTargetNode * pOuterDec = pDec->GetOuterDecorator();
		delete pDec;
		pDec = pOuterDec;
	}
}

// Non-static member function to return the target's type.

const zString CCommandTargetComposite::GetTargetType() const
{
    return m_Type;
}

// Function to indicate if the CCommandTargetNode is a composite target.

bool CCommandTargetComposite::IsComposite() const
{
	return true;
}

// Function to indicate if at least one simple bead target is contained
// in this composite target. This is needed by commands that act on
// simple bead targets only and that need to know if a composite target can 
// be a valid target for their actions. We simply check that there is a bead
// type somewhere in the composite target that is valid.

bool CCommandTargetComposite::IsSimpleBeadTargetInComposite() const
{
    return (GetFirstBeadType() != CCommandTargetNode::GetFirstBeadType());
}

// ****************************************
// Implementation of the IQueryPolymerTarget interface

bool CCommandTargetComposite::IsPolymerTypeInTarget(long type) const
{
	bool bFound = false;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		if(!bFound && (iterTarget->second)->IsPolymerTypeInTarget(type))
			bFound =true;
	}

	return bFound;
}

long CCommandTargetComposite::GetPolymerTotalForType(long type) const
{
	long polymerTotal = 0;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		if((iterTarget->second)->IsPolymerTypeInTarget(type))
			polymerTotal += (iterTarget->second)->GetPolymerTotalForType(type);
	}

	return polymerTotal;
}

long CCommandTargetComposite::GetPolymerTotal() const
{
	long polymerTotal = 0;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		polymerTotal += (iterTarget->second)->GetPolymerTotal();
	}

	return polymerTotal;
}

PolymerVector CCommandTargetComposite::GetPolymers() const
{
	PolymerVector vPolymers;	// Local vector to hold polymers combined from all targets
	vPolymers.clear();

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		// We use a temporary vector to hold the target's polymers because 
		// applying begin() and end() to the (iterTarget->second)->GetPolymers() 
		// function directly yields null pointers.

		PolymerVector vTargetPolymers = (iterTarget->second)->GetPolymers();

		vPolymers.insert(vPolymers.end(), vTargetPolymers.begin(), vTargetPolymers.end());
	}

	return vPolymers;
}

// ****************************************
// Implementation of the IQueryBeadTarget interface

// Function to check if a specified bead type is contained in one of the
// targets held in the composite target.

bool CCommandTargetComposite::IsBeadTypeInTarget(long type) const
{
	bool bFound = false;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		if(!bFound && (iterTarget->second)->IsBeadTypeInTarget(type))
			bFound =true;
	}

	return bFound;
}

// Function to count the number of beads of the specified type held in any
// and all targets in the composite target. If no beads of the type are found,
// it just returns zero.

long CCommandTargetComposite::GetBeadTotalForType(long type) const
{
	long beadTotal = 0;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		if((iterTarget->second)->IsBeadTypeInTarget(type))
			beadTotal += (iterTarget->second)->GetBeadTotalForType(type);
	}

	return beadTotal;
}

// Function to return the total number of beads stored in all command targets
// that are aggregated in this composite target.

long CCommandTargetComposite::GetBeadTotal() const
{
	long beadTotal = 0;

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		beadTotal += (iterTarget->second)->GetBeadTotal();
	}

	return beadTotal;
}

// VF to return the type of the beads in the first simple bead target contained
// in a composite target. If no simple bead target is found, it returns the illegal
// value -1.

long CCommandTargetComposite::GetFirstBeadType() const
{
	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
        CCommandTargetNode* pTarget = iterTarget->second;

        if(dynamic_cast<CCommandTarget*>(pTarget))
        {
            return pTarget->GetCurrentBeadType();
        }
        else if(pTarget->IsComposite() && pTarget->GetFirstBeadType() != CCommandTargetNode::GetFirstBeadType())
        {
            return pTarget->GetFirstBeadType();
        }
	}

    // If we reach here, it means the composite target only contained non-simple
    // bead type targets so we call the base class function to return an illegal
    // bead type

    return CCommandTargetNode::GetFirstBeadType();
}

BeadVector CCommandTargetComposite::GetBeads() const
{
	BeadVector vBeads;	// Local vector to hold beads combined from all targets
	vBeads.clear();

	for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		// We use a temporary vector to hold the target's beads because 
		// applying begin() and end() to the (iterTarget->second)->GetBeads() 
		// function directly yields null pointers.

		BeadVector vTargetBeads = (iterTarget->second)->GetBeads();

		vBeads.insert(vBeads.end(), vTargetBeads.begin(), vTargetBeads.end());
	}

	return vBeads;
}

// ****************************************

// Function to return the number of targets held in the composite target.

long CCommandTargetComposite::GetTargetTotal() const
{
	return m_mTargetMap.size();
}

// Function to add a target to the composite's map using its label as the key. 
// A target can be added directly at most once, and if both targets are composite
// we check that each is not contained in the other. But we cannot check that
// a third target is not contained, at some level, in two distinct trees
// of composite targets.

bool CCommandTargetComposite::AddTarget(CCommandTargetNode *const pTarget)
{
	const zString label = pTarget->GetLabel();

	if(!GetTargetRecursively(label) && !pTarget->GetTargetRecursively(GetLabel()))
	{
		m_mTargetMap.insert(zPairST(label, pTarget));
		return true;
	}
	else
		return ErrorTrace("Composite command target error: target already in map");
}

// Function to remove a command target from the map using its label as the key.

bool CCommandTargetComposite::RemoveTarget(const zString label)
{
	if(m_mTargetMap.find(label) != m_mTargetMap.end())
	{
		m_mTargetMap.erase(label);
		return true;
	}
	else
		return ErrorTrace("Composite command target error: target not in map cannot be removeed");
}

// Function to return a target stored in the composite target. If not found,
// a NULL pointer is retured.

CCommandTargetNode* CCommandTargetComposite::GetTarget(const zString label) const
{
	CCommandTargetNode* pTarget = 0;

	if(m_mTargetMap.find(label) != m_mTargetMap.end())
	{
		pTarget = (*m_mTargetMap.find(label)).second;
	}

	return pTarget;
}

// Function to return a target stored at any level in a composite target. 
// If the sought target is not found at the first level in the composite target,
// we iterate over all contained targets and search all those that are themselves
// composite.
// If not found, a NULL pointer is retured.

CCommandTargetNode* CCommandTargetComposite::GetTargetRecursively(const zString label) const
{
	CCommandTargetNode* pTarget = 0;

	if(m_mTargetMap.find(label) != m_mTargetMap.end())
	{
		pTarget = (*m_mTargetMap.find(label)).second;
	}
    else
    {
        for(cStringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
        {
            if(iterTarget->second->IsComposite())
                return iterTarget->second->GetTargetRecursively(label);
        }
    }

	return pTarget;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. As a composite commmand target 
// contains other targets, we store just the unique id and label of each
// contained target.

zInStream& CCommandTargetComposite::Read(zInStream& is)
{
    long proxyId;
    zString label;
    long targetSize = 0;
    is >> targetSize;
    if(!is.good() || targetSize < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing composite command target: contained target number invalid");
        return is;
    }
//    std::cout << "Reading " << targetSize << " targets into composite target" << zEndl;

    for(long j=0; j<targetSize; j++)
    {
        is >> proxyId >> label;
        if(!is.good() || proxyId < 0 || label.empty())
        {
            new CLogRestartStateBuilderError(0, "Error importing composite command target: invalid proxy id or label");
            return is;
        }

        // Add the proxy id to the target's container for later replacement
        // with an actual pointer

        m_ProxyIds.push_back(proxyId);
    }

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// A composite target stores other targets, so we just write out their unique
// identifying label and id. The latter is necessary as as proxy to allow use
// to read in composite targets from a restart state and replace the numeric
// proxy ids with the actualy targets.
//
zOutStream& CCommandTargetComposite::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first

    os << GetTargetTotal() << zEndl;
    for(cStringTargetIterator citerST=m_mTargetMap.begin(); citerST!=m_mTargetMap.end(); citerST++)
    {
        os << citerST->second->GetId() << " " << citerST->first << zEndl;
    }

    return os;
}

// Virtual function to replace a target's integer proxies with actual pointers.
// This function must be called after all targets have been imported into the 
// initial state so that it can retrieve their pointers given just their unique
// numeric id.

bool CCommandTargetComposite::ReplaceProxyIds(CInitialState* const pIS)
{
    bool bFound = false;
    CCommandTargetNode* pTarget= 0;

    CommandTargetSequence targets = pIS->GetCommandTargets();

    for(zLongVectorIterator iv=m_ProxyIds.begin(); iv!=m_ProxyIds.end(); iv++)
    {
        bFound = false;
        CommandTargetIterator it=targets.begin(); 

        while(!bFound && it!=targets.end())
        {
            if((*it)->GetId() == *iv)
            {
                bFound = true;
                pTarget = *it;
            }
            it++;
        }

        // We use the member function to add the new target as it performs checks
        // to avoid recursive and duplicate target containment.

        if(bFound && pTarget)
        {
            AddTarget(pTarget);
        }
        else
        {
            new CLogRestartStateBuilderError(0, "Error reading restart state: unable to replace target proxy (invalid id?)");
            return false;
        }
    }

    return true;
}

// VF that propagates the command to change a simple bead target's beads
// to a new type. It iterates over all of its contained targets recursively,
// and returns the boolean AND of their return values. A return value of true
// indicates that only simple bead targets were found, and their types changed,
// while a return value of false indicates that some contained targets were
// non-simple bead targets and their contents were unchanged. Note that in the 
// latter case, contained simple bead targets still have their bead types
// changed.

bool CCommandTargetComposite::ChangeNamedBeadType(long newType)
{
    bool bAllSimpleBeadTargets = true;

	for(StringTargetIterator iterTarget=m_mTargetMap.begin(); iterTarget!=m_mTargetMap.end(); iterTarget++)
	{
		bAllSimpleBeadTargets = bAllSimpleBeadTargets && (iterTarget->second)->ChangeNamedBeadType(newType);
	}

    return bAllSimpleBeadTargets;
}


