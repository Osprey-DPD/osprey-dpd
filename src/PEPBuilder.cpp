/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// PEPBuilder.cpp: implementation of the CPEPBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "PEPBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"
#include "PolymerCrossLink.h"

#include "CNTCell.h"			// needed for random number generator



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPEPBuilder::CPEPBuilder(const zLongVector&  blmPolymerTypes,
			const zLongVector&	blmReservoirTypes,
			const zDoubleVector&  blmUpperFractions,
			const PolymerCrossLinkVector&  blmCrossLinks,
		        long blmx, long blmy, long blmz,
		        double blmCentre, double blmThickness,
			bool bblmLinear, const bool bblmPatches[2], bool bblmPolymerise,
			double area, double l1, double l2, double bilayerCentre, 
			double upperHead, double lowerHead) : m_BLMPolymerTypes(blmPolymerTypes),
								m_BLMReservoirTypes(blmReservoirTypes),
								m_BLMUpperFractions(blmUpperFractions),
								m_BLMCrossLinks(blmCrossLinks),
								m_BLMX(blmx), m_BLMY(blmy), m_BLMZ(blmz),
								m_BLMCentre(blmCentre),
								m_BLMThickness(blmThickness),
								m_bBLMLinearise(bblmLinear),
								m_bBLMPolymerise(bblmPolymerise),
								m_BilayerArea(area),
								m_Bilayer1Length(l1),
								m_Bilayer2Length(l2),
								m_BilayerCentre(bilayerCentre),
								m_BilayerUpperHead(upperHead),
								m_BilayerLowerHead(lowerHead),
								m_bBLMMonolayer(false),
								m_BLMPolymerTotal(0),
								m_BLMUpperTotal(0),
								m_BLMLowerTotal(0)
{
	m_bBLMPatches[0] = bblmPatches[0];
	m_bBLMPatches[1] = bblmPatches[1];
}

CPEPBuilder::~CPEPBuilder()
{

}

// Virtual function to assemble the polymerised-particle and surface initial state.
//

bool CPEPBuilder::Assemble(CInitialState& riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return true;
}

