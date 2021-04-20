/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandTargetPolymer.cpp: implementation of the CCommandTargetPolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CommandTargetPolymer.h"
#include "Polymer.h"
#include "Bead.h"
#include "IGlobalSimBox.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString CCommandTargetPolymer::m_Type = "PolymerTarget";

const zString CCommandTargetPolymer::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new CCommandTargetPolymer(label);}

	const zString id = CCommandTargetPolymer::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor used by the target factory class when serialising a target
// from a file.

CCommandTargetPolymer::CCommandTargetPolymer(const zString label) : CCommandTargetNode(label),
											 m_OriginalType(-1)
{
    m_Polymers.clear();
}

CCommandTargetPolymer::CCommandTargetPolymer(const zString label, 
											 long polyType, PolymerVector polymers) : CCommandTargetNode(label),
											 m_OriginalType(polyType),
											 m_Polymers(polymers)
{

}

// We don't destroy the target's polymers because they are owned by the
// CInitialState.

CCommandTargetPolymer::~CCommandTargetPolymer()
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

const zString CCommandTargetPolymer::GetTargetType() const
{
    return m_Type;
}

// ****************************************
// Implementation of the IQueryPolymerTarget interface

bool CCommandTargetPolymer::IsPolymerTypeInTarget(long type) const
{
	return (type == m_OriginalType);
}

long CCommandTargetPolymer::GetPolymerTotal() const
{
	return m_Polymers.size();
}

long CCommandTargetPolymer::GetPolymerTotalForType(long type) const
{
	if(type == m_OriginalType)
		return m_Polymers.size();
	else
		return 0;
}

long CCommandTargetPolymer::GetOriginalPolymerType() const
{
	return m_OriginalType;
}

PolymerVector CCommandTargetPolymer::GetPolymers() const
{
	return m_Polymers;
}

void CCommandTargetPolymer::AddPolymer(CPolymer* pPolymer)
{
    if(pPolymer && find(m_Polymers.begin(), m_Polymers.end(), pPolymer) == m_Polymers.end())
    {
        m_Polymers.push_back(pPolymer);
    }
}

void CCommandTargetPolymer::RemovePolymer(CPolymer* pPolymer)
{
    if(pPolymer && find(m_Polymers.begin(), m_Polymers.end(), pPolymer) != m_Polymers.end())
	{
		m_Polymers.erase(find(m_Polymers.begin(), m_Polymers.end(), pPolymer));
    }
}


// ****************************************
// Implementation of the IQueryBeadTarget interface

// Function to calculate the CM of the polymer target by summing over the CM of all polymers contained in it.
// The PBCs are applied to ensure that all polymers in the target have their coordinates evaluated less than
// half a box width apart. We arbitrarily use the first polymer's coordinates to determine whether subsequent
// ones should be shifted by a box length. Finally, we check if the CM of the targt is outside the PBCs and
// bring it back in if so. Note that this may not work if a target is so large that its contained polymers
// can span more than half a box length: but then the simulation box is probably too small anyway.

