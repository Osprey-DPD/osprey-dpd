/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTargetACNFreeBonds.cpp: implementation of the CCommandTargetACNFreeBonds class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "CommandTargetACNFreeBonds.h"
#include "aeActiveCellNetwork.h"
#include "aeActivePolymer.h"
#include "aeActiveBond.h"
#include "Polymer.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString CCommandTargetACNFreeBonds::m_Type = "ACNFreeBondsTarget";

const zString CCommandTargetACNFreeBonds::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new CCommandTargetACNFreeBonds(label);}

	const zString id = CCommandTargetACNFreeBonds::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor used by the target factory class when serialising a target
// from a file.

CCommandTargetACNFreeBonds::CCommandTargetACNFreeBonds(const zString label) : CCommandTargetNode(label),
											 m_pACN(0)
{

}

CCommandTargetACNFreeBonds::CCommandTargetACNFreeBonds(const zString label, 
											 aeActiveCellNetwork* pACN) : CCommandTargetNode(label),
											 m_pACN(pACN)
{
	// Store the type of the CPolymers that make up the active bonds, but only
	// if there are any. Otherwise use a free active polymer. If neither exist,
	// set the original type to an invalid value.

	if(m_pACN->GetFreeBondTotal() > 0)
	{
		ActiveBondList lFreeActiveBonds = m_pACN->GetFreeBonds();

		const aeActiveBond* const pBond = lFreeActiveBonds.front();

		m_OriginalType = pBond->GetTailMonomer()->GetType();
	}
	else if(m_pACN->GetFreePolymerTotal() > 0)
	{
		ActivePolymerList lFreeActivePolymers = m_pACN->GetFreePolymers();

		const aeActivePolymer* const pPolymer = lFreeActivePolymers.front();

		m_OriginalType = pPolymer->GetTailBond()->GetTailMonomer()->GetType();
	}
	else
	{
		m_OriginalType = -1;
	}
}

// We don't destroy the target's polymers because they are owned by the
// CInitialState.

CCommandTargetACNFreeBonds::~CCommandTargetACNFreeBonds()
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

const zString CCommandTargetACNFreeBonds::GetTargetType() const
{
    return m_Type;
}

// ****************************************
// Implementation of the IQueryPolymerTarget interface

bool CCommandTargetACNFreeBonds::IsPolymerTypeInTarget(long type) const
{
	return (type == m_OriginalType);
}

long CCommandTargetACNFreeBonds::GetPolymerTotal() const
{
	return GetPolymers().size();
}

long CCommandTargetACNFreeBonds::GetPolymerTotalForType(long type) const
{
	if(type == m_OriginalType)
	{
		return GetPolymerTotal();
	}
	else
		return 0;
}

long CCommandTargetACNFreeBonds::GetOriginalPolymerType() const
{
	return m_OriginalType;
}

PolymerVector CCommandTargetACNFreeBonds::GetPolymers() const
{
	ActiveBondList lFreeActiveBonds = m_pACN->GetFreeBonds();

	// Note that we do not have to include both the head and tail of each
	// active bond as the head monomer of one bond is the tail monomer 
	// of its successor.

	PolymerVector vPolymers;
	vPolymers.clear();

	for(ActiveBondListIterator iterBond = lFreeActiveBonds.begin(); iterBond != lFreeActiveBonds.end(); iterBond++)
	{
		vPolymers.push_back((*iterBond)->GetTailMonomer());
	}

	return vPolymers;
}

// ****************************************
// Implementation of the IQueryBeadTarget interface

bool CCommandTargetACNFreeBonds::IsBeadTypeInTarget(long type) const
{
	// Get the set of beads contained in all polymers. We don't update the
	// stored polymers in this function as the GetBeads() function does it.

	const BeadVector vBeads = GetBeads();

	// Now search them for any of the requested type. Note that using a while loop
	// is faster than counting the beads as in GetBeadTotalForType(). Hence, it
	// makes sense to call this function before trying to cound beads of a specfied type.

	bool bFound = false;

	if(vBeads.size() > 0)
	{
		cBeadVectorIterator citerBead=vBeads.begin();

		while(!bFound && citerBead!=vBeads.end())
		{
			if((*citerBead)->GetType() == type)
			{
				bFound = true;
			}
			citerBead++;
		}
	}

	return bFound;
}

// Function to count the beads of the specified type in the target.
// If the type is not found, it just returns zero.

long CCommandTargetACNFreeBonds::GetBeadTotalForType(long type) const
{
	// Get the set of beads contained in all polymers. We don't update the
	// stored polymers in this function as the GetBeads() function does it.

	const BeadVector vBeads = GetBeads();

	// Now search them for beads of the requested type

	long beadTotal = 0;

	for(cBeadVectorIterator citerBead=vBeads.begin(); citerBead!=vBeads.end(); citerBead++)
	{
		if((*citerBead)->GetType() == type)
		{
			beadTotal += 1;
		}
	}

	return beadTotal;
}

// Function to return the number of beads contained in all the polymers held in 
// the target.  Because a polymer target can only hold a single type of polymer, 
// we could just multiply the number of polymers by the polymer size. However, 
// because polymers may in future have their architectures modified during a run 
// by commands, we play it safe by summing up the beads contained in each polymer. 
// Note that the CPolymer::GetSize() function returns the actual number of beads 
// in a polymer, not the expected size given its original architecture.

long CCommandTargetACNFreeBonds::GetBeadTotal() const
{
	long beadTotal = 0;

	// First update the stored polymers from the ACN

	PolymerVector vPolymers;
	vPolymers.clear();
	vPolymers = GetPolymers();

	for(cPolymerVectorIterator citerPoly=vPolymers.begin(); citerPoly!=vPolymers.end(); citerPoly++)
	{
		beadTotal += (*citerPoly)->GetSize();
	}

	return beadTotal;
}

// Function to return the aggregate of all beads forming the polymers contained
// in the command target. Unlike the GetBeads() function defined for
// bead command targets, this function may return beads of many different types
// depending on the architecture of the polymers.

BeadVector CCommandTargetACNFreeBonds::GetBeads() const
{
	BeadVector vBeads;	// Local vector to hold beads combined from all targets
	vBeads.clear();

	// First update the stored polymers from the ACN

	PolymerVector vPolymers;
	vPolymers.clear();
	vPolymers = GetPolymers();

	for(cPolymerVectorIterator citerPoly=vPolymers.begin(); citerPoly!=vPolymers.end(); citerPoly++)
	{
		BeadVector vTargetBeads = (*citerPoly)->GetBeads();

		vBeads.insert(vBeads.end(), vTargetBeads.begin(), vTargetBeads.end());
	}

	return vBeads;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& CCommandTargetACNFreeBonds::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& CCommandTargetACNFreeBonds::Write(zOutStream& os) const
{
    
    return os;
}

