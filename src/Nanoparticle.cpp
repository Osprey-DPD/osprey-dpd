/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Nanoparticle.cpp: implementation of the CNanoparticle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "Nanoparticle.h"
#include "Bead.h"
#include "Bond.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Static member variable holding the number of commands created.

long CNanoparticle::m_NanoparticleTotal = 0;

long CNanoparticle::GetNanoparticleTotal()
{
	return m_NanoparticleTotal;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor used to make empty polymer instances.

CNanoparticle::CNanoparticle(BondVector vBonds) : m_id(++CNanoparticle::m_NanoparticleTotal), m_MaxWidth(0.0), m_XCM(0.0), m_YCM(0.0), m_ZCM(0.0), m_vBonds(vBonds)
{
    // Create a local map <long, CAbstractBead*> mapping bead ids to instances
    // zPairLongBead is the map iterator
    // LongBeadMap is the container
    // We rely on the beahviour that insertion into a map fails if the key is already present.
    
    m_vBeads.clear();
    
    LongBeadMap mBeads;
    mBeads.clear();
    
    CAbstractBead* pHead = 0;
    CAbstractBead* pTail = 0;
        
    for(cBondVectorIterator citerBond=m_vBonds.begin(); citerBond!=m_vBonds.end(); ++citerBond)
    {
        pHead = (*citerBond)->GetHead();
        pTail = (*citerBond)->GetTail();
        
        mBeads.insert(make_pair(pHead->GetId(), pHead));
        mBeads.insert(make_pair(pTail->GetId(), pTail));    
    }
        
    for(LongBeadIterator iterBead=mBeads.begin(); iterBead!=mBeads.end(); ++iterBead)
    {
        m_vBeads.push_back(dynamic_cast<CBead*>(iterBead->second));
    }

    std::cout << "Nanoparticle " << GetId() << " has " << m_vBeads.size() << " concrete beads connected by " <<  GetBondTotal() << " concrete bonds" << zEndl;

	// Calculate the maximum width of the NP by comparing the distances between all pairs of beads.
	
    zDoubleVector XVector;
    zDoubleVector YVector;
    zDoubleVector ZVector;
    XVector.clear();
    YVector.clear();
    ZVector.clear();
    
    for(BondVectorIterator iterBond = m_vBonds.begin(); iterBond != m_vBonds.end(); iterBond++)
    {
        XVector.push_back((*iterBond)->GetHead()->GetXPos());
        YVector.push_back((*iterBond)->GetHead()->GetYPos());
        ZVector.push_back((*iterBond)->GetHead()->GetZPos());
        
        XVector.push_back((*iterBond)->GetTail()->GetXPos());
        YVector.push_back((*iterBond)->GetTail()->GetYPos());
        ZVector.push_back((*iterBond)->GetTail()->GetZPos());
    }
    
    double oldR = 0.0;
	
    for (long unsigned i=0; i<XVector.size(); i++)
    {
        for (long unsigned j=i;j<XVector.size();j++)
        {
            oldR=sqrt((XVector.at(i)-XVector.at(j))*(XVector.at(i)-XVector.at(j)) + 
		(YVector.at(i)-YVector.at(j))*(YVector.at(i)-YVector.at(j)) + 
		(ZVector.at(i)-ZVector.at(j))*(ZVector.at(i)-ZVector.at(j)));
					  
            if (m_MaxWidth < oldR)
            {
                m_MaxWidth = oldR;
            }
        }
    }
	
    CalculateCM();
    
    std::cout << "Nanoparticle " << GetId() << " has max diam: "<< m_MaxWidth << " and CM: " << GetXCM() << " " << GetYCM() << " " << GetZCM() << zEndl;

}

// Copy constructor for CNanoparticle objects. 
//
// Because CNanoparticle hold vectors of bond pointers we have to define what
// it means to copy a CNanoparticle type into usable CNanoparticle objects.
// We just copy the original bond pointers into the new nanoparticle.

CNanoparticle::CNanoparticle(const CNanoparticle &oldNanoparticle) : m_id(oldNanoparticle.m_id), 
                                                                     m_MaxWidth(oldNanoparticle.m_MaxWidth)
{
    m_vBonds = oldNanoparticle.m_vBonds;
    m_vBeads = oldNanoparticle.m_vBeads;
}

// The CNanoparticle destructor is responsible for destroying its own CBond objects. But note  
// that CBond objects do NOT destroy their beads. As a CNanoparticle is just a wrapper for a set 
// of CBond objects, it is appropriate to destroy the bonds here.

CNanoparticle::~CNanoparticle()
{
	if(!m_vBonds.empty())
	{
		for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
		{
			delete *iterBond;
		}
		m_vBonds.clear();
	}

}

// Functions to calculate the forces on a nanoparticle's beads due to its bonds.

void CNanoparticle::AddBondForces()
{
	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
		(*iterBond)->AddForce();
	}
}

// Function to calculate the Centre of Mass of the nanoparticle. The components are accessed via inline functions
// GetXCM(), etc.

void CNanoparticle::CalculateCM() 
{
    const double beadTotal  = static_cast<double>(GetBeadTotal());

    m_XCM = 0.0;
    m_YCM = 0.0;
    m_ZCM = 0.0;

    for(cBeadVectorIterator citerBead = m_vBeads.begin(); citerBead != m_vBeads.end(); citerBead++)
    {
        const double x = (*citerBead)->GetXPos();
        const double y = (*citerBead)->GetYPos();
        const double z = (*citerBead)->GetZPos();

        m_XCM += x;
        m_YCM += y;
        m_ZCM += z;
    }

    m_XCM /= beadTotal;
    m_YCM /= beadTotal;
    m_ZCM /= beadTotal;
}

