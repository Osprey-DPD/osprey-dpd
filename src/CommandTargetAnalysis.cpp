/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTargetAnalysis.cpp: implementation of the CCommandTargetAnalysisAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CommandTargetAnalysis.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString CCommandTargetAnalysis::m_Type = "AnalysisTarget";

const zString CCommandTargetAnalysis::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new CCommandTargetAnalysis(label);}

	const zString id = CCommandTargetAnalysis::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor used by the target factory class when serialising a target
// from a file.

CCommandTargetAnalysis::CCommandTargetAnalysis(const zString label) : CCommandTargetNode(label),
													m_OriginalType(-1),
													m_CurrentType(-1)
{
    m_Beads.clear();
	m_mTypeMap.clear();
}

CCommandTargetAnalysis::CCommandTargetAnalysis(const zString label, long beadType, 
								BeadVector beads) : CCommandTargetNode(label),
													m_OriginalType(beadType),
													m_CurrentType(beadType),
													m_Beads(beads)
{

	// Initialise a map to hold pairs (newType, oldType) so that 
	// we can restore the previous bead type if necessary. Note 
	// that the map can contain more than one entry if the bead type is
	// changed several times. The original bead type is stored in
	// m_BeadType so that it is always accessible.

	m_mTypeMap.clear();

}

CCommandTargetAnalysis::~CCommandTargetAnalysis()
{
	// Delete all associated decorator instances from the innermost
	// to the outermost.

	CCommandTargetNode * pDec = GetOuterDecorator();

	while(pDec)
	{
		CCommandTargetNode * pOuterDec = pDec->GetOuterDecorator();
		delete pDec;
		pDec = pOuterDec;
	}
}

// Non-static member function to return the target's type.

const zString CCommandTargetAnalysis::GetTargetType() const
{
    return m_Type;
}

bool CCommandTargetAnalysis::IsBeadTypeInTarget(long type) const
{
	return (type == m_CurrentType);
}

// Function to count the beads of the specified type in the target.
// If the type is not found, it just returns zero.

long CCommandTargetAnalysis::GetBeadTotalForType(long type) const
{
	if(type == m_CurrentType)
		return m_Beads.size();
	else
		return 0;
}

long CCommandTargetAnalysis::GetBeadTotal() const
{
	return m_Beads.size();
}

BeadVector CCommandTargetAnalysis::GetBeads() const
{
	return m_Beads;
}

long CCommandTargetAnalysis::GetCurrentBeadType() const
{
	return m_CurrentType;
}

// Function to retrieve the previous type of the beads given the
// current type. This function should only be called after the
// AddTypePair() otherwise it returns an invalid bead type (-1).

long CCommandTargetAnalysis::GetPreviousBeadType(long newType) const
{
	if(m_mTypeMap.find(newType) != m_mTypeMap.end())
		return (*m_mTypeMap.find(newType)).second;
	else
	{
		ErrorTrace("Command target error: bead type not in map");
		return -1;
	}
}

long CCommandTargetAnalysis::GetOriginalBeadType() const
{
	return m_OriginalType;
}

// Function to add a pair to the bead type map indicating that
// the type of the beads contained in the command target has been
// changed. It is used by ccChangeBeadType commands. Storing the 
// types as (newType, oldType) pairs allows the command to be undone
// using the ccRestoreBeadType command. The new type is also stored 
// separately.
//
// We only add the pair if it is not already in the map and the old and 
// new types are different.

bool CCommandTargetAnalysis::AddBeadTypePair(long newType, long oldType)
{
	if(m_mTypeMap.find(newType) != m_mTypeMap.end())
	{
		return ErrorTrace("Command target error: bead type pair already in map");
	}
	else if(newType == oldType)
	{
		return ErrorTrace("Command target error: old and new bead types are equal");
	}
	else
	{
		m_mTypeMap.insert(zPairLL(newType, oldType));
		m_CurrentType = newType;
		return true;
	}
}

// Function to remove a pair from the bead type map. It is used
// by the ccRestoreBeadType command to change a bead type to its
// previous value. We restore the m_CurrentType to its previous
// value before removing the pair.

bool CCommandTargetAnalysis::RemoveBeadTypePair(long newType)
{
	if(m_mTypeMap.find(newType) != m_mTypeMap.end())
	{
		m_CurrentType = (*m_mTypeMap.find(newType)).second;
		m_mTypeMap.erase(newType);
		return true;
	}
	else
		return ErrorTrace("Command target error: bead type not in map cannot be removed");
}

// Function to clear the target's bead type map, and restore the current bead type 
// to the original type. It is used by the ccRestoreOriginalBeadType command.

bool CCommandTargetAnalysis::ClearBeadTypePairs()
{
	m_mTypeMap.clear();
	m_CurrentType = m_OriginalType;

	return true;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& CCommandTargetAnalysis::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& CCommandTargetAnalysis::Write(zOutStream& os) const
{
    
    return os;
}

