/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ISimBoxBase.cpp: implementation of the ISimBoxBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "ISimBoxBase.h"

#include "ISimBox.h"
#include "IMonitorCmd.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISimBoxBase::ISimBoxBase(const ISimBox* const pISimBox) : m_pISimBox(pISimBox)
{

}

ISimBoxBase::~ISimBoxBase()
{

}

const zString ISimBoxBase::GetRunId() const
{
	return m_pISimBox->GetRunId();
}

long ISimBoxBase::GetCurrentTime() const
{
	return m_pISimBox->GetCurrentTime();
}

long ISimBoxBase::GetTotalTime() const
{
	return m_pISimBox->GetTotalTime();
}

long ISimBoxBase::GetSimBoxBeadTypeTotal() const
{
	return m_pISimBox->GetBeadTypeTotal();
}

long ISimBoxBase::GetSimBoxBondTypeTotal() const 
{
	return m_pISimBox->GetBondTypeTotal();
}

long ISimBoxBase::GetSimBoxBondPairTypeTotal() const
{
	return m_pISimBox->GetBondPairTypeTotal();
}

const AbstractBeadVector ISimBoxBase::GetSimBoxBeads() const
{
	return m_pISimBox->GetBeads();
}

const BondVector& ISimBoxBase::GetSimBoxBonds() const
{
	return m_pISimBox->GetBonds();
}

const BondPairVector& ISimBoxBase::GetSimBoxBondPairs()	const
{
	return m_pISimBox->GetBondPairs();
}

const CNTCellVector& ISimBoxBase::GetSimBoxCells()	const
{
	return m_pISimBox->GetCNTCells();
}

long ISimBoxBase::GetSimBoxBeadTotal() const
{
	return m_pISimBox->GetBeadTotal();
}

long ISimBoxBase::GetSimBoxBondTotal() const
{
	return m_pISimBox->GetBondTotal();
}

long ISimBoxBase::GetSimBoxBondPairTotal() const
{
	return m_pISimBox->GetBondPairTotal();
}

long ISimBoxBase::GetSimBoxXCellNo() const
{
	return m_pISimBox->GetCNTXCellNo();
}

long ISimBoxBase::GetSimBoxYCellNo() const
{
	return m_pISimBox->GetCNTYCellNo();
}

long ISimBoxBase::GetSimBoxZCellNo() const
{
	return m_pISimBox->GetCNTZCellNo();
}

long ISimBoxBase::GetSimBoxCellTotal() const
{
	return m_pISimBox->GetCNTCellTotal();
}

double ISimBoxBase::GetSimBoxXCellWidth() const
{
	return m_pISimBox->GetCNTXCellWidth();
}

double ISimBoxBase::GetSimBoxYCellWidth() const
{
	return m_pISimBox->GetCNTYCellWidth();
}

double ISimBoxBase::GetSimBoxZCellWidth() const
{
	return m_pISimBox->GetCNTZCellWidth();
}

double	ISimBoxBase::GetSimBoxXLength() const
{
	return m_pISimBox->GetSimBoxXLength();
}

double	ISimBoxBase::GetSimBoxYLength() const
{
	return m_pISimBox->GetSimBoxYLength();
}

double	ISimBoxBase::GetSimBoxZLength() const
{
	return m_pISimBox->GetSimBoxZLength();
}

double	ISimBoxBase::GetSimBoxHalfXLength() const
{
	return m_pISimBox->GetHalfSimBoxXLength();
}

double	ISimBoxBase::GetSimBoxHalfYLength() const
{
	return m_pISimBox->GetHalfSimBoxYLength();
}

double	ISimBoxBase::GetSimBoxHalfZLength() const
{
	return m_pISimBox->GetHalfSimBoxXLength();
}

double	ISimBoxBase::GetSimBoxVolume() const
{
	return m_pISimBox->GetVolume();
}

double ISimBoxBase::GetSimSpaceXLength() const
{
	return m_pISimBox->GetSimSpaceXLength();
}

double ISimBoxBase::GetSimSpaceYLength() const
{
	return m_pISimBox->GetSimSpaceYLength();
}

double ISimBoxBase::GetSimSpaceZLength() const
{
	return m_pISimBox->GetSimSpaceZLength();
}

// **********************************************************************
// Handler functions to forward commands to the Monitor

