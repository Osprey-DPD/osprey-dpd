/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aaStressTensor1d.cpp: implementation of the aaStressTensor1d class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aaStressTensor1d.h"
#include "AbstractBead.h"
#include "SliceProfile.h"
#include "ISimBox.h"
#include "Monitor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We pass in the CSliceProfile containing the bead lists for each region
// and the normal to the slice profile. This class represents the 
// stress tensor calculated as an average over a 1-d set of slices through 
// the SimBox. Other classes derived from aaStressTensor allow averages over 
// rows and cells.

aaStressTensor1d::aaStressTensor1d(const ISimBox* const pISimBox, CSliceProfile* const pProfile, 
								   long xn, long yn, long zn) : aaStressTensor(pISimBox, xn, yn, zn),
																m_pProfile(pProfile)
													 
{
	// We store the normal as an index and define the size of the stress grid, which
	// is derived from the CMonitor::m_GridXCellNo, etc, members.

	if(xn == 1)
	{
		m_NormalIndex		= 0;
		m_StressGridSize	= m_pISimBox->GetGridXCellNo();
		m_Width				= m_pISimBox->GetGridXCellWidth();
	}
	else if(yn == 1)
	{
		m_NormalIndex		= 1;
		m_StressGridSize	= m_pISimBox->GetGridYCellNo();
		m_Width				= m_pISimBox->GetGridYCellWidth();
	}
	else if(zn == 1)
	{
		m_NormalIndex		= 2;
		m_StressGridSize	= m_pISimBox->GetGridZCellNo();
		m_Width				= m_pISimBox->GetGridZCellWidth();
	}

	// Store the normalisation factors to prevent having to do the division repeatedly

	m_Volume	  = (*m_pProfile->Begin())->GetWidth()*(*m_pProfile->Begin())->GetDepth()*m_Width;
	m_SliceFactor = 1.0/m_Volume;

	for(short i=0; i<9; i++)
	{
		m_Config[i]		= 0.0;
		m_Kinetic[i]	= 0.0;
	}
}

// We don't delete the slice profile as it was not created in the constructor.
// It was copied from the (for example, CBilayer) analysis class by pointer.

aaStressTensor1d::~aaStressTensor1d()
{

}

// Virtual function to calculate the stress tensor averaged over a 1d vector of 
// slices through the SimBox. Note that the CSliceProfile is
// stored in this class but the resulting stress tensor profile is stored in 
// the aaStressTensor base class. The base class does not know what profiles are
// used by the derived classes (slice, row or cell or single slices) so we cannot 
// store them there, but the resulting aaStressTensorPoint profiles can.
//
// Each region in the profile is used in CMonitor to calculate the stress tensor 
// averaged over that region: the resulting value is stored in an aaStressTensorPoint 
// object.
// 
//  m_Config[]		Components of stress tensor due to bead interactions averaged over slice
//  m_Kinetic[]		Components of stress tensor due to bead momenta averaged over slice

