/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BondPair.cpp: implementation of the CBondPair class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBondPair::CBondPair() : m_id(0), m_Type(0), m_Modulus(0.0), m_Phi0(0.0), 
						 m_CosPhi0(0.0), m_SinPhi0(0.0), m_Prefactor(0.0),
						 m_FirstBondIndex(0), m_SecondBondIndex(0),
						 m_pFirst(NULL), m_pSecond(NULL),
						 m_P2(0.0), m_FirstLength(0.0), m_SecondLength(0.0), 
						 m_PhiDiff(0.0), m_PhiDiffSq(0.0)
{
	for(short int i=0; i<3; i++)
	{
		m_BeadXForce[i] = 0.0;
		m_BeadYForce[i] = 0.0;
		m_BeadZForce[i] = 0.0;
	}
}


CBondPair::CBondPair(long type, double strength, double phi0) : m_id(-1), 
					m_Type(type), m_Modulus(strength), m_Phi0(phi0), 
					m_CosPhi0(cos(phi0)), m_SinPhi0(sin(phi0)), m_Prefactor(0.0),
					m_FirstBondIndex(0), m_SecondBondIndex(0),
					m_pFirst(NULL), m_pSecond(NULL),
					m_P2(0.0), m_FirstLength(0.0), m_SecondLength(0.0), 
					m_PhiDiff(0.0), m_PhiDiffSq(0.0)

{
	for(short int i=0; i<3; i++)
	{
		m_BeadXForce[i] = 0.0;
		m_BeadYForce[i] = 0.0;
		m_BeadZForce[i] = 0.0;
	}
}


// Copy constructor for CBondPair objects. 
//
// Note that we cannot set the pointers to the actual bonds
// because they will be different for every bond. But the indices into the 
// polymer bead array will be the same for the equivalent bonds in polymers of the 
// same type.

CBondPair::CBondPair(const CBondPair &oldBondPair)
{
	m_id				= oldBondPair.m_id;
	m_Type				= oldBondPair.m_Type;
	m_Modulus	        = oldBondPair.m_Modulus;
	m_Phi0				= oldBondPair.m_Phi0;
	m_CosPhi0			= oldBondPair.m_CosPhi0;
	m_SinPhi0			= oldBondPair.m_SinPhi0;
	m_Prefactor			= oldBondPair.m_Prefactor;
	m_FirstBondIndex	= oldBondPair.m_FirstBondIndex;
	m_SecondBondIndex	= oldBondPair.m_SecondBondIndex;
	m_pFirst			= NULL;
	m_pSecond			= NULL;
	m_P2				= oldBondPair.m_P2;
	m_FirstLength		= oldBondPair.m_FirstLength;
	m_SecondLength		= oldBondPair.m_SecondLength;
	m_PhiDiff			= oldBondPair.m_PhiDiff;
	m_PhiDiffSq			= oldBondPair.m_PhiDiffSq;

	for(short int i=0; i<3; i++)
	{
		m_BeadXForce[i] = oldBondPair.m_BeadXForce[i];
		m_BeadYForce[i] = oldBondPair.m_BeadYForce[i];
		m_BeadZForce[i] = oldBondPair.m_BeadZForce[i];
	}

}

// The CBondPair destructor does not need to destroy its bonds because this is done
// by the polymer to which they belond.

CBondPair::~CBondPair()
{

}

// Function to store pointers to the bonds comprising the CBondPair. The bonds are
// expected to be in the order in which they occur in the polymer and to have
// their head-tail vectors in the same direction, i.e., the first bond's tail bead
// is the second bond's head bead.

void CBondPair::SetBonds(CBond *pFirst, CBond *pSecond)
{
	m_pFirst  = pFirst;
	m_pSecond = pSecond;
}

// Function to calculate the bending force between two adjacent bonds in a polymer.
// This represents a 3-body force between the beads comprising the two bonds.
// We have friend access to the CBond objects to get the bond lengths and components,
// and to the CAbstractBead, CBead classes to get the bead coordinates.
//
// The force law is:	F() = Strength*(1 - cos(b1.b2 - phi0))
//
// where b1, b2 are unit vectors in the directions of the bonds; phi0 is the
// preferred bond angle, and strength is the bending modulus of the bonds.
//
// Note the convention that the beads are labelled 1,2,3 starting at the tail of
// the first bond; and the bonds are labelled 1 and 2 in the same direction. 
// The bonds are taken from the polymer string in the opposite order to that in
// which they occur. This is to ensure that the direction of the bonds, calculated
// in CBond::AddForce() leads to the correct sign for the cosine calculation.
// The central bead is the one that links the bonds in the CBondPair object as 
// it is common to both bonds. We define the magnitude of the force on the middle
// bead so as to make the total force on all three beads zero, leaving only a 
// torque tending to make the bonds straight.

