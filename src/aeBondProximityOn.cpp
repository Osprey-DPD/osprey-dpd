/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondProximityOn.cpp: implementation of the aeBondProximityOn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeBondProximityOn.h"
#include "aeActiveBond.h"
#include "aeActiveSimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeBondProximityOn::aeBondProximityOn(double max) : m_bAsymmetry(false)
{
	// If only one parameter is specified for the distance at which bonds 
	// form use it for both ends of the filaments and set a flag showing
	// that the bonds are symmetric.
    // If a negative value is passed in we treat it as zero.

    if(max < 0.0)
    {
        max = 0.0;
    }

	m_MaxSeparation[0] = max;
	m_MaxSeparation[1] = max;

	m_MaxSepSquared[0] = max*max;
	m_MaxSepSquared[1] = max*max;
}

aeBondProximityOn::aeBondProximityOn(double maxHead, double maxTail) : m_bAsymmetry(true)
{
	// Allow a different distance to be used for bond addition at the
	// head and tail ends of the filaments, and set a flag showing the
	// bonds are asymmetric.
    // If a negative value is passed in we treat it as zero.

    if(maxHead < 0.0)
    {
        maxHead = 0.0;
    }

    if(maxTail < 0.0)
    {
        maxTail = 0.0;
    }

	m_MaxSeparation[0] = maxHead;		// Bond binds at its head
	m_MaxSeparation[1] = maxTail;		// Bond binds at its tail

	m_MaxSepSquared[0] = maxHead*maxHead;
	m_MaxSepSquared[1] = maxTail*maxTail;
}

aeBondProximityOn::~aeBondProximityOn()
{

}

// Function to check whether a free active bond can attach to an active polymer
// by binding to the terminal bond at the polymer's head. All bond-specific tests. 
// such as whether either bond is a phantom or already polymerised, must be done
// on the aeActiveBond-derived class. Here we just test the distance criterion 
// for connecting two bonds together with a dynamic spring.
//
// This version requires that the tail monomers of the two bonds be within 
// a maximum distance of each other for a single time-step for the bond to be made.
// The distance is measured from the head bead of the polymerised bond to the tail
// bead of the free bond. Only bonds of the same type can be bound together,
// e.g., actin monomers into a filament.
//
// We use a static member function to access the IActiveSimBox interface pointer
// that should be set when the active SimBox is instantiated. If it is null here
// we abort the operation and return false. This means that no bonds will be able
// to bind to the filament, but it is better than crashing the code.

bool aeBondProximityOn::ActivateHead(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const
{
	if(aeActiveSimBox::GetIActiveSimBox())
	{
		const CPolymer* const pFirstTail  = pEnd->GetTailMonomer();
		const CPolymer* const pSecondTail = pFree->GetTailMonomer();

		const CAbstractBead* const pBead = pFirstTail->GetHead();
		const CAbstractBead* const pTargetBead = pSecondTail->GetTail();

		double dx = pBead->GetXPos() - pTargetBead->GetXPos();
		double dy = pBead->GetYPos() - pTargetBead->GetYPos();
		double dz = pBead->GetZPos() - pTargetBead->GetZPos();
		
		if( dx > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();
		else if( dx < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();

		if( dy > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();
		else if( dy < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();

	#if SimDimension == 3
		if( dz > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
		else if( dz < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
	#else
		dz = 0.0;
	#endif

		const double dsq = dx*dx + dy*dy + dz*dz;
		
		if(dsq > 64.0)
		{
			std::cout << "error: monomer is more than1/2 box size away" << zEndl;
		}

		// If the on rates are asymmetric, we determine if the pEnd bond,
		// which is at the head of the filament, can bind to the free bond
		// via its head. If there is no asymmetry we just use the first value.

		if(m_bAsymmetry)
		{
			if(dsq < m_MaxSepSquared[0])
			{
				return true;
			}
		}
		else
		{
			if(dsq < m_MaxSepSquared[0])
			{
				return true;
			}
		}
	}

	return false;
}

// Function to check whether a free active bond can attach to an active polymer
// by binding to the terminal bond at the polymer's tail. All bond-specific tests. 
// such as whether either bond is a phantom or already polymerised, must be done
// on the aeActiveBond-derived class. Here we just test the distance criterion 
// for connecting two bonds together with a dynamic spring.
//
// This version requires that the tail monomers of the two bonds be within 
// a maximum distance of each other for a single time-step for the bond to be made.
// The distance is measured from the tail bead of the polymerised bond to the head
// bead of the free bond. Only bonds of the same type can be bound together,
// e.g., actin monomers into a filament.

bool aeBondProximityOn::ActivateTail(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const
{
	if(aeActiveSimBox::GetIActiveSimBox())
	{
        const CPolymer* const pFirstTail  = pEnd->GetTailMonomer();
		const CPolymer* const pSecondTail = pFree->GetTailMonomer();

		const CAbstractBead* const pBead = pFirstTail->GetTail();
		const CAbstractBead* const pTargetBead = pSecondTail->GetHead();

		double dx = pBead->GetXPos() - pTargetBead->GetXPos();
		double dy = pBead->GetYPos() - pTargetBead->GetYPos();
		double dz = pBead->GetZPos() - pTargetBead->GetZPos();

		if( dx > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();
		else if( dx < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();

		if( dy > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();
		else if( dy < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();

		#if SimDimension == 3
			if( dz > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
				dz = dz - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
			else if( dz < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
				dz = dz + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
		#else
			dz = 0.0;
		#endif

		const double dsq = dx*dx + dy*dy + dz*dz;

		// If the on rates are asymmetric, we determine if the pEnd bond,
		// which is at the tail of the filament, can bind to the free bond
		// via its tail. If there is no asymmetry we just use the first value.

		if(m_bAsymmetry)
		{
			if(dsq < m_MaxSepSquared[1])
			{
				return true;
			}
		}
		else
		{
			if(dsq < m_MaxSepSquared[0])
			{
				return true;
			}
		}
	}

	return false;
}

// Function to set the maximum distance at which two active bonds can 
// associate, given a simple proximity test. This version of the function
// uses the same parameter for bonds at both ends of a filament.

void aeBondProximityOn::SetSeparation(double max)
{
	if(max > 0.0)
	{
		m_MaxSeparation[0] = max;
		m_MaxSeparation[1] = max;

		m_MaxSepSquared[0] = max*max;
		m_MaxSepSquared[1] = max*max;
	}
    else
    {
		m_MaxSeparation[0] = 0.0;
		m_MaxSeparation[1] = 0.0;

		m_MaxSepSquared[0] = 0.0;
		m_MaxSepSquared[1] = 0.0;
    }
}

// Functions to set the maximum distance at which two active bonds can 
// associate via the head of the terminal bond of a filament, given a 
// simple proximity test. These versions allow different distances to be 
// specified for bonds joining at opposite ends of the filaments.

void aeBondProximityOn::SetHeadSeparation(double max)
{
	if(max > 0.0)
	{
		m_MaxSeparation[0] = max;
		m_MaxSepSquared[0] = max*max;
	}
    else
	{
		m_MaxSeparation[0] = 0.0;
		m_MaxSepSquared[0] = 0.0;
	}
}

void aeBondProximityOn::SetTailSeparation(double max)
{
	if(max > 0.0)
	{
		m_MaxSeparation[1] = max;
		m_MaxSepSquared[1] = max*max;
	}
    else
    {
		m_MaxSeparation[1] = 0.0;
		m_MaxSepSquared[1] = 0.0;
	}
}

