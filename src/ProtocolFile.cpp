/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ProtocolFile.cpp: implementation of the CProtocolFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "ProtocolFile.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "xxCommand.h"

#include "InitialStateRandom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProtocolFile::CProtocolFile(const ISimBox* const pISimBox,
							 const zString simType, const zString protocol, 
							 const zString runId, const zString title, const zString date,
							 const zString comment) : xxFile("ep" + protocol + "." + runId + ".xml", true), 
													m_pISimBox(pISimBox),
													m_UserName("John Doe"),
													m_UserID("000"),
													m_UserEmail("myname@mydomain.com"),
													m_ProjectTitle("Default Project Title"),
													m_StartDate("1/1/04"),
													m_Duration("1"),
													m_DurationUnits("Years"),
													m_PrincipalInvestigator("John Doe"),
													m_ProjectDescription("Default project description"),					
													m_SimType(simType),
													m_Protocol(protocol),
													m_RunId(runId),
													m_Title(title),
													m_Date(date),
													m_Comment(comment),
													m_CNTXCellNo(pISimBox->GetCNTXCellNo()),
													m_CNTYCellNo(pISimBox->GetCNTYCellNo()),
													m_CNTZCellNo(pISimBox->GetCNTZCellNo()),
													m_CNTXCellWidth(pISimBox->GetCNTXCellWidth()),
													m_CNTYCellWidth(pISimBox->GetCNTYCellWidth()),
													m_CNTZCellWidth(pISimBox->GetCNTZCellWidth()),
													m_GridXCellNo(pISimBox->GetGridXCellNo()),
													m_GridYCellNo(pISimBox->GetGridYCellNo()),
													m_GridZCellNo(pISimBox->GetGridZCellNo()),
													m_Density(pISimBox->GetDensity()),
													m_Temperature(pISimBox->GetTemperature()),
													m_RNGSeed(pISimBox->GetRNGSeed()),
													m_Lambda(pISimBox->GetLambda()),
													m_StepSize(pISimBox->GetStepSize()),
													m_StepTotal(pISimBox->GetTotalTime()),
													m_SamplePeriod(pISimBox->GetSamplePeriod()),
													m_AnalysisPeriod(pISimBox->GetAnalysisPeriod()),
													m_DensityPeriod(pISimBox->GetDensityPeriod()),
													m_DisplayPeriod(pISimBox->GetDisplayPeriod()),
													m_RestartPeriod(pISimBox->GetRestartPeriod()),
													m_BeadTypes(pISimBox->GetBeadTypes()),
													m_BondTypes(pISimBox->GetBondTypes()),
													m_BondPairTypes(pISimBox->GetBondPairTypes()),
													m_PolymerTypes(pISimBox->GetPolymerTypes()),
													m_Commands(pISimBox->GetCommands())
{

}

CProtocolFile::~CProtocolFile()
{

}

// Function to write out the data needed for a protocol file.
// The fle name is composed of the runId from the control data file,
// a keyword obtained from the name of the initial state, and the 
// suffix ".xml".