void CBondPair::AddForce()
{
	// Store the bond lengths for later use and analysis, and zero the other observables

	m_FirstLength   = m_pFirst->m_Length;
	m_SecondLength  = m_pSecond->m_Length;
	m_P2			= 0.0;
	m_PhiDiff		= 0.0;
	m_PhiDiffSq		= 0.0;

	const double magProduct = m_FirstLength*m_SecondLength;

	// Calculate the angle between the bonds: if either bond length is zero
	// then we set the forces to zero.  Note that the bond vectors are defined
	// in the direction: head - tail.

	if(magProduct > 0.0001)
	{
		double first[3];
		first[0] = m_pFirst->m_dx;
		first[1] = m_pFirst->m_dy;
		first[2] = m_pFirst->m_dz;

		double second[3];
		second[0] = m_pSecond->m_dx;
		second[1] = m_pSecond->m_dy;
		second[2] = m_pSecond->m_dz;

		const double b1MagSq		= m_FirstLength*m_FirstLength;
		const double b2MagSq		= m_SecondLength*m_SecondLength;
		const double b1Dotb2		= first[0]*second[0] + first[1]*second[1] + first[2]*second[2];
		const double b1b2Overb1Sq	= b1Dotb2/b1MagSq;
		const double b1b2Overb2Sq	= b1Dotb2/b2MagSq;
		const double cosPhiSq		= b1b2Overb1Sq*b1b2Overb2Sq;

		double forceMag = 0.0;

		// Check that the bond angle is not exactly 90 deg but allow the cosine to be < 0

		if(fabs(b1Dotb2) > 0.000001)
		{
			m_Prefactor = sqrt(1.0/cosPhiSq - 1.0);
			
			// Add the restoring force depending on whether there is a preferred angle
			// for the bond pair or not

			if(m_Phi0 > 0.0)
			{
				forceMag = m_Modulus*(m_CosPhi0 - m_SinPhi0/m_Prefactor)/magProduct;
				m_P2     = 0.5*(3.0*cosPhiSq*(m_CosPhi0 + m_SinPhi0*m_Prefactor) - 1.0);
			}
			else
			{
				forceMag = m_Modulus/magProduct;
				m_P2	 = 0.5*(3.0*cosPhiSq - 1.0);
			}
		}
		else
		{
			forceMag	= m_Modulus/magProduct;
			m_P2		= -0.5;
		}

		// These observables may not be used as they will be expensive to calculation,
		// but they might be useful.

		m_PhiDiff	= acos(cosPhiSq) - m_Phi0;
		m_PhiDiffSq	= m_PhiDiff*m_PhiDiff;

/*
		// Debug output of forces prior to stiff bonds
		double oldXForce1 = m_pFirst->m_pTail->m_Force[0];
		double oldYForce1 = m_pFirst->m_pTail->m_Force[1];
		double oldZForce1 = m_pFirst->m_pTail->m_Force[2];

		double oldXForce2 = m_pFirst->m_pHead->m_Force[0];
		double oldYForce2 = m_pFirst->m_pHead->m_Force[1];
		double oldZForce2 = m_pFirst->m_pHead->m_Force[2];

		double oldXForce3 = m_pSecond->m_pTail->m_Force[0];
		double oldYForce3 = m_pSecond->m_pTail->m_Force[1];
		double oldZForce3 = m_pSecond->m_pTail->m_Force[2];

		double oldXForce4 = m_pSecond->m_pHead->m_Force[0];
		double oldYForce4 = m_pSecond->m_pHead->m_Force[1];
		double oldZForce4 = m_pSecond->m_pHead->m_Force[2];
*/


		// Store the force on the first bead in both the first and the last
		// elements of the array to simplify calculating the stress tensor
		// contribution in CMonitor::AddBondPairStress().

		m_BeadXForce[0] = forceMag*(b1b2Overb1Sq*first[0] - second[0]);
		m_BeadYForce[0] = forceMag*(b1b2Overb1Sq*first[1] - second[1]);
		m_BeadZForce[0] = forceMag*(b1b2Overb1Sq*first[2] - second[2]);

		m_BeadXForce[3] = m_BeadXForce[0];
		m_BeadYForce[3] = m_BeadYForce[0];
		m_BeadZForce[3] = m_BeadZForce[0];

		m_pFirst->m_pTail->m_Force[0] += m_BeadXForce[0];
		m_pFirst->m_pTail->m_Force[1] += m_BeadYForce[0];
		m_pFirst->m_pTail->m_Force[2] += m_BeadZForce[0];

		// Store the force on the third bead for use in calculating stresses

		m_BeadXForce[2] = forceMag*(first[0] - b1b2Overb2Sq*second[0]);
		m_BeadYForce[2] = forceMag*(first[1] - b1b2Overb2Sq*second[1]);
		m_BeadZForce[2] = forceMag*(first[2] - b1b2Overb2Sq*second[2]);

		m_pSecond->m_pHead->m_Force[0] += m_BeadXForce[2];
		m_pSecond->m_pHead->m_Force[1] += m_BeadYForce[2];
		m_pSecond->m_pHead->m_Force[2] += m_BeadZForce[2];

		// Restoring force on the middle bead is chosen so as to make the 
		// total force generated by the bond stiffness vanish, hence the
		// minus sign. Note that the first bond's head bead is the same 
		// as the second's tail.

		m_BeadXForce[1] = -m_BeadXForce[0] - m_BeadXForce[2];
		m_BeadYForce[1] = -m_BeadYForce[0] - m_BeadYForce[2];
		m_BeadZForce[1] = -m_BeadZForce[0] - m_BeadZForce[2];

		m_pFirst->m_pHead->m_Force[0] += m_BeadXForce[1];
		m_pFirst->m_pHead->m_Force[1] += m_BeadYForce[1];
		m_pFirst->m_pHead->m_Force[2] += m_BeadZForce[1];

/*
		// Debug output of new forces
		double newXForce1 = m_pFirst->m_pTail->m_Force[0];
		double newYForce1 = m_pFirst->m_pTail->m_Force[1];
		double newZForce1 = m_pFirst->m_pTail->m_Force[2];

		double newXForce2 = m_pFirst->m_pHead->m_Force[0];
		double newYForce2 = m_pFirst->m_pHead->m_Force[1];
		double newZForce2 = m_pFirst->m_pHead->m_Force[2];

		double newXForce3 = m_pSecond->m_pTail->m_Force[0];
		double newYForce3 = m_pSecond->m_pTail->m_Force[1];
		double newZForce3 = m_pSecond->m_pTail->m_Force[2];

		double newXForce4 = m_pSecond->m_pHead->m_Force[0];
		double newYForce4 = m_pSecond->m_pHead->m_Force[1];
		double newZForce4 = m_pSecond->m_pHead->m_Force[2];
*/

	}
}