aaVector CCommandTargetPolymer::GetCM() const
{
    const double polymerTotal = static_cast<double>(GetPolymerTotal());
    
    double tcm[3];
    tcm[0] = 0.0;
    tcm[1] = 0.0;
    tcm[2] = 0.0;
    
    const double x0 = m_Polymers.front()->GetCM().GetX();
    const double y0 = m_Polymers.front()->GetCM().GetY();
    const double z0 = m_Polymers.front()->GetCM().GetZ();

    for(cPolymerVectorIterator citerPoly = m_Polymers.begin(); citerPoly != m_Polymers.end(); ++citerPoly)
    {
        double x = (*citerPoly)->GetCM().GetX();
        double y = (*citerPoly)->GetCM().GetY();
        double z = (*citerPoly)->GetCM().GetZ();

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
 
        tcm[0] += x;
        tcm[1] += y;
        tcm[2] += z;
    }
    
    tcm[0] /= polymerTotal;
    tcm[1] /= polymerTotal;
    tcm[2] /= polymerTotal;
    
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

bool CCommandTargetPolymer::IsBeadTypeInTarget(long type) const
{
	// Get the set of beads contained in all polymers

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

long CCommandTargetPolymer::GetBeadTotalForType(long type) const
{
	// Get the set of beads contained in all polymers

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

// Function to return the number of beads contained in all the polymers held in the target.
// Because a polymer target can only hold a single type of polymer, we could just 
// multiply the number of polymers by the polymer size. However, because polymers
// may in future have their architectures modified during a run by commands, 
// we play it safe by summing up the beads contained in each polymer. Note that 
// the CPolymer::GetSize() function returns the actual number of beads in a polymer, 
// not the expected size given its original architecture.

long CCommandTargetPolymer::GetBeadTotal() const
{
	long beadTotal = 0;

	for(cPolymerVectorIterator citerPoly=m_Polymers.begin(); citerPoly!=m_Polymers.end(); citerPoly++)
	{
		beadTotal += (*citerPoly)->GetSize();
	}

	return beadTotal;
}

// Function to return the aggregate of all beads forming the polymers contained
// in the polymer command target. Unlike the GetBeads() function defined for
// bead command targets, this function may return beads of many different types
// depending on the architecture of the polymers.

BeadVector CCommandTargetPolymer::GetBeads() const
{
	BeadVector vBeads;	// Local vector to hold beads combined from all targets
	vBeads.clear();

	for(cPolymerVectorIterator citerPoly=m_Polymers.begin(); citerPoly!=m_Polymers.end(); citerPoly++)
	{
		BeadVector vTargetBeads = (*citerPoly)->GetBeads();

		vBeads.insert(vBeads.end(), vTargetBeads.begin(), vTargetBeads.end());
	}

	return vBeads;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& CCommandTargetPolymer::Read(zInStream& is)
{
    is >> m_OriginalType;
    if(!is.good() || m_OriginalType < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing polymer command target: polymer type invalid");
        return is;
    }

    long proxyId;
    long targetSize = 0;
    is >> targetSize;
    if(!is.good() || targetSize < 0)
    {
        new CLogRestartStateBuilderError(0, "Error importing polymer command target: number of polymers invalid");
        return is;
    }
//    std::cout << "Reading " << targetSize << " polymers into target" << zEndl;

    for(long j=0; j<targetSize; j++)
    {
        is >> proxyId;
        if(!is.good() || proxyId < 0)
        {
            new CLogRestartStateBuilderError(0, "Error importing polymer command target: invalid proxy id");
            return is;
        }

        // Add the proxy id to the target's container for later replacement
        // with an actual pointer

        m_ProxyIds.push_back(proxyId);
    }


    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
//
zOutStream& CCommandTargetPolymer::Write(zOutStream& os) const
{ 
    CCommandTargetNode::Write(os);  // Write base class data first

    os << GetOriginalPolymerType() << zEndl;
    os << GetPolymerTotal() << zEndl;
    for(cPolymerVectorIterator citerPoly=m_Polymers.begin(); citerPoly!=m_Polymers.end(); citerPoly++)
    {
        os << (*citerPoly)->GetId() << zEndl;
    }

    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

// Virtual function to replace a target's integer proxies with actual pointers.
// This function assumes that a polymer's id defines its place in the container
// of all polymers. As polymer ids are immutable, and they are created sequentially,
// this should be true:but note the zero-based indexing of the container!

bool CCommandTargetPolymer::ReplaceProxyIds(CInitialState* const pIS)
{
    for(zLongVectorIterator iv=m_ProxyIds.begin(); iv!=m_ProxyIds.end(); iv++)
    {
        CPolymer* pPolymer = pIS->GetPolymers().at(*iv-1);

//        std::cout << "target " << GetLabel() << " replacing polymer proxy " << *iv << " with polymer pointer " << pPolymer->GetId() << zEndl;

        if(pPolymer)
        {
            m_Polymers.push_back(pPolymer);
        }
        else
        {
            new CLogRestartStateBuilderError(0, "Error reading restart state: unable to replace polymer proxy (invalid id?)");
            return false;
        }
    }

    return true;
}
