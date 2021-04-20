/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTarget.cpp: implementation of the CCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CommandTarget.h"
#include "Bead.h"
#include "CurrentState.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString CCommandTarget::m_Type = "BeadTarget";

const zString CCommandTarget::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new CCommandTarget(label);}

	const zString id = CCommandTarget::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor used by the target factory class when serialising a target
// from a file.

CCommandTarget::CCommandTarget(const zString label) : CCommandTargetNode(label),
													  m_OriginalType(-1),
													  m_CurrentType(-1)
{
    m_Beads.clear();
	m_mTypeMap.clear();
}

CCommandTarget::CCommandTarget(const zString label, long beadType, 
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

CCommandTarget::~CCommandTarget()
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

const zString CCommandTarget::GetTargetType() const
{
    return m_Type;
}


// ****************************************
// Implementation of the IQueryBeadTarget interface

// Function to calculate the CM of the target. We loop over all beads in the target and calculate their CM.
// The PBCs are applied to ensure that all beads in the target have their coordinates evaluated less than
// half a box width apart. We arbitrarily use the first bead's coordinates to determine whether subsequent
// ones should be shifted by a box length. Finally, we check if the CM of the targt is outside the PBCs and
// bring it back in if so. Note that this may not work if a target is so large that its contained beads
// can span more than half a box length: but then the simulation box is probably too small anyway.

aaVector CCommandTarget::GetCM() const
{
    const double beadTotal = static_cast<double>(GetBeadTotal());

    double tcm[3];
    tcm[0] = 0.0;
    tcm[1] = 0.0;
    tcm[2] = 0.0;
    
    const double x0 = m_Beads.front()->GetXPos();
    const double y0 = m_Beads.front()->GetYPos();
    const double z0 = m_Beads.front()->GetZPos();

    for(cBeadVectorIterator citerBead = m_Beads.begin(); citerBead != m_Beads.end(); ++citerBead)
    {
        double x = (*citerBead)->GetXPos();
        double y = (*citerBead)->GetYPos();
        double z = (*citerBead)->GetZPos();

        // Apply PBCs
        
        if( (x - x0) > IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
            x = x - IGlobalSimBox::Instance()->GetSimBoxXLength();
        else if( (x - x0) < -IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
            x = x + IGlobalSimBox::Instance()->GetSimBoxXLength();

        if( (y - y0) > IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
            y = y - IGlobalSimBox::Instance()->GetSimBoxYLength();
        else if( (y - y0) < -IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
            y = y + IGlobalSimBox::Instance()->GetSimBoxYLength();

        if( (z - z0) > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
            z = z - IGlobalSimBox::Instance()->GetSimBoxZLength();
        else if( (z - z0) < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
            z = z + IGlobalSimBox::Instance()->GetSimBoxZLength();

        tcm[0] += (*citerBead)->GetXPos();
        tcm[1] += (*citerBead)->GetYPos();
        tcm[2] += (*citerBead)->GetZPos();
    }

    tcm[0] /= beadTotal;
    tcm[1] /= beadTotal;
    tcm[2] /= beadTotal;
    
    // If the CM is outside the PBCs translate it back in
    
    if( tcm[0] > IGlobalSimBox::Instance()->GetSimBoxXLength() )
        tcm[0] = tcm[0] - IGlobalSimBox::Instance()->GetSimBoxXLength();
    else if( tcm[0] < 0.0 )
        tcm[0] = tcm[0] + IGlobalSimBox::Instance()->GetSimBoxXLength();

    if( tcm[1] > IGlobalSimBox::Instance()->GetSimBoxYLength() )
        tcm[1] = tcm[1] - IGlobalSimBox::Instance()->GetSimBoxYLength();
    else if( tcm[1] < 0.0 )
        tcm[1] = tcm[1] + IGlobalSimBox::Instance()->GetSimBoxYLength();

    if( tcm[2] > IGlobalSimBox::Instance()->GetSimBoxZLength() )
        tcm[2] = tcm[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
    else if( tcm[2] < 0.0 )
        tcm[2] = tcm[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();

    aaVector cm(tcm[0], tcm[1], tcm[2]);

    return cm;
}

bool CCommandTarget::IsBeadTypeInTarget(long type) const
{
	return (type == m_CurrentType);
}

// Function to count the beads of the specified type in the target.
// If the type is not found, it just returns zero.

long CCommandTarget::GetBeadTotalForType(long type) const
{
	if(type == m_CurrentType)
		return m_Beads.size();
	else
		return 0;
}

long CCommandTarget::GetBeadTotal() const
{
	return m_Beads.size();
}

// VF that returns the current bead type for a simple bead target.

long CCommandTarget::GetFirstBeadType() const
{
    return GetCurrentBeadType();
}

BeadVector CCommandTarget::GetBeads() const
{
	return m_Beads;
}

// ****************************************
// Functions peculiar to simple bead targets

long CCommandTarget::GetCurrentBeadType() const
{
	return m_CurrentType;
}

// Function to retrieve the previous type of the beads given the
// current type. This function should only be called after the
// AddTypePair() otherwise it returns an invalid bead type (-1).

long CCommandTarget::GetPreviousBeadType(long newType) const
{
	if(m_mTypeMap.find(newType) != m_mTypeMap.end())
		return (*m_mTypeMap.find(newType)).second;
	else
	{
		ErrorTrace("Command target error: bead type not in map");
		return -1;
	}
}

long CCommandTarget::GetOriginalBeadType() const
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

bool CCommandTarget::AddBeadTypePair(long newType, long oldType)
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

bool CCommandTarget::RemoveBeadTypePair(long newType)
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

bool CCommandTarget::ClearBeadTypePairs()
{
	m_mTypeMap.clear();
	m_CurrentType = m_OriginalType;

	return true;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. The generic target data has already
// been read in the base class' operator>>() function, and we read only the
// target-specific data here. Because we don't have access to the beads here,
// we store their ids in a local container and later replace them with the actual
// bead pointers.

zInStream& CCommandTarget::Read(zInStream& is)
{
    is >> m_CurrentType >> m_OriginalType;
    if(!is.good() || m_CurrentType < 0 || m_OriginalType < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing bead command target: bead types invalid");
        return is;
    }

    long newType, oldType;
    long mapSize = 0;
    is >> mapSize;
    if(!is.good() || mapSize < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing bead command target: bead type map size invalid");
        return is;
    }

    for(long i=0; i<mapSize; i++)
    {
        is >> newType >> oldType;
        if(!is.good() || mapSize < 0)
        {
            new CLogRestartStateBuilderError(0, "Error importing bead command target: unable to insert bead type pair into map");
            return is;
        }
        AddBeadTypePair(newType, oldType);
    }

    long proxyId;
    long targetSize = 0;
    is >> targetSize;
    if(!is.good() || targetSize < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing bead command target: invalid bead total");
        return is;
    }

    for(long j=0; j<targetSize; j++)
    {
        is >> proxyId;
        if(!is.good() || proxyId < 0)
        {
            new CLogRestartStateBuilderError(0, "Error importing bead command target: invalid proxy id");
            return is;
        }

        // Add the proxy id to the target's container for later replacement
        // with an actual pointer

        m_ProxyIds.push_back(proxyId);
    }

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First, we write the generic target data in the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& CCommandTarget::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first

    os << GetCurrentBeadType() << " " << GetOriginalBeadType() << zEndl;

    os << m_mTypeMap.size() << zEndl;
    for(cLongLongIterator citerLL=m_mTypeMap.begin(); citerLL!=m_mTypeMap.end(); citerLL++)
    {
        os << citerLL->first << " " << citerLL->second << zEndl;
    }
    os << GetBeadTotal() << zEndl;
    for(cBeadVectorIterator citerBead=m_Beads.begin(); citerBead!=m_Beads.end(); citerBead++)
    {
        os << (*citerBead)->GetId()<< zEndl;
    }

    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

// Virtual function to replace a target's integer proxies with actual pointers.
// This function assumes that a bead's id defines its place in the container
// of all beads. As bead ids are immutable, and they are created sequentially,
// this should be true: but note the zero-based indexing of the container!

bool CCommandTarget::ReplaceProxyIds(CInitialState* const pIS)
{
    for(zLongVectorIterator iv=m_ProxyIds.begin(); iv!=m_ProxyIds.end(); iv++)
    {
        CBead* pBead = pIS->GetBeads().at(*iv-1);

//        std::cout << "target " << GetLabel() << " replacing bead proxy " << *iv << " with bead " << pBead->GetId() << zEndl;

        if(pBead)
        {
            m_Beads.push_back(pBead);
        }
        else
        {
            new CLogRestartStateBuilderError(0, "Error reading restart state: unable to replace bead proxy (invalid id?)");
            return false;
        }
    }

    return true;
}

// VF to change the type of all the beads in a simple bead target to a new value.
// Note that we only store the numeric bead type here, the targets know nothing
// about any string name associated with this type, and do not propagate any 
// changes outside this class: it is the responsibility of the calling routine
// to ensure that bead types stored elsewhere in the simulation are consistent.

bool CCommandTarget::ChangeNamedBeadType(long newType)
{
	for(BeadVectorIterator iterBead = m_Beads.begin(); iterBead != m_Beads.end(); iterBead++)
	{
		(*iterBead)->SetType(newType);
        (*iterBead)->SetVisible();
        CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), newType);
	}

    // Store the old and new types in a map so that we can restore the old type 
    // later if necessary

    AddBeadTypePair(newType, GetCurrentBeadType());

    // Return true to indicate that the beads' type has been successfully changed

    return true;
}

