/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Bilayer.cpp: implementation of the CBilayer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimMiscellaneousFlags.h"
#include "Bilayer.h"		
#include "BilayerData.h"
#include "SimState.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "aaRegionToType.h"

#include "SliceProfile.h"
#include "RowProfileSet.h"
#include "CellProfileSet.h"

#include "aaScalarSingle.h"				// Types of data that can be sent as observables
#include "aaVectorSingle.h"
#include "aaScalarProfile.h"
#include "aaVectorProfile.h"

#include "ScalarObservable.h"			// Types of observables
#include "VectorObservable.h"
#include "ScalarProfileObservable.h"

#include "TimeSeriesData.h"
#include "AnalysisTool.h"				// Need this if no tools are defined
#include "FT1d.h"

#include "aaStressTensor1d.h"			// Stress tensor profile object
#include "aaStressTensorPoint.h"		// Actual stress tensor components at a point



	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We pass the CAnalysisData-derived object to the CAnalysis base class for 
// initialisation of all general analysis data. The pointer is passed as
// const as we do not expect it to be altered.

CBilayer::CBilayer(const zString runId, long samplePeriod, 
				   const CBilayerData* const pAD) : CAnalysis(runId, samplePeriod, pAD),
												    m_Polymer(pAD->m_Polymer),
													m_X(pAD->m_X),
													m_Y(pAD->m_Y),
													m_Z(pAD->m_Z),
													m_SliceTotal(pAD->m_SliceTotal),
													m_RowTotal(pAD->m_RowTotal),
													m_CellTotal(pAD->m_CellTotal),
												    m_Solvent(pAD->m_Solvent),
													m_pTSD(0),
													m_LipidType(0),
													m_LipidHeadType(0),
													m_LipidTailType(0),
													m_SolventType(0),
													m_SolventHeadType(0),
													m_LipidBeadTypes(0),
													m_LipidBondTypes(0),
													m_LipidBondPairTypes(0),
													m_pLipidSP(0),
													m_pLipidHeadDensity(0),
													m_pLipidTailDensity(0),
													m_pSolventDensity(0),
													m_LipidTotal(0),
													m_InnerTotal(0),
													m_OuterTotal(0),
													m_MissingTotal(0),
													m_InnerHeight1(0.0),
													m_InnerHeight2(0.0),
													m_OuterHeight1(0.0),
													m_OuterHeight2(0.0),
													m_BilayerHeight1(0.0),
													m_BilayerHeight2(0.0),
													m_InnerWidth(0.0),
													m_OuterWidth(0.0),
													m_BilayerWidth(0.0),
													m_InnerArea(0.0),
													m_OuterArea(0.0),
													m_InnerAN(0.0),
													m_OuterAN(0.0),
													m_ProjArea(0.0),
													m_InnerAreaRatio(0.0),
													m_OuterAreaRatio(0.0),
													m_VolumePerLipid(0.0),
													m_SliceWidth(0.0),
													m_RowWidth(0.0),
													m_RowWidthSq(0.0),
													m_CellWidth(0.0),
													m_CellWidthSq(0.0),
													m_RowOverCell(0.0),
													m_RowOverCellSq(0.0),
													m_RowCellTotal(static_cast<double>(m_RowTotal*m_CellTotal)),
													m_bPoreOpen(false),
													m_PoreSize(0),
													m_1dProfileCounter(0),
													m_StressGridTotal(0),
													m_SurfaceTension(0.0),
													m_SurfaceTensionTrap(0.0),
													m_FirstMomentOfStress(0.0),
													m_SecondMomentOfStress(0.0),
													m_pStress1d(0),
													m_pTotalStress(0),
													m_pWWStress(0),
													m_pLipidHeadWStress(0),
													m_pLipidTailWStress(0),
													m_pLipidHeadHeadStress(0),
													m_pLipidHeadTailStress(0),
													m_pLipidTailTailStress(0),
													m_pSxx(0),
													m_pSyy(0),
													m_pSzz(0)
{
	m_lLipids.clear();
	m_lInnerLipids.clear();
	m_lOuterLipids.clear();

	m_vBondStress.clear();			// No bond stress profiles yet
	m_vBondPairStress.clear();		// No bondpair stress profiles yet

	m_vBeadTypeIndex.clear();		// All bead types in a lipid polymer
	m_vBondTypeIndex.clear();		// All bond types in a lipid polymer
	m_vBondPairTypeIndex.clear();	// All bondpair types in a lipid polymer

	m_vBeadStressIndex.clear();		// Bead-bead index identifying stress contributions,
	m_vBondStressIndex.clear();		// and the bond index for bond stress terms
	m_vBondPairStressIndex.clear();	// and the bondpair index for bond stress terms

	for(short int i=0; i<3; i++)
	{
		m_rcmHead[i] = 0.0;
		m_rcmTail[i] = 0.0;
	}

	m_mPolymerFromHeadId.clear();
	m_mPolymerFromTailId.clear();

	m_vBilayerProfiles.clear();

#ifndef BILAYER_NO_USE_VALARRAY
	// Set the size of the valarrays used to calculate bilayer properties
	// using the grid as they don't need to be altered.

	m_aInnerHeadNo2d.resize(m_CellTotal*m_RowTotal, 0.0); 
	m_aOuterHeadNo2d.resize(m_CellTotal*m_RowTotal, 0.0);
	m_aInnerHead2d.resize(m_CellTotal*m_RowTotal, 0.0);
	m_aOuterHead2d.resize(m_CellTotal*m_RowTotal, 0.0);

	m_aInnerTailNo2d.resize(m_CellTotal*m_RowTotal, 0.0);
	m_aOuterTailNo2d.resize(m_CellTotal*m_RowTotal, 0.0);
	m_aInnerTail2d.resize(m_CellTotal*m_RowTotal, 0.0);
	m_aOuterTail2d.resize(m_CellTotal*m_RowTotal, 0.0);

	m_aInnerAN2d.resize(m_CellTotal*m_RowTotal);
	m_aOuterAN2d.resize(m_CellTotal*m_RowTotal);

	// Set the components of a gslice used to and and extract 1d height data
	// from the 2d n(x,y), h(x,y) fields stored in valarrays.

	m_aHeightFieldDim.resize(2);
	m_aHeightFieldInt.resize(2);

	m_aHeightFieldDim[0] = m_CellTotal;
	m_aHeightFieldDim[1] = 1;
	m_aHeightFieldInt[0] = 1;
	m_aHeightFieldInt[1] = m_CellTotal;

#endif

	// Set the size of the area per lipid arrays here as they don't need to be
	// altered.

	m_aInnerAN2d.resize(m_CellTotal*m_RowTotal);
	m_aOuterAN2d.resize(m_CellTotal*m_RowTotal);

}

CBilayer::~CBilayer()
{
	// Delete the profile objects if they exist

	if(m_pLipidSP)
	{
		delete m_pLipidSP;
		m_pLipidSP = 0;
	}

	if(m_pLipidHeadDensity)
	{
		delete m_pLipidHeadDensity;
		m_pLipidHeadDensity = 0;
	}

	if(m_pLipidTailDensity)
	{
		delete m_pLipidTailDensity;
		m_pLipidTailDensity = 0;
	}

	if(m_pSolventDensity)
	{
		delete m_pSolventDensity;
		m_pSolventDensity = 0;
	}

	if(m_pStress1d)
	{
		delete m_pStress1d;
		m_pStress1d = 0;
	}

	if(m_pTotalStress)
	{
		delete m_pTotalStress;
		m_pTotalStress = 0;
	}

	if(m_pWWStress)
	{
		delete m_pWWStress;
		m_pWWStress = 0;
	}

	if(m_pLipidHeadWStress)
	{
		delete m_pLipidHeadWStress;
		m_pLipidHeadWStress = 0;
	}

	if(m_pLipidTailWStress)
	{
		delete m_pLipidTailWStress;
		m_pLipidTailWStress = 0;
	}

	if(m_pLipidHeadHeadStress)
	{
		delete m_pLipidHeadHeadStress;
		m_pLipidHeadHeadStress = 0;
	}

	if(m_pLipidHeadTailStress)
	{
		delete m_pLipidHeadTailStress;
		m_pLipidHeadTailStress = 0;
	}

	if(m_pLipidTailTailStress)
	{
		delete m_pLipidTailTailStress;
		m_pLipidTailTailStress = 0;
	}

	if(m_pSxx)
	{
		delete m_pSxx;
		m_pSxx = 0;
	}

	if(m_pSyy)
	{
		delete m_pSyy;
		m_pSyy = 0;
	}

	if(m_pSzz)
	{
		delete m_pSzz;
		m_pSzz = 0;
	}

	// Delete the bond stress profiles if any exist

	if(!m_vBondStress.empty())
	{
		for(ScalarProfileIterator iterSP=m_vBondStress.begin(); iterSP!=m_vBondStress.end(); iterSP++)
		{
			delete *iterSP;
		}
		m_vBondStress.clear();
	}

	// and delete the bondpair stress profiles if any exist

	if(!m_vBondPairStress.empty())
	{
		for(ScalarProfileIterator iterSP=m_vBondPairStress.begin(); iterSP!=m_vBondPairStress.end(); iterSP++)
		{
			delete *iterSP;
		}
		m_vBondPairStress.clear();
	}

	if(!m_vBilayerProfiles.empty())
	{
		for(CellProfileSetVectorIterator iterCPS=m_vBilayerProfiles.begin(); iterCPS!=m_vBilayerProfiles.end(); iterCPS++)
		{
			delete *iterCPS;
		}
		m_vBilayerProfiles.clear();
	}
}

// **********************************************************************
// Function to analyse the properties of a bilayer made up of a single 
// major component, called "lipids" for convenience. In the future, minor 
// components may be added to the bilayer but there must be a single 
// defining component that forms the bulk of the bilayer. We collect all 
// polymers that are of the user-specified type and store them locally 
// for rapid analysis.
//
// **********************************************************************