// Because the SimBox already calls functions called SaveCurrentState and
// SaveRestartState, we add "Cmd" to the end of the two command handler functions. 
// Only when a current or restart state is saved as the result of an explicit 
// command should a message be logged.

void ISimBoxBase::Comment(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->Comment(pCommand);
#endif
}

void ISimBoxBase::GenerateRNGGaussian(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->GenerateRNGGaussian(pCommand);
#endif
}

void ISimBoxBase::GenerateRNGUniform(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->GenerateRNGUniform(pCommand);
#endif
}

void ISimBoxBase::GetBuildDate(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->GetBuildDate(pCommand);
}

void ISimBoxBase::GetBuildId(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->GetBuildId(pCommand);
}

void ISimBoxBase::IssueRunCompleteFraction(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->IssueRunCompleteFraction(pCommand);
}

void ISimBoxBase::PanCurrentStateCamera(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->PanCurrentStateCamera(pCommand);
#endif
}

void ISimBoxBase::RunGud(const xxCommand* const pCommand) const
{
	m_pISimBox->Saveud();
}

void ISimBoxBase::SaveAmiraCurrentState(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveAmiraCurrentState(pCommand);
}

void ISimBoxBase::SaveBead1dDensityProfile(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveBead1dDensityProfile(pCommand);
}

void ISimBoxBase::SaveBeadKineticTemperatures(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveBeadKineticTemperatures(pCommand);
}

void ISimBoxBase::SaveBeadVelocityScalarProduct1dProfile(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveBeadVelocityScalarProduct1dProfile(pCommand);
}

void ISimBoxBase::SaveBeadDensityFluctuations(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveBeadDensityFluctuations(pCommand);
}

void ISimBoxBase::SaveBeadDensityFluctuationsByType(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveBeadDensityFluctuationsByType(pCommand);
}

void ISimBoxBase::SaveCommandHistory(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->SaveCommandHistory(pCommand);
#endif
}

void ISimBoxBase::SaveCommandHistoryToCurrentTime(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->SaveCommandHistoryToCurrentTime(pCommand);
#endif
}

void ISimBoxBase::SaveCurrentStateCmd(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveCurrentStateCmd(pCommand);
}

void ISimBoxBase::SaveNonBeadInclusiveRestartStateCmd(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveNonBeadInclusiveRestartStateCmd(pCommand);
}

void ISimBoxBase::SavePairCorrelationFunction(const xxCommand* const pCommand) const
{
    m_pISimBox->IIMonitorCmd()->SavePairCorrelationFunction(pCommand);
}

void ISimBoxBase::SaveParaviewCurrentState(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveParaviewCurrentState(pCommand);
}

void ISimBoxBase::SavePolymerBeadRDF(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SavePolymerBeadRDF(pCommand);
}

void ISimBoxBase::SavePovrayCurrentState(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SavePovrayCurrentState(pCommand);
}

void ISimBoxBase::SaveProtocolFile(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->SaveProtocolFile(pCommand);
#endif
}

void ISimBoxBase::SaveRestartStateCmd(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SaveRestartStateCmd(pCommand);
}

void ISimBoxBase::SaveSAXS(const xxCommand* const pCommand) const
{
    m_pISimBox->IIMonitorCmd()->SaveSAXS(pCommand);
}

void ISimBoxBase::SetAllBeadsInvisible(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->SetAllBeadsInvisible(pCommand);
#endif
}

void ISimBoxBase::SetAllBeadsVisible(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->SetAllBeadsVisible(pCommand);
#endif
}

void ISimBoxBase::SetAnalysisPeriod(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetAnalysisPeriod(pCommand);
}

void ISimBoxBase::SetBeadDisplayId(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetBeadDisplayId(pCommand);
}

void ISimBoxBase::SetBeadTypeDisplayId(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetBeadTypeDisplayId(pCommand);
}


void ISimBoxBase::SetCurrentStateCamera(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetCurrentStateCamera(pCommand);
}

void ISimBoxBase::SetCurrentStateDefaultFormat(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetCurrentStateDefaultFormat(pCommand);
}

void ISimBoxBase::SetCurrentStateLight(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetCurrentStateLight(pCommand);
}

void ISimBoxBase::SetDensityPeriod(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetDensityPeriod(pCommand);
}

void ISimBoxBase::SetDisplayBeadRange(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetDisplayBeadRange(pCommand);
}

