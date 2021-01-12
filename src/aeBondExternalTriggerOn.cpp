/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondExternalTriggerOn.cpp: implementation of the aeBondExternalTriggerOn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeBondExternalTriggerOn.h"
#include "aeActiveBond.h"
#include "aeActiveSimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeBondExternalTriggerOn::aeBondExternalTriggerOn()
{

}

aeBondExternalTriggerOn::~aeBondExternalTriggerOn()
{

}

// Function to check whether a free active bond can attach to an active polymer
// by binding to the terminal bond at the polymer's head.
//
// This version forces a bond to bind when it receives an external trigger. It is
// used to allow one ACN to control the binding of its bonds with those of another
// ACN.

bool aeBondExternalTriggerOn::ActivateHead(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const
{
/*
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

		std::cout << pEnd->CountHeadAdjacentBonds() << " " << pEnd->CountTailAdjacentBonds() << " bonds to head/tail" << zEndl;
	}
*/
	return false;
}

// Function to check whether a free active bond can attach to an active polymer
// by binding to the terminal bond at the polymer's tail.
//
// This version requires that the tail monomers of the two bonds be within 
// a maximum distance of each other for a single time-step for the bond to be made.
// The distance is measured from the tail bead of the polymerised bond to the head
// bead of the free bond. Only bonds of the same type can be bound together,
// e.g., actin monomers into a filament.

bool aeBondExternalTriggerOn::ActivateTail(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const
{
/*
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

		std::cout << pEnd->CountHeadAdjacentBonds() << " " << pEnd->CountTailAdjacentBonds() << " bonds to head/tail" << zEndl;
	}
*/
	return false;
}