bool CProtocolFile::Serialize()
{
	// Only write the protocol file if xml is enabled

#if EnableXMLProtocol == SimXMLEnabled

	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here.

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"protocol.css\" ?>" << zEndl;

	// now the top-level Protocol element with its type and runId attributes

	m_outStream << "<Protocol type = \"" << m_Protocol << "\" runId = \"" << m_RunId << "\">" << zEndl;

// ****************************************
	m_outStream << "<!-- Experiment Data -->" << zEndl;

	m_outStream << "<Experiment>" << zEndl;

	m_outStream << "<User authorisationCode = \"" << m_UserID << "\">" << zEndl;
	m_outStream << "    <Name>"  << m_UserName  << "</Name>"  << zEndl;
	m_outStream << "    <Email>" << m_UserEmail << "</Email>" << zEndl;
	m_outStream << "</User>" << zEndl;

	m_outStream << "<Project durationUnits = \"" << m_DurationUnits << "\">"	<< zEndl;
	m_outStream << "    <Title>"		<< m_ProjectTitle << "</Title>"			<< zEndl;
	m_outStream << "    <StartDate>"    << m_StartDate    << "</StartDate>"		<< zEndl;
	m_outStream << "    <Duration>"		<< m_Duration     << "</Duration>"		<< zEndl;
	m_outStream << "    <PrincipalInvestigator>"		  << m_PrincipalInvestigator <<"</PrincipalInvestigator>" << zEndl;
	m_outStream << "    <Description>"		<< m_ProjectDescription << "</Description>" << zEndl;
	m_outStream << "</Project>" << zEndl;

	m_outStream << "<Security>"  << zEndl;
	m_outStream << "</Security>" << zEndl;

	m_outStream << "<Deployment>"  << zEndl;
	m_outStream << "</Deployment>" << zEndl;

	m_outStream << "</Experiment>" << zEndl;

// ****************************************
	m_outStream << "<!-- Simulation Data -->" << zEndl;

	m_outStream << "<Simulation simType = \"" << m_SimType << "\">" << zEndl;

	m_outStream << "<Header>"  << zEndl;
	m_outStream << "    <Title>"   << m_Title   << "</Title>"   << zEndl;
	m_outStream << "    <Date>"    << m_Date    << "</Date>"    << zEndl;
	m_outStream << "    <Comment>" << m_Comment << "</Comment>" << zEndl;
	m_outStream << "</Header>" << zEndl;

	m_outStream << "<SimBox>" << zEndl;
	m_outStream << "    <BoxSize>" << zEndl;
	m_outStream << "        <LX>" << m_CNTXCellNo << "</LX>" << zEndl;
	m_outStream << "        <LY>" << m_CNTYCellNo << "</LY>" << zEndl;
	m_outStream << "        <LZ>" << m_CNTZCellNo << "</LZ>" << zEndl;
	m_outStream << "    </BoxSize>" << zEndl;

	m_outStream << "    <CNTCellSize>" << zEndl;
	m_outStream << "        <DX>" << m_CNTXCellWidth << "</DX>" << zEndl;
	m_outStream << "        <DY>" << m_CNTYCellWidth << "</DY>" << zEndl;
	m_outStream << "        <DZ>" << m_CNTZCellWidth << "</DZ>" << zEndl;
	m_outStream << "    </CNTCellSize>" << zEndl;

	m_outStream << "    <GridSize>" << zEndl;
	m_outStream << "        <LX>" << m_GridXCellNo << "</LX>" << zEndl;
	m_outStream << "        <LY>" << m_GridYCellNo << "</LY>" << zEndl;
	m_outStream << "        <LZ>" << m_GridZCellNo << "</LZ>" << zEndl;
	m_outStream << "    </GridSize>" << zEndl;

	m_outStream << "	<Density>"		<< m_Density		<< "</Density>"		<< zEndl;
	m_outStream << "	<Temperature>"	<< m_Temperature	<< "</Temperature>" << zEndl;
	m_outStream << "	<RNGSeed>"		<< m_RNGSeed		<< "</RNGSeed>"		<< zEndl;

	m_outStream << "	<Integration simType = \"" << m_SimType << "\">" << zEndl;

	// The DPD integration scheme requires the lambda parameter whereas MD does not

#if SimIdentifier == BD

	m_outStream << "	<Lambda>" << m_Lambda << "</Lambda>" << zEndl;

#elif SimIdentifier == DPD

	m_outStream << "	<Lambda>" << m_Lambda << "</Lambda>" << zEndl;

#elif SimIdentifier == MD

#endif

	m_outStream << "	</Integration>" << zEndl;
	m_outStream << "</SimBox>" << zEndl;

	m_outStream << "<SimTimes>" << zEndl;
	m_outStream << "    <StepSize>"			<< m_StepSize		<< "</StepSize>"		<< zEndl;
	m_outStream << "    <StepTotal>"		<< m_StepTotal		<< "</StepTotal>"		<< zEndl;
	m_outStream << "    <SamplePeriod>"		<< m_SamplePeriod	<< "</SamplePeriod>"	<< zEndl;
	m_outStream << "    <AnalysisPeriod>"	<< m_AnalysisPeriod << "</AnalysisPeriod>"	<< zEndl;
	m_outStream << "    <DensityPeriod>"	<< m_DensityPeriod	<< "</DensityPeriod>"	<< zEndl;
	m_outStream << "    <DisplayPeriod>"	<< m_DisplayPeriod	<< "</DisplayPeriod>"	<< zEndl;
	m_outStream << "    <RestartPeriod>"	<< m_RestartPeriod	<< "</RestartPeriod>"	<< zEndl;
	m_outStream << "</SimTimes>" << zEndl;

	m_outStream << "</Simulation>" << zEndl;

// ****************************************
	m_outStream << "<!-- Substrate Data -->" << zEndl;

	m_outStream << "<Substrate simType = \"" << m_SimType << "\">" << zEndl;

	m_outStream << "	<Polymers>" << zEndl;
	for(PolymerVectorIterator iterPoly=m_PolymerTypes.begin(); iterPoly!=m_PolymerTypes.end(); iterPoly++)
	{
		m_outStream << "        <Polymer>" << zEndl;
		m_outStream << "        <Name>" << m_pISimBox->GetPolymerNameFromType((*iterPoly)->GetType()) << "</Name>" << zEndl;
		m_outStream << "            " << m_Parser.ConvertToTags(m_pISimBox->GetPolymerShapeFromType((*iterPoly)->GetType())) << zEndl;
		m_outStream << "	    </Polymer>" << zEndl;
	}
	m_outStream << "	</Polymers>" << zEndl;

	m_outStream << "	<Beads>" << zEndl;

	long bc = 0;	// Counter for current bead type

	for(BeadVectorIterator iterBead=m_BeadTypes.begin(); iterBead!=m_BeadTypes.end(); iterBead++)
	{
		m_outStream << "	    <Bead>" << zEndl;
		m_outStream << "	        <Name>" << m_pISimBox->GetBeadNameFromType((*iterBead)->GetType()) << "</Name>" << zEndl;
		m_outStream << "	        <Radius>" << (*iterBead)->GetRadius() << "</Radius>" << zEndl;

		m_outStream << "   	        ";

		// Loop over previous bead types for conservative and dissipative parameters

		for(long consbc=0; consbc<=bc;consbc++)
		{
			m_outStream << "<ConsInt>" << m_pISimBox->GetDPDBeadConsInt(bc, consbc) << "</ConsInt>";
		}
		m_outStream << zEndl << "            ";

		for(long dissbc=0; dissbc<=bc;dissbc++)
		{
			m_outStream << "<DissInt>" << m_pISimBox->GetDPDBeadDissInt(bc, dissbc) << "</DissInt>";
		}
		m_outStream << zEndl << "        </Bead>" << zEndl;
		bc++;
	}
	m_outStream << "	</Beads>" << zEndl;

	m_outStream << "	<Bonds>" << zEndl;
	for(BondVectorIterator iterBond=m_BondTypes.begin(); iterBond!=m_BondTypes.end(); iterBond++)
	{
		const long bondType = (*iterBond)->GetType();

		m_outStream << "	    <Bond type=\"Hookean\">" << zEndl;
		m_outStream << "	        <Head>" << m_pISimBox->GetBondHeadNameFromType(bondType)  << "</Head>" << zEndl;
		m_outStream << "	        <Tail>" << m_pISimBox->GetBondTailNameFromType(bondType)  << "</Tail>" << zEndl;
		m_outStream << "	        <SpringConstant>"    << (*iterBond)->GetSprConst()		<< "</SpringConstant>" << zEndl;
		m_outStream << "	        <UnstretchedLength>" << (*iterBond)->GetUnStrLength()	<< "</UnstretchedLength>" << zEndl;
		m_outStream << "	    </Bond>" << zEndl;
	}
	m_outStream << "	</Bonds>" << zEndl;

	m_outStream << "	<BondPairs>" << zEndl;
	for(BondPairVectorIterator iterBP=m_BondPairTypes.begin(); iterBP!=m_BondPairTypes.end(); iterBP++)
	{
		const long bondPairType = (*iterBP)->GetType();

		m_outStream << "	    <BondPair>" << zEndl;
		m_outStream << "	        <Head>"   << m_pISimBox->GetBondPairHeadNameFromType(bondPairType)     << "</Head>"   << zEndl;
		m_outStream << "	        <Middle>" << m_pISimBox->GetBondPairMiddleNameFromType(bondPairType)   << "</Middle>" << zEndl;
		m_outStream << "	        <Tail>"   << m_pISimBox->GetBondPairTailNameFromType(bondPairType)     << "</Tail>"   << zEndl;
		m_outStream << "	        <BendingModulus>"	<< (*iterBP)->GetModulus()	<< "</BendingModulus>" << zEndl;
		m_outStream << "	        <PreferredAngle>"	<< (*iterBP)->GetPhi0()		<< "</PreferredAngle>" << zEndl;
		m_outStream << "	    </BondPair>" << zEndl;
	}
	m_outStream << "	</BondPairs>" << zEndl;

	m_outStream << "</Substrate>" << zEndl;

// ****************************************
	m_outStream << "<!-- Sculpture Data -->" << zEndl;

	m_outStream << "<Sculpture>" << zEndl;

	// We only allow the random state to be written at the moment

	if(m_pISimBox->GetInitialStateType() == CInitialStateRandom::GetType())
	{
		m_outStream << "    <Random>" << zEndl;

		for(PolymerVectorIterator iterPoly1=m_PolymerTypes.begin(); iterPoly1!=m_PolymerTypes.end(); iterPoly1++)
		{
			m_outStream << "	    <PolymerFraction name = \" " << m_pISimBox->GetPolymerNameFromType((*iterPoly1)->GetType()) << "\"" 
								            << " fraction = \" " << m_pISimBox->GetPolymerFractionFromType((*iterPoly1)->GetType()) << "\" />" << zEndl;
		}

		m_outStream << "    </Random>" << zEndl;
	}
	m_outStream << "</Sculpture>" << zEndl;

// ****************************************
	m_outStream << "<!-- ProbeField Data -->" << zEndl;


// ****************************************
	m_outStream << "<!-- ExternalField Data -->" << zEndl;


// ****************************************
	m_outStream << "<!-- Processes Data -->" << zEndl;

	m_outStream << "<Processes>" << zEndl;

	// Iterate over the commands writing out their arguments

#if EnableXMLCommands == SimXMLEnabled

	m_outStream << "<Commands>" << zEndl;

	for(CommandIterator iterCom=m_Commands.begin(); iterCom!=m_Commands.end(); iterCom++)
	{
		(*iterCom)->put(m_outStream);
	}

	m_outStream << "</Commands>" << zEndl;

#endif
	m_outStream << "</Processes>" << zEndl;

	m_outStream << "</Protocol>" << zEndl;
	return Close();

#else
	// XML is not enabled for the protocol file: return false so that a "command failed"
	// error message can be written to the logstate file.

	Close();
	return false;

#endif
}