void ISimBoxBase::SetDisplayPeriod(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetDisplayPeriod(pCommand);
}

void ISimBoxBase::SetPolymerDisplayId(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetPolymerDisplayId(pCommand);
}

void ISimBoxBase::SetPolymerTypeDisplayId(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetPolymerTypeDisplayId(pCommand);
}

void ISimBoxBase::SetRestartPeriod(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetRestartPeriod(pCommand);
}

void ISimBoxBase::SetRestartStateDefaultBeadCoordinates(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetRestartStateDefaultBeadCoordinates(pCommand);
}

void ISimBoxBase::SetRestartStateDefaultInclusive(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetRestartStateDefaultInclusive(pCommand);
}

void ISimBoxBase::SetRunCompleteInterval(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetRunCompleteInterval(pCommand);
}

void ISimBoxBase::SetSamplePeriod(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->SetSamplePeriod(pCommand);
}

void ISimBoxBase::SetSAXSProcessBeadElectronNo(const xxCommand* const pCommand) const
{
    m_pISimBox->IIMonitorCmd()->SetSAXSProcessBeadElectronNo(pCommand);
}

void ISimBoxBase::ShowAllProcesses(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ShowAllProcesses(pCommand);
#endif
}

void ISimBoxBase::ShowModifiableProcesses(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ShowModifiableProcesses(pCommand);
#endif
}

void ISimBoxBase::ToggleBeadDisplay(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->ToggleBeadDisplay(pCommand);
}

void ISimBoxBase::ToggleCurrentStateAnalysis(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ToggleCurrentStateAnalysis(pCommand);
#endif
}

void ISimBoxBase::ToggleCurrentStateBox(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->ToggleCurrentStateBox(pCommand);
}

void ISimBoxBase::ToggleDensityFieldOutput(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->ToggleDensityFieldOutput(pCommand);
}

void ISimBoxBase::ToggleEnergyOutput(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ToggleEnergyOutput(pCommand);
#endif
}

void ISimBoxBase::TogglePolymerDisplay(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->TogglePolymerDisplay(pCommand);
}

void ISimBoxBase::ToggleRestartWarningMessages(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->ToggleRestartWarningMessages(pCommand);
}

void ISimBoxBase::ToggleSliceEnergyOutput(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ToggleSliceEnergyOutput(pCommand);
#endif
}

void ISimBoxBase::WriteLogMessage(const xxCommand* const pCommand) const
{
	m_pISimBox->IIMonitorCmd()->WriteLogMessage(pCommand);
}

void ISimBoxBase::ZoomCurrentStateCamera(const xxCommand* const pCommand) const
{
#if EnableMonitorCommand == SimCommandEnabled
	m_pISimBox->IIMonitorCmd()->ZoomCurrentStateCamera(pCommand);
#endif
}

// **********************************************************************
// Non-command forwarding functions to the Monitor

void ISimBoxBase::Sample() const
{
	m_pISimBox->Sample();
}

void ISimBoxBase::SampleProcess() const
{
	m_pISimBox->SampleProcess();
}

void ISimBoxBase::SaveCurrentState() const
{
	m_pISimBox->SaveCurrentState();
}

void ISimBoxBase::SaveProcessState() const
{
	m_pISimBox->SaveProcessState();
}

void ISimBoxBase::SaveRestartState() const
{
	m_pISimBox->SaveRestartState();
}

void ISimBoxBase::ZeroSliceStress() const
{
	m_pISimBox->ZeroSliceStress();
}

void ISimBoxBase::AddBondStress(const CBond* const pBond) const
{
	m_pISimBox->AddBondStress(pBond);
}

void ISimBoxBase::AddBondPairStress(const CBondPair* const pBondPair) const
{
	m_pISimBox->AddBondPairStress(pBondPair);
}

void ISimBoxBase::ZeroTotalEnergy() const
{
	m_pISimBox->ZeroTotalEnergy();
}

void ISimBoxBase::UpdateBeadTypes() const
{
	m_pISimBox->UpdateBeadTypes();
}

// Function to tell the Monitor to update its bond types
// after a command that may have changed the number of bonds or their types.
// It is called by the derived CSimBox class' function of the same name.

void ISimBoxBase::UpdateBondTypes() const
{
	m_pISimBox->UpdateBondTypes();
}