// Function to return the X coordinate of the first bond in the NP. If there are no bonds, 
// we return the illegal value of -1.0, but this must be checked for by the calling routine.

double CNanoparticle::GetFirstBondHeadXPos() const
{
    if(!m_vBonds.empty())
	{
	    return m_vBonds.front()->GetHead()->GetXPos();
	}
	else
	{
	    return -1.0;
	}
}

// Function to return the Y coordinate of the first bond in the NP. If there are no bonds, 
// we return the illegal value of -1.0, but this must be checked for by the calling routine.

double CNanoparticle::GetFirstBondHeadYPos() const
{
    if(!m_vBonds.empty())
	{
	    return m_vBonds.front()->GetHead()->GetYPos();
	}
	else
	{
	    return -1.0;
	}
}

// Function to return the Z coordinate of the first bond in the NP. If there are no bonds, 
// we return the illegal value of -1.0, but this must be checked for by the calling routine.

double CNanoparticle::GetFirstBondHeadZPos() const
{
    if(!m_vBonds.empty())
	{
	    return m_vBonds.front()->GetHead()->GetZPos();
	}
	else
	{
	    return -1.0;
	}
}

// Function to add a concrete bond instance to the NP's container.

void CNanoparticle::AddBond(CBond* const pBond)
{
    if(pBond)
    {
        m_vBonds.push_back(pBond);
    }
    
    // Now add the beads taking care to exclude identical beads
    
    if(pBond->GetHead()->GetId() == pBond->GetTail()->GetId())
    {
        m_vBeads.push_back(dynamic_cast<CBead*>(pBond->GetHead()));
    }
    else
    {
        m_vBeads.push_back(dynamic_cast<CBead*>(pBond->GetHead()));
        m_vBeads.push_back(dynamic_cast<CBead*>(pBond->GetTail()));  
    }
}

// Function to remove a concrete bond instance from the NP's container.

CBond* CNanoparticle::RemoveBond(CBond* const pBond)
{    
    return RemoveBond(pBond->GetId());
}

// Overloaded function to remove a concrete bond instance from the NP's container given its id.

CBond* CNanoparticle::RemoveBond(long bondId)
{
    CBond* pFoundBond = 0;
    
    BondVectorIterator iterBond=m_vBonds.begin();
    
    while(!pFoundBond && iterBond!=m_vBonds.end())
    {
        if((*iterBond)->GetId() == bondId)
        {
            pFoundBond = *iterBond;
            m_vBonds.erase(iterBond);
        }
        else
        {
            ++iterBond;
        }
    }
    
    // Now remove the beads as well
    
    for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); ++iterBead)
    {
        if((*iterBead)->GetId() == pFoundBond->GetHead()->GetId() || (*iterBead)->GetId() == pFoundBond->GetTail()->GetId())
        {
            m_vBeads.erase(iterBead);
        }
    }

    return pFoundBond;
}

// Function to check if a specific bead is contained in the nanoparticle. The inefficient way to do this 
// is to iterate over all bonds and check their head tail bead ids. We can improve on this later by caching 
// the unique beads, and so reducing the amount of searching we have to do.

bool CNanoparticle::IsBeadPresent(long beadId)
{
    bool bPresent = 0;
    
    cBondVectorIterator citerBond=m_vBonds.begin(); 
    
    while(!bPresent && citerBond!=m_vBonds.end())
    {
        if((*citerBond)->GetHead()->GetId() == beadId || (*citerBond)->GetTail()->GetId() == beadId)
        {
            bPresent = true;
        }
        else
        {
            ++citerBond;
        }
    }

    return bPresent;
}

// Boolean function that returns true if any part of the nanoparticle lies within the Border CNT cells of the simulation space.
// We check whether the CM of the nanoparticle lies within a distance of the boundaries equal to the maximum width of the nanoparticle.
// We pass in the size of the SimBox and the CNT cell widths.

bool CNanoparticle::IsBorder(double lx, double ly, double lz, double dx, double dy, double dz)
{    
    CalculateCM();  // First calculate the current CM coordinates

    const double x = GetXCM();
    const double y = GetYCM();
    const double z = GetZCM();

    std::cout << "Nanoparticle " << GetId() << " has CM " << x << " " << y << " " << z << zEndl;

    const double xBound = m_MaxWidth + dx;
    const double yBound = m_MaxWidth + dy;
    const double zBound = m_MaxWidth + dz;

    bool bBorder = false;

    if(x < xBound || y < yBound || z < zBound || (lx - x) < xBound || (ly - y) < yBound || (lz - z) < zBound)
    {
        bBorder = true;
    }

    return bBorder;
}

// Function that returns true if the nanoparticle contains only bonds that themselves contain two concrete bead instances.

bool CNanoparticle::IsComplete() const
{
    bool bComplete = true;
    
    cBondVectorIterator citerBond=m_vBonds.begin(); 
    
    while(bComplete && citerBond!=m_vBonds.end())
    {
        if(!(*citerBond)->GetHead() || !(*citerBond)->GetTail())
        {
            bComplete = false;
        }
        else
        {
            ++citerBond;
        }
    }

    return bComplete;
}





