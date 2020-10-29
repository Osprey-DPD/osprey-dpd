/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BeadChargeWrapper.cpp: implementation of the CBeadChargeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BeadChargeWrapper.h"
#include "AbstractBead.h"

#include "SimMathFlags.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// This wrapper class has to receive a CAbstractBead pointer to store as the
// underlying bead. It then adds its force to the contained bead.

CBeadChargeWrapper::CBeadChargeWrapper(CAbstractBead* const pBead, double strength,
									   double range, double kappa) : m_pBead(pBead),
									   m_Strength(strength), m_Range(range), m_Kappa(kappa)
						
{

}

CBeadChargeWrapper::~CBeadChargeWrapper()
{

}

// Function to add the screened charge force to a pair of charged beads.
// Notice that the force law is 
//
//	F(rij) = F(0)exp(-kappa*rij)
//
// where F(0) = strength1*strength2,
//
// and kappa = kappa1 + kappa2.
//
// This avoids us having to store an array of the various bead-bead interaction
// parameters. We have to have friend status for the CAbstractBead class so that
// we can add the resulting forces to the underlying beads.
//
// The bead separation is corrected for the PBCs in CSimBox::AddChargedBeadForces()
// before calling this routine. This is because it is difficult to access the
// SimBox side lengths from the CBeadChargeWrapper class.

void CBeadChargeWrapper::AddForce(CBeadChargeWrapper *const pChargedBead2, double dx[3])
{
	const double dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
	const double dr = sqrt(dr2);
	
	const double force = m_Strength*pChargedBead2->m_Strength/(dr*exp((m_Kappa+pChargedBead2->m_Kappa)*dr));

	// Add the force to the current bead and its interacting partner

	m_pBead->m_Force[0] += force*dx[0]/dr;
	m_pBead->m_Force[1] += force*dx[1]/dr;
	m_pBead->m_Force[2] += force*dx[2]/dr;

	pChargedBead2->m_pBead->m_Force[0] -= force*dx[0]/dr;
	pChargedBead2->m_pBead->m_Force[1] -= force*dx[1]/dr;
	pChargedBead2->m_pBead->m_Force[2] -= force*dx[2]/dr;

}