void aaStressTensor1d::CalculateProfile()
{
	// Loop over the slices in the profile creating an aaStressTensorPoint object
	// and storing in it the stress tensor averaged over each slice. We use a binning 
	// procedure to allocate the bead-bead interaction contributions to the 
	// appropriate slice.
	// Note that this loop is over stress grid slices not CNT slices.

	long sliceIndex = 0;

	for(sliceIndex = 0; sliceIndex < m_StressGridSize; sliceIndex++)
	{
		// Create an empty aaStressTensorPoint 

		AddStressTensorPoint();

		// Forget the kinetic contribution for now

		// I need to use a copy of the bead list because the loop over beads
		// seems to fail if I try to use the GetBeads() function in the loop.

/*		const BeadList& rBeads = m_pProfile->GetRegion(CNTSliceIndex)->GetBeads();

		for(cBeadListIterator citerBead=rBeads.begin(); citerBead!=rBeads.end(); citerBead++)
		{
			CAbstractBead* const pBead = *citerBead;

			// Add the bead momentum to the kinetic part of the stress tensor:
			// NOTE the minus sign in the definition is postponed until
			// the kinetic and configurational contributions are added in aaStressTensorPoint.

			m_Kinetic[0] += pBead->m_Mom[0]*pBead->m_Mom[0];
			m_Kinetic[1] += pBead->m_Mom[0]*pBead->m_Mom[1];
			m_Kinetic[2] += pBead->m_Mom[0]*pBead->m_Mom[2];
			m_Kinetic[4] += pBead->m_Mom[1]*pBead->m_Mom[1];
			m_Kinetic[5] += pBead->m_Mom[1]*pBead->m_Mom[2];
			m_Kinetic[8] += pBead->m_Mom[2]*pBead->m_Mom[2];

			// Define the components of the kinetic contribution determined by symmetry

			m_Kinetic[3] = m_Kinetic[1];
			m_Kinetic[6] = m_Kinetic[2];
			m_Kinetic[7] = m_Kinetic[5];
		}
*/
		// Do the configurational contribution

		// Get the configurational contribution to the stress for all the
		// bead-bead pair types, bond forces and bondpair forces from the CMonitor. 
		// Note that the 1/(z2-z1) term has been included in the calculations, 
		// and we indicate this by having "slice" in the name of the arrays.
		// The contributions are stored in the aaStressTensorPoint objects
		// in the order: bead-bead then bond then bondpair.
		
		for(long beadIndex=0; beadIndex<m_BeadPairTypeTotal; beadIndex++)
		{
			m_Config[0] = m_pISimBox->GetMonitor()->GetSliceStress11(sliceIndex, beadIndex);
			m_Config[1] = m_pISimBox->GetMonitor()->GetSliceStress12(sliceIndex, beadIndex);
			m_Config[2] = m_pISimBox->GetMonitor()->GetSliceStress13(sliceIndex, beadIndex);
			m_Config[3] = m_pISimBox->GetMonitor()->GetSliceStress21(sliceIndex, beadIndex);
			m_Config[4] = m_pISimBox->GetMonitor()->GetSliceStress22(sliceIndex, beadIndex);
			m_Config[5] = m_pISimBox->GetMonitor()->GetSliceStress23(sliceIndex, beadIndex);
			m_Config[6] = m_pISimBox->GetMonitor()->GetSliceStress31(sliceIndex, beadIndex);
			m_Config[7] = m_pISimBox->GetMonitor()->GetSliceStress32(sliceIndex, beadIndex);
			m_Config[8] = m_pISimBox->GetMonitor()->GetSliceStress33(sliceIndex, beadIndex);

			AddBeadStress(sliceIndex, beadIndex, m_Config, m_Kinetic);
		}

		// Now the bond stress.

		for(long bondIndex=0; bondIndex<m_BondTypeTotal; bondIndex++)
		{
			m_Config[0] = m_pISimBox->GetMonitor()->GetSliceStressBond11(sliceIndex, bondIndex);
			m_Config[1] = m_pISimBox->GetMonitor()->GetSliceStressBond12(sliceIndex, bondIndex);
			m_Config[2] = m_pISimBox->GetMonitor()->GetSliceStressBond13(sliceIndex, bondIndex);
			m_Config[3] = m_pISimBox->GetMonitor()->GetSliceStressBond21(sliceIndex, bondIndex);
			m_Config[4] = m_pISimBox->GetMonitor()->GetSliceStressBond22(sliceIndex, bondIndex);
			m_Config[5] = m_pISimBox->GetMonitor()->GetSliceStressBond23(sliceIndex, bondIndex);
			m_Config[6] = m_pISimBox->GetMonitor()->GetSliceStressBond31(sliceIndex, bondIndex);
			m_Config[7] = m_pISimBox->GetMonitor()->GetSliceStressBond32(sliceIndex, bondIndex);
			m_Config[8] = m_pISimBox->GetMonitor()->GetSliceStressBond33(sliceIndex, bondIndex);

			AddBondStress(sliceIndex, bondIndex, m_Config, m_Kinetic);
		}

		// Now the 3-body bondpair stress. Note that the factor of 1/3 that comes from
		// the sum over the three bead pairs in the 3-body force is now in the Monitor function
		// AddBondPairStress.

		for(long bondPairIndex=0; bondPairIndex<m_BondPairTypeTotal; bondPairIndex++)
		{
			m_Config[0] = m_pISimBox->GetMonitor()->GetSliceStressBondPair11(sliceIndex, bondPairIndex);
			m_Config[1] = m_pISimBox->GetMonitor()->GetSliceStressBondPair12(sliceIndex, bondPairIndex);
			m_Config[2] = m_pISimBox->GetMonitor()->GetSliceStressBondPair13(sliceIndex, bondPairIndex);
			m_Config[3] = m_pISimBox->GetMonitor()->GetSliceStressBondPair21(sliceIndex, bondPairIndex);
			m_Config[4] = m_pISimBox->GetMonitor()->GetSliceStressBondPair22(sliceIndex, bondPairIndex);
			m_Config[5] = m_pISimBox->GetMonitor()->GetSliceStressBondPair23(sliceIndex, bondPairIndex);
			m_Config[6] = m_pISimBox->GetMonitor()->GetSliceStressBondPair31(sliceIndex, bondPairIndex);
			m_Config[7] = m_pISimBox->GetMonitor()->GetSliceStressBondPair32(sliceIndex, bondPairIndex);
			m_Config[8] = m_pISimBox->GetMonitor()->GetSliceStressBondPair33(sliceIndex, bondPairIndex);

			AddBondPairStress(sliceIndex, bondPairIndex, m_Config, m_Kinetic);
		}

	}

	// Multiply the configurational and kinetic contributions to the stress tensor 
	// by the factors arising from the volume of the slices.
	// We pass in 0 to ignore the kinetic contribution, although it too can be
	// scaled if required.

	MultiplyStressTensor(m_SliceFactor, 0.0);
}