void CBilayer::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Counters and iterator used many times below. I have to declare them because
	// different machines may or may not allow them to be redefined in different loops
	// in the same scope.

	short int i;
	cPolymerListIterator iterLipid;

	// **********************************************************************
	// On the first call, initialise the slice profile and create the observables 
	// needed to output the data on the bilayer

	if(!m_pLipidSP)
	{
		// The number of slices is now set by the user in the control data file.
		// Their width can be the same as, less than, or greater than the CNT
		// cell size in the appropriate normal direction.
		//
		// We also set the width of the rows and cells using the number of 
		// CNT cells in the appropriate dimensions and their widths obtained
		// from the ISimBox interface.

		if(m_Z == 1)
		{
			m_StressGridTotal = rSimState.GetGridZCellNo();
			m_ProjArea	 = pISimBox->GetSimBoxXLength()*pISimBox->GetSimBoxYLength();
			m_SliceWidth = pISimBox->GetCNTZCellWidth()*static_cast<double>(pISimBox->GetCNTZCellNo())/
				           static_cast<double>(m_SliceTotal);
			m_RowWidth   = pISimBox->GetCNTYCellWidth()*static_cast<double>(pISimBox->GetCNTYCellNo())/
				           static_cast<double>(m_RowTotal);
			m_CellWidth  = pISimBox->GetCNTXCellWidth()*static_cast<double>(pISimBox->GetCNTXCellNo())/
				           static_cast<double>(m_CellTotal);
		}
		else if(m_Y == 1)
		{
			m_StressGridTotal = rSimState.GetGridYCellNo();
			m_ProjArea	 = pISimBox->GetSimBoxXLength()*pISimBox->GetSimBoxZLength();
			m_SliceWidth = pISimBox->GetCNTYCellWidth()*static_cast<double>(pISimBox->GetCNTYCellNo())/
				           static_cast<double>(m_SliceTotal);
			m_RowWidth   = pISimBox->GetCNTXCellWidth()*static_cast<double>(pISimBox->GetCNTXCellNo())/
				           static_cast<double>(m_RowTotal);
			m_CellWidth  = pISimBox->GetCNTZCellWidth()*static_cast<double>(pISimBox->GetCNTZCellNo())/
				           static_cast<double>(m_CellTotal);
		}
		else if(m_X == 1)
		{
			m_StressGridTotal = rSimState.GetGridXCellNo();
			m_ProjArea	 = pISimBox->GetSimBoxYLength()*pISimBox->GetSimBoxZLength();
			m_SliceWidth = pISimBox->GetCNTXCellWidth()*static_cast<double>(pISimBox->GetCNTXCellNo())/
				           static_cast<double>(m_SliceTotal);
			m_RowWidth   = pISimBox->GetCNTZCellWidth()*static_cast<double>(pISimBox->GetCNTZCellNo())/
				           static_cast<double>(m_RowTotal);
			m_CellWidth  = pISimBox->GetCNTYCellWidth()*static_cast<double>(pISimBox->GetCNTYCellNo())/
				           static_cast<double>(m_CellTotal);
		}

		// Define the ratio of the row to cell widths that is used in calculating the
		// area per lipid using the cell breakdown of the SimBox. There can never be
		// no cells so we don't check for divide by zero.

		m_RowOverCell   = m_RowWidth/m_CellWidth;

		// Square of the above quantities to save time in the loops

		m_RowWidthSq	= m_RowWidth*m_RowWidth;
		m_CellWidthSq	= m_CellWidth*m_CellWidth;
		m_RowOverCellSq = m_RowOverCell*m_RowOverCell;

		// Create the slice profile to identify which slices contain the polymers
		// in the bilayer

		m_pLipidSP	= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);

		// Create the profile objects to hold the scalar, vector or tensor data obtained 
		// from analysing the slices, rows or cells. Mare sure their contents are
		// destroyed before the container is used again.
		// Note that the solvent density is calculated from the number of solvent
		// polymer head beads in a given slice.

		m_pLipidHeadDensity		= new aaScalarProfile(m_SliceTotal);
		m_pLipidTailDensity		= new aaScalarProfile(m_SliceTotal);
		m_pSolventDensity		= new aaScalarProfile(m_SliceTotal);

		// The size of the stress tensor profiles is set from the size of
		// analysis grid in the appropriate direction. Note that the bond
		// profiles are created after the lipid polymers have been collected
		// into the m_lLipids container.

		m_pTotalStress			= new aaScalarProfile(m_StressGridTotal);
		m_pWWStress				= new aaScalarProfile(m_StressGridTotal);
		m_pLipidHeadWStress		= new aaScalarProfile(m_StressGridTotal);
		m_pLipidTailWStress		= new aaScalarProfile(m_StressGridTotal);
		m_pLipidHeadHeadStress	= new aaScalarProfile(m_StressGridTotal);
		m_pLipidHeadTailStress	= new aaScalarProfile(m_StressGridTotal);
		m_pLipidTailTailStress	= new aaScalarProfile(m_StressGridTotal);
		m_pSxx					= new aaScalarProfile(m_StressGridTotal);
		m_pSyy					= new aaScalarProfile(m_StressGridTotal);
		m_pSzz					= new aaScalarProfile(m_StressGridTotal);

		// Store the bead indexes needed to access the stress tensor components 
		// created for each bead-bead interaction. These are stored in the order
		// in which the contributions are calculated:
		//
		//	Solvent-Solvent, Head-Solvent, Tail-Solvent, Head-Head, Head-Tail, Tail-Tail
		//
		// But the number of bead types, and the types of each can be in an
		// arbitrary order that depends on how many bead types have been created.
		// Hence the actual indexes stored may not be monotonically ordered.
		// If further contributions are added the appropriate indxes must be added
		// here. We order them by choosing the larger of the two indices as the first
		// when accessing the off-diagonal elements. This corresponds to the order in 
		// which they are stored in CMonitor::AddBeadStress().

		m_vBeadStressIndex.resize(rSimState.GetBeadTypeTotal()*rSimState.GetBeadTypeTotal());

		m_vBeadStressIndex.at(0) = m_SolventHeadType*rSimState.GetBeadTypeTotal() + m_SolventHeadType;
		m_vBeadStressIndex.at(1) =   m_LipidHeadType*rSimState.GetBeadTypeTotal() + m_SolventHeadType;
		m_vBeadStressIndex.at(2) =   m_LipidTailType*rSimState.GetBeadTypeTotal() + m_SolventHeadType;
		m_vBeadStressIndex.at(3) =   m_LipidHeadType*rSimState.GetBeadTypeTotal() + m_LipidHeadType;
		m_vBeadStressIndex.at(4) =   m_LipidHeadType*rSimState.GetBeadTypeTotal() + m_LipidTailType;
		m_vBeadStressIndex.at(5) =   m_LipidTailType*rSimState.GetBeadTypeTotal() + m_LipidTailType;


		// Find out how many lipids there are in the system so that we can allocate
		// memory to the containers and store the lipid polymers locally for use in
		// analysis. We also create maps allowing polymers to be accessed given the
		// ids of their head or tail beads as keys.

		m_LipidTotal = count_if(rSimState.GetPolymers().begin(), rSimState.GetPolymers().end(), aaPolymerType(m_LipidType));

		// Create maps allowing polymer pointers to be obtained given head and tail bead
		// ids as keys. These are used in the bilayer height calculataion.

		for(cPolymerVectorIterator iterPoly=rSimState.GetPolymers().begin(); iterPoly!=rSimState.GetPolymers().end(); iterPoly++)
		{
			if((*iterPoly)->GetType() == m_LipidType)
			{
				m_lLipids.push_back(*iterPoly);

				m_mPolymerFromHeadId.insert(zPairLongPolymer((*iterPoly)->GetHead()->GetId(), *iterPoly));
				m_mPolymerFromTailId.insert(zPairLongPolymer((*iterPoly)->GetTail()->GetId(), *iterPoly));
			}
		}

		// Create the bond stress profiles for all bond types that occur in the
		// lipid polymers. We use the first lipid polymer in the m_lLipids container
		// created above to obtain the vector of bonds in the lipids. Then we
		// search for all  distinct bond types in the bond vector.
		// Note that this assumes there is only one type of polymer stored
		// in the container. This might not be true if the CBilayer aggregate is
		// later changed to allow multiple polymer types to define the bilayer.
		// The index used to obtain the bond profile. 

		CPolymer* const pLipid		= m_lLipids.front();
		BeadVector& rBeads			= pLipid->GetBeads();
		BondVector& rBonds			= pLipid->GetBonds();
		BondPairVector& rBondPairs	= pLipid->GetBondPairs();

		// We also store the set of bead types that occur in the lipid polymers
		// for future use.

		for(cBeadVectorIterator iterBead=rBeads.begin(); iterBead!=rBeads.end(); iterBead++)
		{
			const long beadType = (*iterBead)->GetType();

			czLongVectorIterator iterBeadType = find(m_vBeadTypeIndex.begin(), m_vBeadTypeIndex.end(), beadType);

			if(iterBeadType == m_vBeadTypeIndex.end())
			{
				m_vBeadTypeIndex.push_back(beadType);
			}
		}

		m_LipidBeadTypes = m_vBeadTypeIndex.size();

		// Now the bonds. Note that the stress index for bonds is just the bond type,
		// unlike for the beads where it is made up of the indices of each bead pair.

		for(cBondVectorIterator iterBond=rBonds.begin(); iterBond!=rBonds.end(); iterBond++)
		{
			const long bondType = (*iterBond)->GetType();
			const long bondStressIndex = bondType;

			czLongVectorIterator iterBondType = find(m_vBondStressIndex.begin(), m_vBondStressIndex.end(), bondStressIndex);

			if(iterBondType == m_vBondStressIndex.end())
			{
				m_vBondTypeIndex.push_back(bondType);
				m_vBondStressIndex.push_back(bondStressIndex);

				aaScalarProfile* pSP = new aaScalarProfile(m_StressGridTotal);
				m_vBondStress.push_back(pSP);
			}
		}

		m_LipidBondTypes = m_vBondTypeIndex.size();

		// Now the bondpairs. Note that the stress index for bondpairs is 
		// just the bondpair type

		for(cBondPairVectorIterator iterBP=rBondPairs.begin(); iterBP!=rBondPairs.end(); iterBP++)
		{
			const long bondpairType = (*iterBP)->GetType();
			const long bondpairStressIndex = bondpairType;

			czLongVectorIterator iterBPType = find(m_vBondPairStressIndex.begin(), m_vBondPairStressIndex.end(), bondpairStressIndex);

			if(iterBPType == m_vBondPairStressIndex.end())
			{
				m_vBondPairTypeIndex.push_back(bondpairType);
				m_vBondPairStressIndex.push_back(bondpairStressIndex);

				aaScalarProfile* pSP = new aaScalarProfile(m_StressGridTotal);
				m_vBondPairStress.push_back(pSP);
			}
		}

		m_LipidBondPairTypes = m_vBondPairTypeIndex.size();

		// Create the observables needed to hold the bilayer data.

		// Id	Name
		// *********
		//
		//  First define observables derived from the lipid bead lists
		//
		//	0	Inner monolayer lipid total
		//	1	Outer    "        "    "
		//  2	Inner monolayer height from head positions
		//  3	Inner monolayer height from tail positions
		//	4	Outer    "        "
		//	5	Outer    "        "
		//  6   Bilayer Height from head positions
		//  7   Bilayer Height from tail positions
		//  8	Inner monolayer width
		//	9	Outer    "        "
		//	10	Inner monolayer CM coordinates
		//	11	Outer    "             "
		//
		// Now define those derived from a geometric analysis of the SimBox
		//
		//  12  Number of lipids in inner monolayer
		//  13  Number of lipids in outer monolayer
		//  14  Missing lipids (not in bilayer)
		//  15  Inner monolayer area
		//  16  Outer monolayer area
		//  17  Inner monolayer area/projected area
		//  18  Outer monolayer area/projected area
		//  19  Inner monolayer thickness
		//  20  Outer    "         "
		//  21  Bilayer Thickness
		//  22  Bilayer volume per lipid
		//	23	Inner monolayer area per lipid
		//	24	Outer monolayer area per lipid
		//  25  Lipid head density profile
		//	26	Lipid tail density profile
		//	27  Solvent density profile through the bilayer
		//
		// Now the stress tensor observables. Note that these have a size set by
		// the number of slices in the whole SimBox not just the bilayer.
		//
		//  28  Surface tension of bilayer calculated using Trapezoidal rule
		//  29  Surface tension of bilayer calculated original way
		//  30  First moment of stress tensor across bilayer
		//  31  Second moment of stress tensor across bilayer
		//	32	Total stress profile through bilayer
		//	33	Water-Water stress profile through bilayer
		//	34	Head-Water  stress profile through bilayer
		//	35	Tail-Water  stress profile through bilayer
		//	36	Head-Head   stress profile through bilayer
		//	37	Head-Tail   stress profile through bilayer
		//	38	Tail-Tail   stress profile through bilayer
		//
		// Now the bond stress tensor observables. We don't know a priori how
		// many bond types there are in a simulation so the number of observables
		// will vary from run to run.
		//
		//  39 to 39 + No of bond types in a lipid polymer
		//			Bond stress profiles through bilayer
		
		// Now the bondpair stress tensor observables. We don't know a priori how
		// many bondpair types there are in a simulation so the number of observables
		// will vary from run to run.
		//
		//  39 + No of bond types in a lipid polymer
		//     to 39 + No of bond types in a lipid polymer + No of bondpair types
		//			Bond stress profiles through bilayer
		
		CScalarObservable* pInnerTotal	= new CScalarObservable(m_Polymer + " inner monolayer total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pOuterTotal	= new CScalarObservable(m_Polymer + " outer monolayer total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pInnerHeight1  = new CScalarObservable(m_Polymer + " inner monolayer height (head)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pInnerHeight2  = new CScalarObservable(m_Polymer + " inner monolayer height (tail)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pOuterHeight1  = new CScalarObservable(m_Polymer + " outer monolayer height (head)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pOuterHeight2  = new CScalarObservable(m_Polymer + " outer monolayer height (tail)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pHeight1       = new CScalarObservable(m_Polymer + " bilayer height (head)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pHeight2       = new CScalarObservable(m_Polymer + " bilayer height (tail)",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pInnerWidth  = new CScalarObservable(m_Polymer + " inner monolayer width",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pOuterWidth  = new CScalarObservable(m_Polymer + " outer monolayer width",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		CVectorObservable* pHeadCM		= new CVectorObservable(m_Polymer + " head CM coords",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pTailCM		= new CVectorObservable(m_Polymer + " tail CM coords",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		CScalarObservable* pgInnerTotal		= new CScalarObservable(m_Polymer + " inner monolayer total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgOuterTotal		= new CScalarObservable(m_Polymer + " outer monolayer total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgMissingTotal	= new CScalarObservable(m_Polymer + " missing total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgInnerArea		= new CScalarObservable(m_Polymer + " inner monolayer area",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgOuterArea		= new CScalarObservable(m_Polymer + " outer monolayer area",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgInnerAreaRatio	= new CScalarObservable(m_Polymer + " inner monolayer area ratio",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgOuterAreaRatio	= new CScalarObservable(m_Polymer + " outer monolayer area ratio",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgInnerWidth		= new CScalarObservable(m_Polymer + " inner monolayer width",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgOuterWidth		= new CScalarObservable(m_Polymer + " outer monolayer width",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgBilayerWidth  = new CScalarObservable(m_Polymer + " Bilayer width",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		CScalarObservable* pgBilayerVN		= new CScalarObservable(m_Polymer + " Bilayer V/N",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		CScalarObservable* pgInnerAN		= new CScalarObservable(m_Polymer + " inner monolayer A/N",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pgOuterAN		= new CScalarObservable(m_Polymer + " outer monolayer A/N",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

	    CScalarProfileObservable* pHeadDensPro = new CScalarProfileObservable(m_Polymer + " Head Density Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);

	    CScalarProfileObservable* pTailDensPro = new CScalarProfileObservable(m_Polymer + " Tail Density Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);

	    CScalarProfileObservable* pSolventDensPro = new CScalarProfileObservable(m_Solvent + " Density Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);

		// Stress-related observables

		CScalarObservable* pSurfaceTensionTrap	= new CScalarObservable("Bilayer Trapezoidal Surface Tension",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		
		CScalarObservable* pSurfaceTension	= new CScalarObservable("Bilayer Surface Tension",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		
		CScalarObservable* pFirstMomentStress	= new CScalarObservable("First Moment of Stress",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		
		CScalarObservable* pSecondMomentStress	= new CScalarObservable("Second Moment of Stress",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		
	    CScalarProfileObservable* pTotalStressPro = new CScalarProfileObservable("Total Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pWWStressPro = new CScalarProfileObservable(m_Solvent + "-" + m_Solvent + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pHWStressPro = new CScalarProfileObservable(m_Polymer + " Head-" + m_Solvent + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pTWStressPro = new CScalarProfileObservable(m_Polymer + " Tail-" + m_Solvent + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pHHStressPro = new CScalarProfileObservable(m_Polymer + " Head-Head" + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pHTStressPro = new CScalarProfileObservable(m_Polymer + " Head-Tail" + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pTTStressPro = new CScalarProfileObservable(m_Polymer + " Tail-Tail" + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);
	
		m_vObservables.push_back(pInnerTotal);
		m_vObservables.push_back(pOuterTotal);
		m_vObservables.push_back(pInnerHeight1);
		m_vObservables.push_back(pInnerHeight2);
		m_vObservables.push_back(pOuterHeight1);
		m_vObservables.push_back(pOuterHeight2);
		m_vObservables.push_back(pHeight1);
		m_vObservables.push_back(pHeight2);
		m_vObservables.push_back(pInnerWidth);
		m_vObservables.push_back(pOuterWidth);
		m_vObservables.push_back(pHeadCM);
		m_vObservables.push_back(pTailCM);
		m_vObservables.push_back(pgInnerTotal);
		m_vObservables.push_back(pgOuterTotal);
		m_vObservables.push_back(pgMissingTotal);
		m_vObservables.push_back(pgInnerArea);
		m_vObservables.push_back(pgOuterArea);
		m_vObservables.push_back(pgInnerAreaRatio);
		m_vObservables.push_back(pgOuterAreaRatio);
		m_vObservables.push_back(pgInnerWidth);
		m_vObservables.push_back(pgOuterWidth);
		m_vObservables.push_back(pgBilayerWidth);
		m_vObservables.push_back(pgBilayerVN);
		m_vObservables.push_back(pgInnerAN);
		m_vObservables.push_back(pgOuterAN);
		m_vObservables.push_back(pHeadDensPro);
		m_vObservables.push_back(pTailDensPro);
		m_vObservables.push_back(pSolventDensPro);
		m_vObservables.push_back(pSurfaceTensionTrap);
		m_vObservables.push_back(pSurfaceTension);
		m_vObservables.push_back(pFirstMomentStress);
		m_vObservables.push_back(pSecondMomentStress);
		m_vObservables.push_back(pTotalStressPro);
		m_vObservables.push_back(pWWStressPro);
		m_vObservables.push_back(pHWStressPro);
		m_vObservables.push_back(pTWStressPro);
		m_vObservables.push_back(pHHStressPro);
		m_vObservables.push_back(pHTStressPro);
		m_vObservables.push_back(pTTStressPro);

		// Create the profiles needed for lipid polymer bonds

		for(long bondNo=0; bondNo<m_LipidBondTypes; bondNo++)
		{
			long bondType = m_vBondTypeIndex.at(bondNo);

			CScalarProfileObservable* pBondStressPro = new CScalarProfileObservable(m_Polymer + " Bond Type " + ToString(bondType) + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);
	
			m_vObservables.push_back(pBondStressPro);
		}

		// and the profiles needed for lipid polymer bondpairs

		for(long bondpairNo=0; bondpairNo<m_LipidBondPairTypes; bondpairNo++)
		{
			long bondpairType = m_vBondPairTypeIndex.at(bondpairNo);

			CScalarProfileObservable* pBondPairStressPro = new CScalarProfileObservable(m_Polymer + " BondPair Type " + ToString(bondpairType) + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);
	
			m_vObservables.push_back(pBondPairStressPro);
		}

		// Now the diagonal elements of the stress tensor

	    CScalarProfileObservable* pxxStressPro = new CScalarProfileObservable(" Sxx Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pyyStressPro = new CScalarProfileObservable(" Syy Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pzzStressPro = new CScalarProfileObservable(" Szz Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

		m_vObservables.push_back(pxxStressPro);
		m_vObservables.push_back(pyyStressPro);
		m_vObservables.push_back(pzzStressPro);

		// CAnalysisTool-derived classes
		// *****************************
		//

	}
	else	// If not the first time delete the old slice profile before creating a new one
	{
		delete m_pLipidSP;
		m_pLipidSP	= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);

	}

	// **********************************************************************
	// Analyse the bilayer using the set of polymers that have been specified 
	// by the user to compose the bilayer. Only one polymer type is allowed.
	// This means that analysis of bilayers that have two or more polymer
	// types in them may be inaccurate. For these bilayers the grid analysis
	// is more accurate.
	// We refer to these polymers as lipids for simplicity even if they don't 
	// have an architecture corresponding to real lipids.
	// Because lipids may enter and leave each monolayer we do this every sample.
	// However, the total number of lipids in the simulation is unchanging.

	// Divide the lipids into the two monolayers using the directions of
	// their Head-Tail vectors along the bilayer normal, and store the 
	// average monolayer thicknesses. We empty the lists holding the lipids
	// in the inner and outer monolayers prior to couting them using push_back().
	// Then find the height of the inner and outer monolayers by summing the positions
	// of all the head beads in each monolayer. We don't use the grid for this
	// because membranes containing multiple polymers can have patches where there
	// are no lipids of the specified type for the bilayer analysis. This causes the
	// height average to be wrongly normalized. Finally, find the CM of the bilayer 
	// lipids from the coordinates of their head and tail beads separately.

	m_lInnerLipids.clear();
	m_lOuterLipids.clear();

	m_InnerTotal	= 0;
	m_OuterTotal	= 0;
	m_InnerHeight1	= 0.0;
	m_InnerHeight2	= 0.0;
	m_OuterHeight1	= 0.0;
	m_OuterHeight2	= 0.0;
	m_InnerWidth	= 0.0;
	m_OuterWidth	= 0.0;

	for(i=0; i<3; i++)
	{
		m_rcmHead[i] = 0.0;
		m_rcmTail[i] = 0.0;
	}

	if(m_X == 1)
	{
		for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
		{
			double hx = (*iterLipid)->GetHead()->GetXPos();
			double hy = (*iterLipid)->GetHead()->GetYPos();
			double hz = (*iterLipid)->GetHead()->GetZPos();
			double tx = (*iterLipid)->GetTail()->GetXPos();
			double ty = (*iterLipid)->GetTail()->GetYPos();
			double tz = (*iterLipid)->GetTail()->GetZPos();

			if(hx < tx)
			{
				m_InnerTotal++;
				m_lInnerLipids.push_back(*iterLipid);
				m_InnerHeight1 += hx;
				m_InnerHeight2 += tx;
				m_InnerWidth   += (tx-hx);
			}
			else
			{
				m_OuterTotal++;
				m_lOuterLipids.push_back(*iterLipid);
				m_OuterHeight1 += hx;
				m_OuterHeight2 += tx;
				m_OuterWidth   += (hx-tx);
			}

			m_rcmHead[0] += hx;
			m_rcmHead[1] += hy;
			m_rcmHead[2] += hz;
			m_rcmTail[0] += tx;
			m_rcmTail[1] += ty;
			m_rcmTail[2] += tz;
		}
	}
	else if(m_Y == 1)
	{
		for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
		{
			double hx = (*iterLipid)->GetHead()->GetXPos();
			double hy = (*iterLipid)->GetHead()->GetYPos();
			double hz = (*iterLipid)->GetHead()->GetZPos();
			double tx = (*iterLipid)->GetTail()->GetXPos();
			double ty = (*iterLipid)->GetTail()->GetYPos();
			double tz = (*iterLipid)->GetTail()->GetZPos();

			if(hy < ty)
			{
				m_InnerTotal++;
				m_lInnerLipids.push_back(*iterLipid);
				m_InnerHeight1 += hy;
				m_InnerHeight2 += ty;
				m_InnerWidth   += (ty-hy);
			}
			else
			{
				m_OuterTotal++;
				m_lOuterLipids.push_back(*iterLipid);
				m_OuterHeight1 += hy;
				m_OuterHeight2 += ty;
				m_OuterWidth   += (hy-ty);
			}

			m_rcmHead[0] += hx;
			m_rcmHead[1] += hy;
			m_rcmHead[2] += hz;
			m_rcmTail[0] += tx;
			m_rcmTail[1] += ty;
			m_rcmTail[2] += tz;
		}
	}
	else if(m_Z == 1)
	{
		for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
		{
			double hx = (*iterLipid)->GetHead()->GetXPos();
			double hy = (*iterLipid)->GetHead()->GetYPos();
			double hz = (*iterLipid)->GetHead()->GetZPos();
			double tx = (*iterLipid)->GetTail()->GetXPos();
			double ty = (*iterLipid)->GetTail()->GetYPos();
			double tz = (*iterLipid)->GetTail()->GetZPos();

			if(hz < tz)
			{
				m_InnerTotal++;
				m_lInnerLipids.push_back(*iterLipid);
				m_InnerHeight1 += hz;
				m_InnerHeight2 += tz;
				m_InnerWidth   += (tz-hz);
			}
			else
			{
				m_OuterTotal++;
				m_lOuterLipids.push_back(*iterLipid);
				m_OuterHeight1 += hz;
				m_OuterHeight2 += tz;
				m_OuterWidth   += (hz-tz);
			}

			m_rcmHead[0] += hx;
			m_rcmHead[1] += hy;
			m_rcmHead[2] += hz;
			m_rcmTail[0] += tx;
			m_rcmTail[1] += ty;
			m_rcmTail[2] += tz;
		}
	}

	m_InnerHeight1 /= static_cast<double>(m_InnerTotal);
	m_InnerHeight2 /= static_cast<double>(m_InnerTotal);
	m_OuterHeight1 /= static_cast<double>(m_OuterTotal);
	m_OuterHeight2 /= static_cast<double>(m_OuterTotal);
	m_BilayerHeight1 = 0.5*(m_InnerHeight1 + m_OuterHeight1);
	m_BilayerHeight2 = 0.5*(m_InnerHeight2 + m_OuterHeight2);
	m_InnerWidth   /= static_cast<double>(m_InnerTotal);
	m_OuterWidth   /= static_cast<double>(m_OuterTotal);

	for(i=0; i<3; i++)
	{
		m_rcmHead[i] /= static_cast<double>(m_LipidTotal);
		m_rcmTail[i] /= static_cast<double>(m_LipidTotal);
	}

	// Copy the data to the bilayer's observables using the encapsulated data types
	// aaScalarSingle, aaVectorSingle, etc.

	aaScalarSingle innerTotal(m_InnerTotal);
	aaScalarSingle outerTotal(m_OuterTotal);
	aaScalarSingle innerHeight1(m_InnerHeight1);
	aaScalarSingle innerHeight2(m_InnerHeight2);
	aaScalarSingle outerHeight1(m_OuterHeight1);
	aaScalarSingle outerHeight2(m_OuterHeight2);
	aaScalarSingle bilayerHeight1(m_BilayerHeight1);
	aaScalarSingle bilayerHeight2(m_BilayerHeight2);
	aaScalarSingle innerWidth(m_InnerWidth);
	aaScalarSingle outerWidth(m_OuterWidth);
	aaVectorSingle headCM(m_rcmHead[0],m_rcmHead[1],m_rcmHead[2]);
	aaVectorSingle tailCM(m_rcmTail[0],m_rcmTail[1],m_rcmTail[2]);

	// Call the AddData() function on each data type so that it can pass itself
	// to the appropriate CObservable-derived object

	innerTotal.AddData(m_vObservables.at(0));
	outerTotal.AddData(m_vObservables.at(1));
	innerHeight1.AddData(m_vObservables.at(2));
	innerHeight2.AddData(m_vObservables.at(3));
	outerHeight1.AddData(m_vObservables.at(4));
	outerHeight2.AddData(m_vObservables.at(5));
	bilayerHeight1.AddData(m_vObservables.at(6));
	bilayerHeight2.AddData(m_vObservables.at(7));
	innerWidth.AddData(m_vObservables.at(8));
	outerWidth.AddData(m_vObservables.at(9));
	headCM.AddData(m_vObservables.at(10));
	tailCM.AddData(m_vObservables.at(11));

	// **********************************************************************
	// Now do the analysis using a grid covering the SimBox and sorting the
	// polymers into slices, rows and cells. This allows a systematic
	// analysis of the bilayer height, width, etc using grids of arbitrary
	// size.
	//
	// Start by removing slices that do not contain any lipid polymers (that is,
	// any lipid head or tail beads). These correspond to the solvent volume 
	// around the bilayer. In fact, we copy the slices containing lipid polymers 
	// from the original slice profile to a local copy. This avoids having to 
	// delete any elements in the original CProfile and invalidating the iterators. 
	// We have to use the CSlice copy constructor because all CProfile objects 
	// will delete their CRegion contents when they are destroyed. Because the 
	// CRegions just contain bead lists we don't have to delete the objects 
	// pointed to, in fact, we must not because they are the bead lists created 
	// at the start of the program.

	CSliceProfile bilayerSP;

	for(cSliceVectorIterator iterSlice=m_pLipidSP->Begin(); iterSlice!=m_pLipidSP->End(); iterSlice++)
	{
		BeadList lBeads = (*iterSlice)->GetBeads();

		cBeadListIterator iterBead = find_if(lBeads.begin(), lBeads.end(), aaBeadType2(m_LipidHeadType, m_LipidTailType));

		if(iterBead!=lBeads.end())	// Slice contains lipid head/tail beads
		{
			CSlice* pSlice = new CSlice(**iterSlice);
			bilayerSP.AddRegion(pSlice);
		}
	}

	// Check the density profile of lipid head and tail beads in the whole SimBox

#ifndef BILAYER_NO_USE_VALARRAY
	transform(m_pLipidSP->Begin(), m_pLipidSP->End(), m_pLipidHeadDensity->Begin(), aaDensity(m_LipidHeadType));
	transform(m_pLipidSP->Begin(), m_pLipidSP->End(), m_pLipidTailDensity->Begin(), aaDensity(m_LipidTailType));
	transform(m_pLipidSP->Begin(), m_pLipidSP->End(), m_pSolventDensity->Begin(),   aaDensity(m_SolventHeadType));
#endif

/*	for(long iSlice = 0; iSlice<m_SliceTotal; iSlice++)
	{
		double headDens		= m_pLipidHeadDensity->GetData(iSlice);
		double tailDens		= m_pLipidTailDensity->GetData(iSlice);
		double solventDens	= m_pSolventDensity->GetData(iSlice);
	}
*/
	// Construct a row profile set from the slice profile. This is a set of row
	// profiles each of which divides up one slice. The number of such rows is 
	// set by the user in the bilayer analysis object data as m_RowTotal.
	//
	// The rows are used to construct a new profile orthogonal to the old one. 
	// A cell profile set is then constructed from each orthogonal row profile.
	// There is one cell profile for each row in the orthogonal row profile.
	// The number of cells in each cell profile is also set by the user as m_CellTotal.
	// The cell profiles are then used to construct the orthogonal cell profiles
	// that span one-dimensional cuts through the original slices.
	//
	// The orthogonal cell profile sets represent 1-d profiles through the bilayer 
	// that are  used to get the height field, h(x,y) of the bilayer. The 
	// discretization of the height field is set by the cell and row widths.

	CRowProfileSet rowSet(&bilayerSP, m_RowTotal);

	// Delete the CCellProfileSets used to hold the 1d bilayer data before creating
	// a new profile. We do it here so that the aggregate continues to hold its data
	// after exiting and it can be accessed by events. Any observables that depend
	// on the calculation of the height field, h(x,y), should also be zeroed here
	// and then left until the next call so that events can check their values also.

	for(CellProfileSetVectorIterator iterCP=m_vBilayerProfiles.begin(); iterCP!=m_vBilayerProfiles.end(); iterCP++)
	{
		delete *iterCP;
	}
	m_vBilayerProfiles.clear();


	for(long row=0; row<m_RowTotal; row++)
	{
		CCellProfileSet* pCPS = new CCellProfileSet(rowSet.GetOrthogonalProfile(row), m_CellTotal);

		m_vBilayerProfiles.push_back(pCPS);
	}

	// Calculate the bilayer properties in 1d profiles through the bilayer.
	//
	// IMPORTANT. Note that we reverse the order of analysing the 1d profiles
	// because we want the valarray data to be stored as a 2d matrix with the
	// first row of data corresponding to the last profile. This ensures that
	// the orientation of the data looks the same as the orientation of the
	// profiles. We are limited because we can only add 1d valarrays to the 
	// end of a 2d one.
	//
	// We zero the data here and then accumulate the results calculated by 
	// the Thickness() routine. We perform the main loop  here so that future 
	// analysis functions can be added in without having to repeat the loop.

#ifndef BILAYER_NO_USE_VALARRAY
	m_aInnerHeadNo2d = 0.0;		// n(x,y) number fields for lipid head beads
	m_aOuterHeadNo2d = 0.0;
	m_aInnerHead2d   = 0.0;		// h(x,y) height fields for lipid head beads
	m_aOuterHead2d   = 0.0;

	m_aInnerTailNo2d = 0.0;		// n(x,y) number fields for lipid tail beads
	m_aOuterTailNo2d = 0.0;
	m_aInnerTail2d   = 0.0;		// h(x,y) height fields for lipid tail beads
	m_aOuterTail2d   = 0.0;
#endif

	m_InnerWidth   = 0.0;		// Overall average of monolayer/bilayer widths
	m_OuterWidth   = 0.0;
	m_BilayerWidth = 0.0;
	m_1dProfileCounter	= 0;	// Zero 1d profile counter prior to calling Thickness()

	m_bPoreOpen = false;		// Reset the pore flag for the bilayer
	m_PoreSize	= 0;			// Reset the number of grid points within pore to zero

	for(long rowi=m_RowTotal-1; rowi>=0; rowi--)
	{
		CCellProfileSet* pBilayer1d = m_vBilayerProfiles.at(rowi);

		Thickness(pBilayer1d);
	}

	m_InnerWidth  /= static_cast<double>(m_RowTotal);
	m_OuterWidth  /= static_cast<double>(m_RowTotal);
	m_BilayerWidth = m_InnerWidth + m_OuterWidth;

	// **********************************************************************
	// Calculate bilayer properties using the 2d height fields and slice profile.
	// The stress tensor calculation needs access to the CMonitor data t get
	// the bead-bead interactions. Because we don't store the ISimBox pointer 
	// we pass it in as an argument.
	//
	// First one is area per lipid for each monolayer. We also sum the lipids
	// in each monolayer and check if any are missing. These might be in the 
	// solvent as micelles. The stress tensor calculation does not use the height fields.

	AreaPerLipid();

	Shape1d();

	Stress1d(pISimBox);

	SurfaceTension();


	// Get the ratio of actual monolayer areas to the projected area,
	// and the volume per lipid from the average bilayer thickness and
	// area per lipid

	m_InnerAreaRatio = m_InnerArea/m_ProjArea;
	m_OuterAreaRatio = m_OuterArea/m_ProjArea;

	m_VolumePerLipid = 0.5*(m_InnerAN + m_OuterAN)*m_BilayerWidth;

	// **********************************************************************
	// Create time-dependent data on the bilayer analysis and pass it to
	// the CAggregateAnalysis object for storage. We specify how many data 
	// items will be stored in the CTimeSeriesData object to avoid enlarging 
	// the arrays all the time. 
	//
	// Current data are:
	//
	// Time

	// Mean bilayer height from head coordinates
	// Mean bilayer height from tail coordinates
	// Mean bilayer thickness    "      "
	// Mean bilayer thickness    "      "
	//
	// Note that the inner and outer arrays must have the same number of 
	// elements, and this is assured by Area(). But some of the elements can
	// be zero (if there are pores, or patches of membrane occupied by a
	// different lipid type than the one used to define the membrane) so we
	// must only count the non-zero elements when normalising.

#ifndef BILAYER_NO_USE_VALARRAY

//	double heightNo = static_cast<double>(m_aInnerHead2d.size());
//	double heightNo2 = static_cast<double>(m_aInnerTail2d.size());

//	double meanHeadHeight = 0.5*(m_aInnerHead2d.sum() + m_aOuterHead2d.sum())/heightNo;
//	double meanTailHeight = 0.5*(m_aInnerTail2d.sum() + m_aOuterTail2d.sum())/heightNo;
//	double meanTailHeight = 0.0;

	// Get the thickness by subtracting one head group valarray from the other.
	// Because this alters the invoking array we cannot use it as the outer
	// monolayer height data afterwards

//	m_aOuterHead2d -= m_aInnerHead2d;
//	m_aOuterTail2d -= m_aInnerHead2d;

//	double meanHeadThickness = m_aOuterHead2d.sum()/heightNo;
//	double meanTailThickness = m_aOuterTail2d.sum()/heightNo;
//	double meanTailThickness = 0.0;

	// Perform CAnalysisTool analysis, e.g., FRAP monitoring of bead diffusion.

	for(ToolIterator iterTool=m_vTools.begin(); iterTool!=m_vTools.end(); iterTool++)
	{
		(*iterTool)->UpdateState(rSimState, pISimBox);
	}


	// Store the data in a TSD object and pass it to the aggregate state object.

	long dataTotal = 4;

	m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1, m_BilayerWidth,             "BilayerWidth");
	m_pTSD->SetValue(2, m_SurfaceTension,           "SurfaceTension");
	m_pTSD->SetValue(3, m_SurfaceTensionTrap,       "SurfaceTensionTrapezoidal");

	m_pState->AddTimeSeriesData(m_pTSD);

#endif

	// **********************************************************************
	// Create the data needed to fill the bilayer observables

	aaScalarSingle gInnerTotal(m_InnerTotal);
	aaScalarSingle gOuterTotal(m_OuterTotal);
	aaScalarSingle gMissingTotal(m_MissingTotal);
	aaScalarSingle gInnerArea(m_InnerArea);
	aaScalarSingle gOuterArea(m_OuterArea);
	aaScalarSingle gInnerAreaRatio(m_InnerAreaRatio);
	aaScalarSingle gOuterAreaRatio(m_OuterAreaRatio);
	aaScalarSingle gInnerWidth(m_InnerWidth);
	aaScalarSingle gOuterWidth(m_OuterWidth);
	aaScalarSingle gBilayerWidth(m_BilayerWidth);	
	aaScalarSingle gVolumePerLipid(m_VolumePerLipid);	
	aaScalarSingle gInnerAN(m_InnerAN);
	aaScalarSingle gOuterAN(m_OuterAN);
	aaScalarSingle gSurfaceTensionTrap(m_SurfaceTensionTrap);
	aaScalarSingle gSurfaceTension(m_SurfaceTension);
	aaScalarSingle gFirstMomentStress(m_FirstMomentOfStress);
	aaScalarSingle gSecondMomentStress(m_SecondMomentOfStress);

	         gInnerTotal.AddData(m_vObservables.at(12));
	         gOuterTotal.AddData(m_vObservables.at(13));
	       gMissingTotal.AddData(m_vObservables.at(14));
	          gInnerArea.AddData(m_vObservables.at(15));
	          gOuterArea.AddData(m_vObservables.at(16));
	     gInnerAreaRatio.AddData(m_vObservables.at(17));
	     gOuterAreaRatio.AddData(m_vObservables.at(18));
	         gInnerWidth.AddData(m_vObservables.at(19));
	         gOuterWidth.AddData(m_vObservables.at(20));
	       gBilayerWidth.AddData(m_vObservables.at(21));
	     gVolumePerLipid.AddData(m_vObservables.at(22));
	            gInnerAN.AddData(m_vObservables.at(23));
			    gOuterAN.AddData(m_vObservables.at(24));

	m_pLipidHeadDensity->AddData(m_vObservables.at(25));
	m_pLipidTailDensity->AddData(m_vObservables.at(26));
	  m_pSolventDensity->AddData(m_vObservables.at(27));
	 gSurfaceTensionTrap.AddData(m_vObservables.at(28));
	     gSurfaceTension.AddData(m_vObservables.at(29));
	  gFirstMomentStress.AddData(m_vObservables.at(30));
	 gSecondMomentStress.AddData(m_vObservables.at(31));

	        m_pTotalStress->AddData(m_vObservables.at(32));
	           m_pWWStress->AddData(m_vObservables.at(33));
	   m_pLipidHeadWStress->AddData(m_vObservables.at(34));
	   m_pLipidTailWStress->AddData(m_vObservables.at(35));
	m_pLipidHeadHeadStress->AddData(m_vObservables.at(36));
	m_pLipidHeadTailStress->AddData(m_vObservables.at(37));
	m_pLipidTailTailStress->AddData(m_vObservables.at(38));

	// Bond stress profiles: we clear the data here to avoid having to
	// use another loop just below.

		for(long bondNo=0; bondNo<m_LipidBondTypes; bondNo++)
		{	
			m_vBondStress.at(bondNo)->AddData(m_vObservables.at(39+bondNo));

			m_vBondStress.at(bondNo)->ClearData();
		}

	// BondPair stress profiles: we clear the data here to avoid having to
	// use another loop just below.

		for(long bondpairNo=0; bondpairNo<m_LipidBondPairTypes; bondpairNo++)
		{	
			m_vBondPairStress.at(bondpairNo)->AddData(m_vObservables.at(39+m_LipidBondTypes+bondpairNo));

			m_vBondPairStress.at(bondpairNo)->ClearData();
		}

	// Now the diagonal elements of the total stress

		m_pSxx->AddData(m_vObservables.at(39+m_LipidBondTypes+m_LipidBondPairTypes));
		m_pSyy->AddData(m_vObservables.at(40+m_LipidBondTypes+m_LipidBondPairTypes));
		m_pSzz->AddData(m_vObservables.at(41+m_LipidBondTypes+m_LipidBondPairTypes));


	// **********************************************************************
	// Delete the data created by the function objects for the scalar, vector and 
	// tensor profiles to avoid a memory leak. This is because the transform()
	// algorithm can only insert data into a vector of a non-zero size. I don't want
	// to keep aaScalar, etc objects around and put new data into them so I have to
	// create and delete the objects as they are used. This means that their 
	// construction and destruction should be as simple as possible.

	m_pLipidHeadDensity->ClearData();
	m_pLipidTailDensity->ClearData();
	m_pSolventDensity->ClearData();

	m_pTotalStress->ClearData();
	m_pWWStress->ClearData();
	m_pLipidHeadWStress->ClearData();
	m_pLipidTailWStress->ClearData();
	m_pLipidHeadHeadStress->ClearData();
	m_pLipidHeadTailStress->ClearData();
	m_pLipidTailTailStress->ClearData();
	m_pSxx->ClearData();
	m_pSyy->ClearData();
	m_pSzz->ClearData();


}

// **********************************************************************
// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.
//
// **********************************************************************

void CBilayer::ConvertNames(const CAnalysisState &raState)
{
	m_LipidType			= raState.GetPolymerTypeFromName(m_Polymer);
	m_LipidHeadType		= raState.GetPolymerHeadType(m_LipidType);
	m_LipidTailType		= raState.GetPolymerTailType(m_LipidType);
	m_SolventType		= raState.GetPolymerTypeFromName(m_Solvent);
	m_SolventHeadType	= raState.GetPolymerHeadType(m_SolventType);
}


// **********************************************************************
// Function to calculate the thickness of the bilayer as a 1d function
// from a CCellProfileSet. It uses the CBilayer member variables 
// m_InnerWidth and m_OuterWidth to store the inner and outer monolayer
// thickness averaged over the whole bilayer area. It also stores data
// on the number of lipids per analysis cell and their CM coordinates to
// allow the height field to be calculated.
//
// It loops over each orthogonal cell profile in the set, pCPS, and searches 
// the cells for the inner and outer monolayers: monolayers are defined 
// by the lipid head bead type. The bilayer thickness is taken as the sum 
// of the monolayer thicknesses. Each monolayer's thickness is calculated 
// from the difference of the CM of beads in each cell from the profile's 
// CM averaged over all cells, weighted by the number of beads in the cell.
//
// Physical Assumptions in this analysis
// *************************************
//
//	The CM coordinates of all the head beads in a profile define the bilayer 
//  centre at that (x,y) point. A second profile is defined using the lipid
//  tail beads.
//
//  The location of each monolayer is taken as the CM of head beads in 
//  each profile cell weighted by the number of beads in the cells. This
//  reduces the influence of single lipids that protrude from the average
//  monolayer plane.
//
//  The thicknessess of both monolayers in a given profile are equal. This 
//  is a consequence of using the CM to define the bilayer centre.
// 
// **********************************************************************

void CBilayer::Thickness(CCellProfileSet *const pCPS)
{
	// Note that the CCellProfileSet member function Size() returns the number 
	// of profiles not the size of each profile; similarly, GetOrthogonalSize() 
	// returns the number of orthogonal profiles not the cells in an orthogonal 
	// profile. Because of the way they profiles are constructed, the number of 
	// orthogonal profiles is the same as the number of cells in a profile, and 
	// the number of cells in an orthogonal profile is the same as the number of 
	// profiles. We get a 1d function with each orthogonal cell profile contributing 
	// one value of the membrane thickness.

	// If the platform is a Cray remove all code that uses valarrays

#ifndef BILAYER_NO_USE_VALARRAY
	std::valarray<double>             h1No(m_CellTotal);
	std::valarray<double>             h1CM(m_CellTotal);
	std::valarray<double>	 aInnerHeadNo(m_CellTotal);
	std::valarray<double>	 aOuterHeadNo(m_CellTotal);
	std::valarray<double> aInnerHeadHeight(m_CellTotal);
	std::valarray<double> aOuterHeadHeight(m_CellTotal);
	std::valarray<double>  aInnerThickness(m_CellTotal);
	std::valarray<double>  aOuterThickness(m_CellTotal);

	// Analyse each orthogonal cell profile, storing the lipid head bead 
	// density for each cell in a valarray. The average head bead coordinates 
	// in the cells along the bilayer normal are also calculated and stored
	// in a valarray. At the moment we only allow normals along the main
	// axes of the SimBox, but in future this constraint may be relaxed.
	// Given the average head bead coordinates in each cell we find the CM 
	// for the whole profile and measure the thickness of each monolayer.
	// We simply take the distance from the CM to the average head bead 
	// coordinate in each cell weighted by the number of head beads in the cell.

	std::valarray<double> headNo(pCPS->Size());
	std::valarray<double> headCM(pCPS->Size());

	double* pHN  = &headNo[0];
	double* pHCM = &headCM[0];

	// If there are no head beads in an orthogonal cell profile we skip the 
	// calculations and put zero in the height fields and reduce the number 
	// of cells used for normalising the thickness. The flag m_bPoreOpen is
	// set to false in the calling routine, and if a pore is found, i.e.,
	// h(x,y) = 0 for some values of x,y, it is set to true here. Note that
	// this function only calculates a 1d profile so the pore flag cannot be
	// reset here.

	long occupiedCellNo = m_CellTotal;

	for(long cop=0; cop<m_CellTotal; cop++)
	{
		CCellProfile* pOCP = pCPS->GetOrthogonalProfile(cop);

		std::transform(pOCP->Begin(), pOCP->End(), pHN,  aaCellNumber(m_LipidHeadType));

		if(m_X == 1)				// X normal to bilayer
		{
			std::transform(pOCP->Begin(), pOCP->End(), pHCM, aaCellXCM(m_LipidHeadType));
		}
		else if(m_Y == 1)			// Y normal
		{
			std::transform(pOCP->Begin(), pOCP->End(), pHCM, aaCellYCM(m_LipidHeadType));
		}

		else if(m_Z == 1)			// Z normal
		{
			std::transform(pOCP->Begin(), pOCP->End(), pHCM, aaCellZCM(m_LipidHeadType));
		}

		// h1No and h1CM hold the number of lipid heads in the orthogonal cell
		// profile and their CM respectively. They represents the data contained 
		// at one (x,y) coordinate in the projection of the membrane onto a 
		// flat plane with the same normal. Notice that we zero the valarrays here 
		// because this is not automatically done when they are created.
		//
		// Some cells may not have any lipid heads and so don't contribute to
		// the CM sum. We cannot use the size of the profile to normalize the
		// CM sum. We multiply the CM for each cell by the number of beads 
		// in the cell using the valarray multiplication operator and then 
		// average over the total number of beads in the profile. It is possible
		// that a whole cell profile has no lipids but this is unlikely. It
		// would imply a long slit pore in the membrane spanning the periodic
		// simulation box.
		// Also, an orthogonal cell profile may contain lipid head beads in one
		// of the monolayers but not the other. We check that both are non-zero
		// before normalizing by them.
		//

		aInnerHeadNo[cop]		= 0.0;
		aOuterHeadNo[cop]		= 0.0;
		aInnerHeadHeight[cop]	= 0.0;
		aOuterHeadHeight[cop]	= 0.0;
		aInnerThickness[cop]	= 0.0;
		aOuterThickness[cop]	= 0.0;

		// Get the number of head beads in the current OCP

		h1No[cop] = headNo.sum();

		if(h1No[cop] > 0.0)
		{
			h1CM[cop] = (std::operator*(headCM, headNo).sum())/h1No[cop];

// Fake the number of beads and their position so that every cell in the profile
// has the same two heights for its beads

//		h1CM[cop] = 5.0;
//		for(long ii=0; ii<pOCP->Size(); ii++)
//		{
//			headNo[ii] = 0.0;
//			headCM[ii] = 0.0;
//			if( ii%2 == 0 )
//			{
//				headNo[3] = 1.0;
//				headCM[3] = 3.0;
//				headNo[6] = 1.0;
//				headCM[6] = 6.0;
//			}
//			else
//			{
//				headNo[4] = 1.0;
//				headCM[4] = 4.0;
//				headNo[7] = 1.0;
//				headCM[7] = 7.0;
//			}
//		}


			// Loop over each cell in the orthogonal cell profile (equal to the
			// slices in the original slice profile) and find the inner and
			// outer monolayers from the CM of the lipid head beads in each
			// cell compared to the CM of lipid head beads in all cells in
			// the profile.
			// Note that headNo[] contains doubles not longs because aaCellNumber
			// (used to create it above) returns a double result. We don't perform
			// the sums when there are no head beads in a given OCP cell.

			for(long oc=0; oc<pOCP->Size(); oc++)
			{
				if(headNo[oc] > 0.0)
				{
					if(headCM[oc] < h1CM[cop])	// inner monolayer
					{
						aInnerHeadNo[cop]		+= headNo[oc];
						aInnerHeadHeight[cop]	+= headCM[oc]*headNo[oc];
						aInnerThickness[cop]	+= (h1CM[cop]-headCM[oc])*headNo[oc];
					}
					else
					{
						aOuterHeadNo[cop]		+= headNo[oc];
						aOuterHeadHeight[cop]	+= headCM[oc]*headNo[oc];
						aOuterThickness[cop]	+= (headCM[oc]-h1CM[cop])*headNo[oc];
					}
				}
			}

					// Fix the coordinates of the beads in each OCP cell to be at a fixed
					// height that depends only on whether the Cell id is even or odd.
					// We fix their CM coordinates below or above the CM depending on the
					// real coordinates but fix them arbitrarily to 3, 6 and 4, 7.


			if(aInnerHeadNo[cop] > 0.0)
			{
				aInnerHeadHeight[cop]	/= aInnerHeadNo[cop];
				aInnerThickness[cop]	/= aInnerHeadNo[cop];

//				if( cop%2 == 0 )
//				{
//					aInnerHeadHeight[cop]	= 3.0;
//					aInnerThickness[cop]	= 1.5;
//				}
//				else
//				{
//					aInnerHeadHeight[cop]	= 4.0;
//					aInnerThickness[cop]	= 1.5;
//				}

			}
			if(aOuterHeadNo[cop] > 0.0)
			{
				aOuterHeadHeight[cop]	/= aOuterHeadNo[cop];
				aOuterThickness[cop]	/= aOuterHeadNo[cop];

//				if( cop%2 == 0 )
//				{
//					aOuterHeadHeight[cop]	= 6.0;
//					aOuterThickness[cop]	= 1.5;
//				}
//				else
//				{
//					aOuterHeadHeight[cop]	= 7.0;
//					aOuterThickness[cop]	= 1.5;
//				}
			}
		}
		else
		{
			occupiedCellNo -= 1;
		}

//		double n1 = aInnerHeadNo[cop];
//		double n2 = aOuterHeadNo[cop];
//		double h1 = aInnerHeadHeight[cop];
//		double h2 = aOuterHeadHeight[cop];
//		double t1 = aInnerThickness[cop];
//		double t2 = aOuterThickness[cop];

	}

//	for(long ii=0; ii<aInnerHeadNo.size(); ii++)
//	{
//		long inNo = aInnerHeadNo[ii];
//	}

//	for(long jj=0; jj<aOuterHeadNo.size(); jj++)
//	{
//		long outNo = aOuterHeadNo[jj];
//	}

//	long inSum  = aInnerHeadNo.sum();
//	long outSum = aOuterHeadNo.sum();

	// Add the 1d data to the 2d fields. We use a counter, m_1dProfileCounter
	// to determine how many times this routine has been called, and this
	// sets the position in the 2d valarray where the new 1d profile
	// is to be added.  Each 1d profile forms a new row in the 2d array.

	// All arrays must have the same length because they are only updated here.

	// Test data filling the height arrays with simple numbers

//	static double counter = m_RowTotal-1;
//	aInnerHeadHeight[0] = counter*m_CellTotal;
//	aInnerHeadHeight[1] = counter*m_CellTotal+1.0;
//	aInnerHeadHeight[2] = counter*m_CellTotal+2.0;
//	counter--;

	long oldLength = m_CellTotal*m_1dProfileCounter++;	// Get number of elements in array and increment counter

	m_aInnerHeadNo2d[std::gslice(oldLength,m_aHeightFieldDim, m_aHeightFieldInt)] = aInnerHeadNo;
	m_aOuterHeadNo2d[std::gslice(oldLength,m_aHeightFieldDim, m_aHeightFieldInt)] = aOuterHeadNo;
	m_aInnerHead2d[std::gslice(oldLength,m_aHeightFieldDim, m_aHeightFieldInt)]   = aInnerHeadHeight;
	m_aOuterHead2d[std::gslice(oldLength,m_aHeightFieldDim, m_aHeightFieldInt)]   = aOuterHeadHeight;

	// Accumulate the data for the bilayer thickness to get an average over the SimBox.
	// We have to be careful not to count orthogonal cell profiles that do not contain
	// any lipids: these might be pores or just fluctuations but they do not contribute
	// to the bilayer thickness average. Similarly, we guard against the remote
	// possibility that a whole 1d profile has no lipids in it.

	if(occupiedCellNo > 0)
	{
		m_InnerWidth += aInnerThickness.sum()/static_cast<double>(occupiedCellNo);
		m_OuterWidth += aOuterThickness.sum()/static_cast<double>(occupiedCellNo);
	}

	// If any grid points are empty, indicating a pore, add the number of such
	// points together to get an estimate of the pore size and set a flag indicating
	// that a pore exists. We do not yet distinguish between a single pore and
	// many pores.

	if(occupiedCellNo != m_CellTotal)
	{
		m_bPoreOpen = true;
		m_PoreSize  += (m_CellTotal - occupiedCellNo);
	}

#endif	// End of valarray code removed for compilation on the Cray

}

// **********************************************************************
// Function to calculate the area per lipid for both monolayers in the 
// bilayer using the height field data generated by Thickness().
//
//
// Physical Assumptions in this analysis
// *************************************
//
//	Each monolayer's surface is approximated by the grid into which
//	the SimBox is broken up and the average height of the lipids at each
//	grid point is used to represent the monolayer surface. The
//	grid point is used for the (x,y) coordinates of the cells,
//	although this information is not used here as we only need the 
//	relative position vectors of adjacent cells.
//
// If the height at any of the grid points is zero we ignore the area
// calculated using that point. This can arise if the grid size is
// is comparable or smaller than the bead size or if there are pores
// in the bilayer. In both cases, we only calculate contributions to
// the area from "intact" parts of the membrane surface.
//

void CBilayer::AreaPerLipid(void)
{
	// If the platform is a Cray remove all code using valarrays

#ifndef BILAYER_NO_USE_VALARRAY

	// Copy the inner monolayer height field data into a valarray with one
	// extra entry in each dimensions. This allows us to copy the original
	// array in and wrap the first row and column around making the 
	// extraction of the 2 x 2 arrays used for triangle area calculations
	// easier.

//	for(long myIndex=0; myIndex<m_CellTotal*m_RowTotal; myIndex++)
//	{
//		m_aInnerHead2d[myIndex] = 1.0;
//		double innerHxy = m_aInnerHead2d[myIndex];
//	}

	// Zero the member variables holding the A/N data

	m_aInnerAN2d = 0.0;
	m_aOuterAN2d = 0.0;

	// Define a local array with space for an extra row and column
	
	std::valarray<double> heightDataInn(0.0, (m_CellTotal+1)*(m_RowTotal+1));
	std::valarray<double> heightDataOut(0.0, (m_CellTotal+1)*(m_RowTotal+1));

	// Copy the height data to the new one. We define gslices to hold the
	// dimensions of the height array so that it can be copied into the
	// the new array and the extra column and row added.

	std::valarray<size_t> Dim(2);
	std::valarray<size_t> Int(2);

	Dim[0] = m_RowTotal;
	Dim[1] = m_CellTotal;
	Int[0] = m_CellTotal+1;
	Int[1] = 1;

//	heightDataInn[gslice(0,Dim,Int)] = m_aInnerHead2d;
//	heightDataOut[gslice(0,Dim,Int)] = m_aOuterHead2d;

	// Copy the first row to the last row of the new one

	Dim[0] = 1;
	Dim[1] = m_CellTotal;
	Int[0] = 0;
	Int[1] = 1;

//	heightDataInn[gslice((m_CellTotal+1)*m_RowTotal,Dim,Int)] = m_aInnerHead2d[slice(0,m_CellTotal,1)];
//	heightDataOut[gslice((m_CellTotal+1)*m_RowTotal,Dim,Int)] = m_aOuterHead2d[slice(0,m_CellTotal,1)];

	// Copy the first column to the last column of the new one

	Dim[0] = m_RowTotal;
	Dim[1] = 1;
	Int[0] = m_CellTotal+1;
	Int[1] = 0;

//	heightDataInn[gslice(m_CellTotal,Dim,Int)] = m_aInnerHead2d[slice(0,m_RowTotal,m_CellTotal)];
//	heightDataOut[gslice(m_CellTotal,Dim,Int)] = m_aOuterHead2d[slice(0,m_RowTotal,m_CellTotal)];

	// Finally, copy the first element to the last element

//	heightDataInn[(m_CellTotal+1)*(m_RowTotal+1)-1] = m_aInnerHead2d[0];
//	heightDataOut[(m_CellTotal+1)*(m_RowTotal+1)-1] = m_aOuterHead2d[0];
	
/*	for(long myIndex=0; myIndex<(m_CellTotal+1)*(m_RowTotal+1); myIndex++)
	{
		double innerHxy = heightDataInn[myIndex];
		double outerHxy = heightDataOut[myIndex];
	}
*/
	// Extract each 2 x 2 matrix from the height data and store the lengths of the
	// triangle's arms. Note we increment the index by hand to take account of
	// the unusual way we extract the matrices.

	long index     = 0;
	long indexArea = 0;

	double Z21;
	double Z31;
	double Z41;
	double area123;
	double area134;

	for(long iRow=0; iRow<m_RowTotal; iRow++)
	{
		for(long iCell=0; iCell<m_CellTotal; iCell++)
		{
			index = iRow*(m_CellTotal+1) + iCell;

			// Triangles are formed from the four elements:
			//
			// (index                   index + 1            )
			// (index + m_CellTotal + 1 index + m_CellTotal+2)
			//
			// If any of the cells have zero height this means there are no
			// lipids in that orthogonal cell profile. We ignore triangles  
			// that would include such a zero-height corner. Set the areas
			// of each triangle to zero and then check that triangles 123 and
			// 134 have the appropriate corner heights non-zero before calculating
			// their areas.
			//
			//   1---2  Conditions:  Both heights at 1 and 3 > 0
			//   | \ |               If height at 2 > 0 area123 contributes
			//   4---3               If height at 4 > 0 area134 contributes
			//
			// We leave the array element at zero in the A/N data so we can 
			// perhaps use the information on where the height was zero for
			// further analysis.

			// Find the inner monolayer's area first

			if(heightDataInn[index] > 0.0 && heightDataInn[index+m_CellTotal+2] > 0.0)
			{
				if(heightDataInn[index+1] > 0.0)
				{
					Z21 = heightDataInn[index+1] - heightDataInn[index];
					Z31 = heightDataInn[index+m_CellTotal+2] - heightDataInn[index];

					area123 = m_CellWidth*sqrt((Z21*Z21*m_RowOverCellSq + (Z21-Z31)*(Z21-Z31) + m_RowWidthSq));
				}
				else
					area123 = 0.0;

				if(heightDataInn[index+m_CellTotal+1] > 0.0)
				{
					Z31 = heightDataInn[index+m_CellTotal+2] - heightDataInn[index];
					Z41 = heightDataInn[index+m_CellTotal+1] - heightDataInn[index];

					area134 = m_CellWidth*sqrt(((Z41-Z31)*(Z41-Z31)*m_RowOverCellSq + Z41*Z41 + m_RowWidthSq));
				}
				else
					area134 = 0.0;

				// We use a distinct index because now we are accessing an array with
				// the original size, not with one extra row and column. We also count
				// the number of grid elements that have non-zero heights for later
				// normalization.

				m_aInnerAN2d[indexArea] = area123 + area134;
			}

			// now the outer monolayer

			if(heightDataOut[index] > 0.0 && heightDataOut[index+m_CellTotal+2] > 0.0)
			{
				if(heightDataOut[index+1] > 0.0)
				{
					Z21 = heightDataOut[index+1] - heightDataOut[index];
					Z31 = heightDataOut[index+m_CellTotal+2] - heightDataOut[index];

					area123 = m_CellWidth*sqrt((Z21*Z21*m_RowOverCellSq + (Z21-Z31)*(Z21-Z31) + m_RowWidthSq));
				}
				else
					area123 = 0.0;

				if(heightDataOut[index+m_CellTotal+1] > 0.0)
				{
					Z31 = heightDataOut[index+m_CellTotal+2] - heightDataOut[index];
					Z41 = heightDataOut[index+m_CellTotal+1] - heightDataOut[index];

					area134 = m_CellWidth*sqrt(((Z41-Z31)*(Z41-Z31)*m_RowOverCellSq + Z41*Z41 + m_RowWidthSq));
				}
				else
					area134 = 0.0;

				// We use a distinct index because now we are accessing an array with
				// the original size, not with one extra row and column.

				m_aOuterAN2d[indexArea] = area123 + area134;
			}

			indexArea++;
		}
	}

	// Put back the missing factor of 0.5 from the triangle areas

	m_aInnerAN2d *= 0.5;
	m_aOuterAN2d *= 0.5;

/*	for(long iii=0; iii<m_aInnerAN2d.size(); iii++)
	{
		double myArea1   = m_aInnerAN2d[iii];
		double myArea2   = m_aOuterAN2d[iii];
		double myLipids1 = m_aInnerHeadNo2d[iii];
		double myLipids2 = m_aOuterHeadNo2d[iii];
	}
*/
	// Store the lipids in each monolayer and monolayer areas calculated 
	// from the cells

	m_InnerTotal   = static_cast<long>(m_aInnerHeadNo2d.sum());
	m_OuterTotal   = static_cast<long>(m_aOuterHeadNo2d.sum());
	m_MissingTotal = m_LipidTotal - (m_InnerTotal + m_OuterTotal);
	m_InnerArea    = m_aInnerAN2d.sum();
	m_OuterArea    = m_aOuterAN2d.sum();

	// Calculate the A/N by dividing the area per cell by the number of headgroups
	// in each orthogonal cell profile. But because this quantity can be zero,
	// we just sum the total areas and divide by the total number of lipids.

//	m_aInnerAN2d /= m_aInnerHeadNo2d;
//	m_aOuterAN2d /= m_aOuterHeadNo2d;

//	m_InnerAN = m_aInnerAN2d.sum()/m_RowCellTotal;
//	m_OuterAN = m_aOuterAN2d.sum()/m_RowCellTotal;

	m_InnerAN   = m_InnerArea/static_cast<double>(m_InnerTotal);
	m_OuterAN   = m_OuterArea/static_cast<double>(m_OuterTotal);

#endif	// End of valarray code removed for compilation on the Cray

}

// Function to calculate the 1d Fourier transform of the bilayer height data. 
// We take the height field for the bilayer from the lipid head coordinates and 
// use the FT routines to calculate the spectrum of fluctuations in the shape. 
// This is then fitted to an assumed functional form and the bending stiffness 
// extracted.
//
// The number of points in the h(x) data set is m_CellTotal. But because we use
// the Transform() routine, which requires a set of complex data, we transform
// both the inner and out head bead height fields requiring 2*m_CellTotal points,
// and the arrays are 1-indexed in the CFT1d routines so we add an extra point.

void CBilayer::Shape1d()
{
#if EnableMiscClasses == SimMiscEnabled
#ifndef BILAYER_NO_USE_VALARRAY

	// The total input data consists of height fields h(x) for both upper and
	// lower monolayers. There are m_CellTotal points in each field. This must
	// be a power of 2 or the data must be padded out to the next power of 2.
	// The CFT1d routines used to do the FTs must work on data sets with
	// 2**N points.

	const long complexSize = m_CellTotal;
	const long realSize    = 2*m_CellTotal + 1;

//	const long complexSize = 32;	// No of complex elements for Transform()
//	const long realSize    = 2*complexSize + 1;

	std::valarray<double> data(realSize);

	data[0] = 0.0;

	for(long i=1; i<=complexSize; i++)
	{
		data[2*i-1] = 0.5*(m_aInnerHead2d[i-1] + m_aOuterHead2d[i-1]);
		data[2*i]   = 0.0;

//		data[2*i-1] = cos(4.0*3.14159265359*static_cast<double>(i)/static_cast<double>(complexSize));
//		data[2*i]   = sin(4.0*3.14159265359*static_cast<double>(i)/static_cast<double>(complexSize));
		
//		double x1 = data[2*i-1];
//		double x2 = data[2*i];
	}


#endif
#endif
}

// Function to calculate the stress tensor averaged over sections through the slices
// in a CSliceProfile through the SimBox. The analysis slices may be thinner than
// the CNT cell slices used to hold the beads. The overloaded CAnalysis base class 
// function, StressTensorProfile(), returns a pointer to an aaStressTensor1d object 
// that is used to calculate and store the average stress tensor. The selection of
// the 1d calculation is based on the type of CProfile<> passed in as the second
// argument.
//
// Note that the stress tensor is calculated anew each time the bilayer analysis
// is done, so we have to delete the previous aaStressTensor1d object before
// storing the new one. The width of the slices need not be the same as the
// CNT cell width, in fact, it should be much smaller to get an accurate picture
// of the stress in the bilayer.
//

void CBilayer::Stress1d(const ISimBox* const pISimBox)
{
	if(m_pStress1d)
		delete m_pStress1d;

	// Because the overloaded CAnalysis::StressTensorProfile() functions all 
	// return an aaStressTensor* we have to cast it to the appropriate real type

	m_pStress1d = dynamic_cast<aaStressTensor1d*>(StressTensorProfile(pISimBox, m_pLipidSP, m_X, m_Y, m_Z));
}

// Function to calcuate the surface tension of the bilayer from the stress tensor
// averaged over thin slices parallel to the bilayer. It also calculates the moments
// of the stress tensor averaged over slices. Only the first and second moments
// are output for now.
//
// The surface tension across a planar interface is given by the difference of the
// normal and transverse components of the stress tensor integrated across the
// interface.
//
// This routine also outputs the stress tensor for each type of interaction in the
// bilayer: these are Water-Water, Head-Water, Tail-Water, Head-Head, Head-Tail, 
// Tail-Tail, and all bond types in the lipid polymers. We extract the tensor 
// contributions for each interaction type and store them in scalar profiles 
// across the SimBox.


void CBilayer::SurfaceTension()
{
	// Get the contributions to the stress tensor from each bead-bead interaction
	// and bond type, and convert it into a scalar pressure profile using a 
	// function object that iterates over a vector of aaStressTensorPoints.
	//
	// Note that the total stress contains contributions from all beads not just the
	// solvent, head and tail beads. Hence it may show non-zero values even when the 
	// selected bead-bead interactions show zero.

#ifndef BILAYER_NO_USE_VALARRAY

	transform(m_pStress1d->Begin(), m_pStress1d->End(),            m_pWWStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[0]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(),    m_pLipidHeadWStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[1]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(),    m_pLipidTailWStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[2]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pLipidHeadHeadStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[3]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pLipidHeadTailStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[4]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pLipidTailTailStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[5]));

	for(long bondNo=0; bondNo<m_LipidBondTypes; bondNo++)
	{
		aaScalarProfile* pBondStressPro = m_vBondStress.at(bondNo);

		transform(m_pStress1d->Begin(), m_pStress1d->End(), pBondStressPro->Begin(), aaBondStressZProfile(m_vBondStressIndex[bondNo]));
	}

	for(long bondpairNo=0; bondpairNo<m_LipidBondPairTypes; bondpairNo++)
	{
		aaScalarProfile* pBondPairStressPro = m_vBondPairStress.at(bondpairNo);

		transform(m_pStress1d->Begin(), m_pStress1d->End(), pBondPairStressPro->Begin(), aaBondPairStressZProfile(m_vBondPairStressIndex[bondpairNo]));
	}

	// Now do the total stress from all bead-bead, bond and bondpair contributions
	
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pTotalStress->Begin(), aaStressZProfile());

	// Now do the diagonal components of the total stress
	
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pSxx->Begin(), aaStressComponentProfile(0));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pSyy->Begin(), aaStressComponentProfile(4));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pSzz->Begin(), aaStressComponentProfile(8));

	// Use the midpoint of each slice to define the moments

	double z				= 0.0;

	m_SurfaceTension		= 0.0;
	m_SurfaceTensionTrap	= 0.0;
	m_FirstMomentOfStress	= 0.0;
	m_SecondMomentOfStress	= 0.0;

	for(long i=0; i<m_pStress1d->GetSize(); i++)
	{
		z = (0.5 + static_cast<double>(i))*m_pStress1d->GetWidth();

		/*
		double sww = m_pWWStress->GetData(i);
		double shw = m_pLipidHeadWStress->GetData(i);
		double stw = m_pLipidTailWStress->GetData(i);
		double shh = m_pLipidHeadHeadStress->GetData(i);
		double sht = m_pLipidHeadTailStress->GetData(i);
		double stt = m_pLipidTailTailStress->GetData(i);

		double sww11 = m_pStress1d->GetBeadStress11(i, m_vBeadStressIndex[0]);
		double sww12 = m_pStress1d->GetBeadStress12(i, m_vBeadStressIndex[0]);
		double sww13 = m_pStress1d->GetBeadStress13(i, m_vBeadStressIndex[0]);
		double sww21 = m_pStress1d->GetBeadStress21(i, m_vBeadStressIndex[0]);
		double sww22 = m_pStress1d->GetBeadStress22(i, m_vBeadStressIndex[0]);
		double sww23 = m_pStress1d->GetBeadStress23(i, m_vBeadStressIndex[0]);
		double sww31 = m_pStress1d->GetBeadStress31(i, m_vBeadStressIndex[0]);
		double sww32 = m_pStress1d->GetBeadStress32(i, m_vBeadStressIndex[0]);
		double sww33 = m_pStress1d->GetBeadStress33(i, m_vBeadStressIndex[0]);

		for(long bondNo=0; bondNo<m_LipidBondTypes; bondNo++)
		{
			const long bondType        = m_vBondTypeIndex.at(bondNo);
			const long bondStressIndex = m_vBondStressIndex.at(bondNo);

			aaScalarProfile* pBondStressPro = m_vBondStress.at(bondNo);

			double sbond = pBondStressPro->GetData(i);
		}

		for(long bondpairNo=0; bondpairNo<m_LipidBondPairTypes; bondpairNo++)
		{
			const long bondPairType        = m_vBondPairTypeIndex.at(bondpairNo);
			const long bondPairStressIndex = m_vBondPairStressIndex.at(bondpairNo);

			aaScalarProfile* pBondPairStressPro = m_vBondPairStress.at(bondpairNo);

			double sbondpair = pBondPairStressPro->GetData(i);
		}

		// Write out the diagonal elements of the stress tensor for debug purposes

		double sxx = m_pStress1d->GetComponent(i,0);
		double syy = m_pStress1d->GetComponent(i,4);
		double szz = m_pStress1d->GetComponent(i,8);
		double press = szz - 0.5*(sxx + syy);
*/

		double stotal = m_pTotalStress->GetData(i);

		m_SurfaceTension		+= stotal;
		m_FirstMomentOfStress	+= z*stotal;
		m_SecondMomentOfStress	+= z*z*stotal;

	}

	// Multiply in the width of the stress slices to complete the integral approximation

	m_SurfaceTension		*= m_pStress1d->GetWidth();
	m_FirstMomentOfStress	*= m_pStress1d->GetWidth();
	m_SecondMomentOfStress	*= m_pStress1d->GetWidth();
	
	// Now calculate the surface tension using the Trapezoidal rule
	
	m_SurfaceTensionTrap = TrapezoidalRule(m_pStress1d->GetWidth());
	

#endif

}


// Trapezoidal approximation to the surface tension integral. We use the stored values of the stress profile calculated above. 
// The infinitesimal width of the bins is passed in as the first argument.
// Note that dividing the integration range into N points actually requires N+1 values of the function as we integrate from 0 to N*h inclusively.
// There must be enough points in the stress profile to account for this.

double CBilayer::TrapezoidalRule(const double h)
{
	// As a debug test, we can calculate the integralof r*r over the interval 0 to LZ. Note that we need to use m_ShellTotal+1 points to make sure
	// that we include the end-points required in the Trapezoidal rule correctly. Also, the x coordinate (r above) should start at 0.0 for the test
	// otherwise we are integrating from h/2 to LZ+h/2.
	//
	// If we take N = 100, we have points from 0 to 100, ie 101 points, so we need to add the final point in the vector separately
    // in = 1 to 99, and we add in the first and last points
	
	const long N = static_cast<long>(m_pStress1d->GetSize())-1;
	
	double tension;
	tension = 0.0;
	
	for(long in=1; in<N; in++)
	{
		tension += m_pTotalStress->GetData(in);
	}
	
	tension += 0.5*(m_pTotalStress->GetData(0) + m_pTotalStress->GetData(N));
	tension *= h;
	
    return tension;
}


// **********************************************************************
// IEventXXX interface functions for all supported events.

// IEventLamellaPore and IEventLamellaPoreClosed
// *********************************************
//
// The IEventLamellaPoreClosed event only needs the GetPoreSize() function.

bool CBilayer::LamellaPoreSupported() const
{
	return true;
}

bool CBilayer::LamellaPoreClosedSupported() const
{
	return true;
}

bool CBilayer::IsPoreOpen() const
{
	return m_bPoreOpen;
}

long CBilayer::GetPoreSize() const
{
	return m_PoreSize;
}

// IEventLamellaCusp
// *****************
//

bool CBilayer::LamellaCuspSupported() const
{
	return true;
}

bool CBilayer::IsCuspPresent() const
{
	return true;
}

double CBilayer::GetCurvature() const
{
	return 0.0;
}


// **********************************************************************
// IBilayerXXX interface functions for all supported processes.
//
// IBilayerFusion
// **************
//
// The bilayer centre of mass is obtained from the coordinates of the head beads only.

double CBilayer::GetCMXPos() const
{
	return m_rcmHead[0];
}

double CBilayer::GetCMYPos() const
{
	return m_rcmHead[1];
}

double CBilayer::GetCMZPos() const
{
	return m_rcmHead[2];
}

long CBilayer::GetMajorLipidType() const
{
	return m_LipidType;
}

long CBilayer::GetMajorLipidHeadType() const
{
	return m_LipidHeadType;
}

long CBilayer::GetMajorLipidTailType() const
{
	return m_LipidTailType;
}

double CBilayer::GetBilayerHeight() const
{
	return 0.5*(m_BilayerHeight1 + m_BilayerHeight2);
}

double CBilayer::GetInnerHeadHeight() const
{
	return m_InnerHeight1;
}

double CBilayer::GetOuterHeadHeight() const
{
	return m_OuterHeight1;
}

double CBilayer::GetInnerTailHeight() const
{
	return m_InnerHeight2;
}

double CBilayer::GetOuterTailHeight() const
{
	return m_OuterHeight2;
}