// Function to add an index to the first and second bond in the bondpair
// so that they can be duplicated when creating polymers. The indicies for
// the bonds in a polymer depend only on their location in the polymer's
// shape not on the identity of the bonds.

void CBondPair::SetBondIndex(long firstBondIndex, long secondBondIndex)
{
	m_FirstBondIndex  = firstBondIndex;
	m_SecondBondIndex = secondBondIndex;
}

// Functions to set the values of the stiff bond's bending modulus and
// preferred angle. We don't check that the new values are valid because
// these functions are expected to be only called by CInputData which validates
// the input data itself.

void CBondPair::SetModulus(double newModulus)
{
	m_Modulus = newModulus;
}

void CBondPair::SetPhi0(double newPhi0)
{
	m_Phi0 = newPhi0;
}

// Function to calculate the potential energy stored in the stiff bond.
//
// We assume the lengths of the two bonds have already been calculated, 
// so the CBond::AddPotentialEnergy() function must have been called before 
// this one.

double CBondPair::AddPotentialEnergy()
{
	// Store the bond lengths for later use and analysis

	m_FirstLength   = m_pFirst->m_Length;
	m_SecondLength  = m_pSecond->m_Length;

	const double magProduct = m_FirstLength*m_SecondLength;

	// Calculate the angle between the bonds: if either bond length is zero
	// then we set the energy to zero.  Note that the bond vectors are defined
	// in the direction: head - tail.

	double pe = 0.0;

	if(magProduct > 0.0001)
	{
		const double b1Dotb2 = m_pFirst->m_dx*m_pSecond->m_dx + 
							   m_pFirst->m_dy*m_pSecond->m_dy + 
							   m_pFirst->m_dz*m_pSecond->m_dz;

		const double cosPhiSq = (b1Dotb2*b1Dotb2)/(magProduct*magProduct);

		// Note that sinPhi is always positive for 0 < phi < 180 deg.

		const double sinPhi = sqrt(1.0 - cosPhiSq);

		// Check that the bond angle is not exactly 90 deg but allow the cosine to be < 0

		if(b1Dotb2 > 0.000001)			// phi < 90 deg
		{
			if(m_Phi0 > 0.0)
			{
				pe = m_Modulus*(1.0 - sqrt(cosPhiSq)*m_CosPhi0 - sinPhi*m_SinPhi0);
			}
			else
			{
				pe = m_Modulus*(1.0 - sqrt(cosPhiSq));
			}
		}
		else if(b1Dotb2 < -0.000001)	// phi > 90 deg
		{
			if(m_Phi0 > 0.0)
			{
				pe = m_Modulus*(1.0 + sqrt(cosPhiSq)*m_CosPhi0 - sinPhi*m_SinPhi0);
			}
			else
			{
				pe = m_Modulus*(1.0 + sqrt(cosPhiSq));
			}
		}
		else							// phi = 90 deg
		{
			pe = m_Modulus*(1.0 - m_SinPhi0);
		}
	}

	return pe;
}
