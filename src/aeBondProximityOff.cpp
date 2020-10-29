/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondProximityOff.cpp: implementation of the aeBondProximityOff class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeBondProximityOff.h"
#include "aeActiveBond.h"
#include "aeActiveSimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeBondProximityOff::aeBondProximityOff(double max) : m_bAsymmetry(false)
{
	// If only one parameter is specified for the distance at which bonds 
	// dissociate use it for both ends of the bond and set a flag showing
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

aeBondProximityOff::aeBondProximityOff(double maxHead, double maxTail) : m_bAsymmetry(true)
{
	// Allow a different distance to be used for bond dissociation at its
	// head and tail ends, and set a flag showing the bonds are asymmetric.
    // If a negative value is passed in we treat it as zero.

    if(maxHead < 0.0)
    {
        maxHead = 0.0;
    }

    if(maxTail < 0.0)
    {
        maxTail = 0.0;
    }

	m_MaxSeparation[0] = maxHead;
	m_MaxSeparation[1] = maxTail;

	m_MaxSepSquared[0] = maxHead*maxHead;
	m_MaxSepSquared[1] = maxTail*maxTail;
}

aeBondProximityOff::~aeBondProximityOff()
{

}


// Function to check whether an active bond at the tail end of a polymer 
// can dissociate from the adjacent bond connected to its head.
// 
// This version requires that the tail monomers of the two bonds be beyond 
// a maximum distance of each other for a single time-step for the bond 
// to break. The distance is measured from the head bead of the tail bond 
// to the tail bead of the head bond. 

bool aeBondProximityOff::ActivateHead(const aeActiveBond* const pEnd) const
{
	if(aeActiveSimBox::GetIActiveSimBox())
	{
		const CAbstractBead* const pHeadBead = pEnd->GetHeadTailBead();
		const CAbstractBead* const pTailBead = pEnd->GetTailHeadBead();

		double dx = pHeadBead->GetXPos() - pTailBead->GetXPos();
		double dy = pHeadBead->GetYPos() - pTailBead->GetYPos();
		double dz = pHeadBead->GetZPos() - pTailBead->GetZPos();

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

		// If the off rates are asymmetric use the condition appropriate to the tail bond
		// of a polymer dissociating from the adjacent bond at its head. If there is no
		// asymmetry we just use the first value for the separation.

		if(m_bAsymmetry)
		{
			if(dsq > m_MaxSepSquared[0])
			{
				return true;
			}
		}
		else
		{
			if(dsq > m_MaxSepSquared[0])
			{
				return true;
			}
		}
	}

	return false;
}

// Function to check whether an active bond at the head end of a polymer 
// can dissociate from the adjacent bond connected to its tail.
// 
// This version requires that the tail monomers of the two bonds be beyond 
// a maximum distance of each other for a single time-step for the bond 
// to break. The distance is measured from the head bead of the tail bond 
// to the tail bead of the head bond. 

bool aeBondProximityOff::ActivateTail(const aeActiveBond* const pEnd) const
{
	if(aeActiveSimBox::GetIActiveSimBox())
	{
		const CAbstractBead* const pHeadBead = pEnd->GetTailTailBead();
		const CAbstractBead* const pTailBead = pEnd->GetTailAdjacentBond()->GetTailHeadBead();

		double dx = pHeadBead->GetXPos() - pTailBead->GetXPos();
		double dy = pHeadBead->GetYPos() - pTailBead->GetYPos();
		double dz = pHeadBead->GetZPos() - pTailBead->GetZPos();

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

		// If the off rates are asymmetric use the condition appropriate to the tail bond
		// of a polymer dissociating from the adjacent bond at its tail. If there is no
		// asymmetry we just use the first value for the separation.

		if(m_bAsymmetry)
		{
			if(dsq > m_MaxSepSquared[1])
			{
				return true;
			}
		}
		else
		{
			if(dsq > m_MaxSepSquared[0])
			{
				return true;
			}
		}
	}

	return false;
}

// Function to set the maximum distance at which two active bonds can 
// dissociate, given a simple proximity test.

void aeBondProximityOff::SetSeparation(double max)
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
// dissociate via the head of the terminal bond of a filament, given a 
// simple proximity test. These versions allow different distances to be 
// specified for bonds joining at opposite ends of the filaments.

void aeBondProximityOff::SetHeadSeparation(double max)
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

void aeBondProximityOff::SetTailSeparation(double max)
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


