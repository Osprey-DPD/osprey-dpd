/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ISimBox.cpp: implementation of the ISimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMiscellaneousFlags.h"
#include "ISimBox.h"
#include "mpsSimBox.h"
#include "Monitor.h"
#include "CNTCell.h"
#include "pmSimBoxNeighbourCall.h"
#include "pmSimBoxAssembly.h"

#if EnableShadowSimBox == SimACNEnabled
  #include "aeActiveSimBox.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Static member variable holding a pointer to the single instance of ISimBox.
// Note that it does not get assigned a value until the user creates the instance.

ISimBox* ISimBox::m_pInstance = 0;

// Public member function to create a single instance of the ISimBox class.

ISimBox* ISimBox::Instance(CSimState& simState)
{
	if( !m_pInstance)
	{
		m_pInstance = new ISimBox(simState);
	}

	return m_pInstance;
}

// Constructor for the ISimBox interface class creates the actual SimBox and Monitor
// objects used in the simulation and passes a pointer to itself so that they can 
// communicate during the simulation by sending messages via the ISimBox object.
// 
// Interfaces to the SimBox are also created here and passed to the SimBox constructor
// for storage. This way we make them available via the ISimBox to any objects that
// need them without their having to access the SimBox itself.

ISimBox::ISimBox(CSimState& simState) : IGlobalSimBox(this), m_rSimState(simState)
{
	m_pSimBox = CSimBox::Instance(simState, this);

	m_pMonitor = CMonitor::Instance(&simState, this);

	// Store pointers to the Monitor and ISimBox in the CCNTCell class to allow on-the-fly analysis of 
	// observables stored in the CNT cells, such as the stress tensor in curvilinear coordinates.
	// We use static members because we only want one pointer accessible by all CNT cells.

	CCNTCell::SetMonitorPointer(m_pMonitor);

	CCNTCell::SetISimBoxPointer(this);

    // Now connect up the processors via their mpsSimBox instances.
    // Note that we have access to the SimMPSFlags file and all parallel
    // data via the xxParallelBase base class. Each processor has to store
    // its own location in the set of processors and associate these coordinates
    // with its rank.

#if EnableParallelSimBox == SimMPSEnabled

// Make sure all the processors have instantiated their SimBoxes before starting the 
// simulation time loop. P0 receives a message from each PN containing a list of its 
// nearest neighbour procIds; this allows us to check that all processors are correctly 
// connected before we allow P0 to proceed.  Note that we also set the NN data for
// P0 but do not use a message to retrieve it as this is unnecessary.

    mpsSimBox* const pParallel = m_pSimBox->GetParallelSimBox();

    pmSimBoxNeighbourCall* pNNListMsg = dynamic_cast<pmSimBoxNeighbourCall*>(acfParallelMessageFactory::Instance()->Create(pmSimBoxNeighbourCall::GetType()));
	pNNListMsg->SetSenderId(GetRank());
	
	for(long i=0; i<27; i++)
	{
		pNNListMsg->SetNNProcId(i, pParallel->GetNeighbourPidFromSlot(i));
	}

    if(!IsProcessZero())
    {
		// We use a message that is sent from each PN to P0 and that contains a list of 
        // its nearest neighbour pids; note that there are 27 of these as we include PN itself.
	
		if(pNNListMsg->Validate())
		{
			pNNListMsg->SendP0();
		}
		else
		{
           // Handle error condition here
           std::cout << "pmSimBoxNeighbourCall assembly failed on processor " << GetRank() << zEndl;
//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		}	
    }
    else
    {
        // Here P0 receives the data from PN and uses it to set up our copy
        // of the simulation state. The rank of the sending process is held in 
        // the base class' m_SenderRank member variable.

        pNNListMsg->Receive();
    }

	delete pNNListMsg;

#elif EnableParallelSimBox == SimMPSDisabled
    // Nothing to do here
#endif
}

// Delete the SimBox and Monitor objects that were created by this interface class,
// and the static Instance pointer to the ISimBox.

ISimBox::~ISimBox()
{
	m_pInstance = 0;

	if(m_pSimBox)
	{
		delete m_pSimBox;
		m_pSimBox = 0;
	}

	if(m_pMonitor)
	{
		delete m_pMonitor;
		m_pMonitor = 0;
	}
}

// Functions to return a pointer to the SimBox's ISimCmd, ISimEvent, ISimState interfaces. 
// Note that the pointer cannot be changed to point to another object but the
// SimBox can be altered through it. However, this gave errors on the unix
// compiler so I removed the *const declaration and replaced it with const*.
// This means that the object cannot be altered via this pointer. But as this
// is an abstract base class, does this mean that the derived class cannot
// be changed? This would not work because I want to use the ISimCmd as the
// interface for commands that alter the SimBox state. Hence, I removed the
// const completely.

ISimCmd* ISimBox::IISimCmd() const
{
	return dynamic_cast<ISimCmd*>(m_pSimBox);
}

ISimEvent* ISimBox::IISimEvent() const
{
	return dynamic_cast<ISimEvent*>(m_pSimBox);
}

ISimState* ISimBox::IISimState() const
{
	return dynamic_cast<ISimState*>(m_pSimBox);
}

#if EnableShadowSimBox == SimACNEnabled
// Function to return a pointer to the active SimBox's IActiveSimBox interface

IActiveSimBox* ISimBox::IIActiveSimBox() const
{
	return dynamic_cast<IActiveSimBox*>(m_pSimBox->GetActiveSimBox());
}
#endif

// Function to return a pointer to the CMonitor IMonitorCmd interface. 

IMonitorCmd* ISimBox::IIMonitorCmd() const
{
	return dynamic_cast<IMonitorCmd*>(m_pMonitor);
}

// Forwarding functions to the CSimState object

const BeadVector& ISimBox::GetBeadTypes() const
{
	return m_rSimState.GetBeadTypes();
}

const BondVector& ISimBox::GetBondTypes() const
{
	return m_rSimState.GetBondTypes();
}

const BondPairVector& ISimBox::GetBondPairTypes() const
{
	return m_rSimState.GetBondPairTypes();
}

const PolymerVector& ISimBox::GetPolymerTypes() const
{
	return m_rSimState.GetPolymerTypes();
}

const zString ISimBox::GetBeadNameFromType(long type) const
{
	return m_rSimState.cGetAnalysisState().GetBeadNameFromType(type);
}

const zString ISimBox::GetBondNameFromType(long type) const
{
	return m_rSimState.cGetAnalysisState().GetBondNameFromType(type);
}

const zString ISimBox::GetBondHeadNameFromType(long type) const
{
	const long headType = m_rSimState.cGetAnalysisState().GetBondHeadType(type);

	return GetBeadNameFromType(headType);
}

const zString ISimBox::GetBondTailNameFromType(long type) const
{
	const long tailType = m_rSimState.cGetAnalysisState().GetBondTailType(type);

	return GetBeadNameFromType(tailType);
}

const zString ISimBox::GetBondPairNameFromType(long type) const
{
	return m_rSimState.cGetAnalysisState().GetBondPairNameFromType(type);
}

const zString ISimBox::GetBondPairHeadNameFromType(long type) const
{
	const long headType = m_rSimState.cGetAnalysisState().GetBondPairHeadType(type);

	return GetBeadNameFromType(headType);
}

const zString ISimBox::GetBondPairMiddleNameFromType(long type) const
{
	const long middleType = m_rSimState.cGetAnalysisState().GetBondPairMiddleType(type);

	return GetBeadNameFromType(middleType);
}

const zString ISimBox::GetBondPairTailNameFromType(long type) const
{
	const long tailType = m_rSimState.cGetAnalysisState().GetBondPairTailType(type);

	return GetBeadNameFromType(tailType);
}

const zString ISimBox::GetPolymerNameFromType(long type) const
{
	return m_rSimState.cGetAnalysisState().GetPolymerNameFromType(type);
}

long ISimBox::GetBeadTypeFromName(const zString name) const
{
	return m_rSimState.cGetAnalysisState().GetBeadTypeFromName(name);
}

long ISimBox::GetBeadTotalForType(long type) const
{
	return m_rSimState.cGetInitialState().GetBeadTotalForType(type);
}

long ISimBox::GetBondTotalForType(long type) const
{
	return m_rSimState.cGetInitialState().GetBondTotalForType(type);
}

long ISimBox::GetBondPairTotalForType(long type) const
{
	return m_rSimState.cGetInitialState().GetBondPairTotalForType(type);
}

long ISimBox::GetPolymerTotalForType(long type) const
{
	return m_rSimState.cGetInitialState().GetPolymerTotalForType(type);
}

long ISimBox::GetPolymerSizeFromType(long type) const
{
	return m_rSimState.cGetInitialState().GetPolymerSizeForType(type);
}

double ISimBox::GetPolymerFractionFromType(long type) const
{
	return m_rSimState.cGetInitialState().GetPolymerFractionForType(type);
}

// Function to obtain the zString representation of a polymer's shape
// using its type identifier as a key.

const zString ISimBox::GetPolymerShapeFromType(long polyType) const
{
	return m_rSimState.cGetAnalysisState().GetPolymerShapeFromType(polyType);
}

// Function to obtain the zString representation with terminal spaces
// of a polymer's shape using its type identifier as a key.

const zString ISimBox::GetSpaceWrappedPolymerShapeFromType(long polyType) const
{
	return m_rSimState.cGetAnalysisState().GetSpaceWrappedPolymerShapeFromType(polyType);
}

// Function to return a pointer to the IModifyProcess interface of the
// named process.


IModifyProcess* ISimBox::GetModifiableProcessFromName(const zString name) const
{
	return m_rSimState.cGetAnalysisState().GetModifiableProcessFromName(name);
}

// Function to return a pointer to the IModifyProcess interface of an xxProcess
// if the process whose id is specified implements it.

IModifyProcess* ISimBox::GetModifiableProcessFromPid(const zString pid) const
{
	return m_rSimState.cGetAnalysisState().GetModifiableProcessFromPid(pid);
}

double ISimBox::GetDPDBeadConsInt(long first, long second) const
{
	return m_rSimState.GetDPDBeadConsInt(first, second);
}

double ISimBox::GetDPDBeadDissInt(long first, long second) const
{
	return m_rSimState.GetDPDBeadDissInt(first, second);
}

// Forwarding functions to the CMonitor object

void ISimBox::Sample()	const 
{
    if(IsParallel())
    {
	    m_pMonitor->SampleP();
    }
    else
    {
	    m_pMonitor->Sample();
    }
}

void ISimBox::SampleProcess() const
{
	m_pMonitor->SampleProcess();
}

void ISimBox::SaveCurrentState() const 
{
    if(IsParallel())
    {
	    m_pMonitor->SaveCurrentStateP();
    }
    else
    {
	    m_pMonitor->SaveCurrentState();
    }
}

void ISimBox::SavePrevisCurrentState() const
{
	m_pMonitor->SavePrevisCurrentState();
}

void ISimBox::SaveProcessState() const
{
	m_pMonitor->SaveProcessState();
}

void ISimBox::SaveRestartState() const 
{
	m_pMonitor->SaveRestartState();
}

long ISimBox::GetGridXCellNo() const
{
	return m_pMonitor->GetGridXCellNo();
}

long ISimBox::GetGridYCellNo() const
{
	return m_pMonitor->GetGridYCellNo();
}

long ISimBox::GetGridZCellNo() const
{
	return m_pMonitor->GetGridZCellNo();
}

double ISimBox::GetGridXCellWidth() const
{
	return m_pMonitor->GetGridXCellWidth();
}

double ISimBox::GetGridYCellWidth() const
{
	return m_pMonitor->GetGridYCellWidth();
}

double ISimBox::GetGridZCellWidth() const
{
	return m_pMonitor->GetGridZCellWidth();
}

// Forwarding function to the CMonitor to zero the arrays holding the
// bead, bond and bondpair contributions to the total stress tensor
// averaged over slices through the SimBox.

void ISimBox::ZeroSliceStress() const
{
	m_pMonitor->ZeroSliceStress();
}

// Forwarding function to add the contribution from a pair of interacting CBead objects to
// the stress tensor calculations. 
//
// The Monitor calculates the stress in planar slices across the SimBox, while
// the SimBox itself calculates the stress in each grid cell for later use
// in calculating the stress in curvilinear coordinate systems.
// The latter calculation is very expensive, so we conditionally compile it in.

void ISimBox::AddBeadStress(double rp[3], double rq[3], double stress[9]) const
{
#if EnableStressTensorSphere == SimMiscEnabled
	m_pSimBox->AddWeightedStressToLinearCells(rp, rq, stress);
#endif	
}

// Forwarding function to add the contribution from a CBond object to
// the stress tensor calculations. This cannot be called from the CBond class
// because it does not have access to the Monitor so we call it from the
// SimBox in the AddBondForces() function.
//
// The Monitor calculates the stress in planar slices across the SimBox, while
// The SimBox itself calculates the stress in each CNT cell for later use
// in calculating the stress in curvilinear coordinate systems.
// The latter calculation is very expensive, so we conditionally compile it in.

void ISimBox::AddBondStress(const CBond *const pBond) const
{
	m_pMonitor->AddBondStress(pBond);
	
#if EnableStressTensorSphere == SimMiscEnabled
	m_pSimBox->AddBondStressSphere(pBond);
#endif	
}

// Forwarding function to add the contribution from a CBondPair object to
// the stress tensor similarly to the CBond contribution. 

void ISimBox::AddBondPairStress(const CBondPair *const pBondPair) const
{
	m_pMonitor->AddBondPairStress(pBondPair);
	
#if EnableStressTensorSphere == SimMiscEnabled
	m_pSimBox->AddBondPairStressSphere(pBondPair);
#endif
}

// Forwarding function to the CSimBox to set the origin used in transforming the
// stres tensor from Cartesian coordinates to spherical polar coordinates.
// It is used, for example, by the CVesicle analysis instance.

void ISimBox::SetStressSphereOrigin(double rcm[3]) const
{
#if EnableStressTensorSphere == SimMiscEnabled
	m_pSimBox->SetStressSphereOrigin(rcm);
#endif
}

// Forwarding function to the to CSimBox to transform the stress tensor from
// Cartesian coordinates to spherical polar coordinates for all CNT cells.
// The total stress tensor for the simulation box is returned in the argument.

void ISimBox::TransformStressSphere(double stress[9]) const
{
#if EnableStressTensorSphere == SimMiscEnabled
	m_pSimBox->TransformStressSphere(stress);
#endif
}

void ISimBox::ZeroTotalEnergy() const
{
	m_pMonitor->ZeroTotalEnergy();
}

// Forwarding functions to tell the CMonitor to update its local counters
// of the number of bead types and bond types.

void ISimBox::UpdateBeadTypes() const
{
	m_pMonitor->UpdateBeadTypes();
}

void ISimBox::UpdateBondTypes() const
{
	m_pMonitor->UpdateBondTypes();
}

// Handler function to turn on and off the display of a single polymer type
// in current state snapshots. This provides an alternative to executing the
// mcTogglePolymerDisplay command, and does not issue log messages. It is provided
// to allow internal toggling of polymers in the snapshots.

void ISimBox::InternalTogglePolymerDisplay(const zString polymerName) const
{
    m_pMonitor->InternalTogglePolymerDisplay(polymerName);
}


void ISimBox::Saveud() const
{
    m_pMonitor->Saveud();
}

// Function to tell the CSimulation whether the parallel SimBox assembly on
// any processors failed. Note that a return value of true means assembly failed.
// We get all processors to send a message to P0 indicating whether their
// parallel SimBox was constructed ok.

bool ISimBox::GetParallelAssemblyFailed() const
{
    bool bFailed = false;

#if EnableParallelSimBox == SimMPSEnabled

    pmSimBoxAssembly* pMsg = dynamic_cast<pmSimBoxAssembly*>(acfParallelMessageFactory::Instance()->Create(pmSimBoxAssembly::GetType()));

    if(!IsProcessZero())
    {
	    pMsg->SetSenderRank(GetRank());	
        pMsg->SetAssemblyFailed(m_pSimBox->GetParallelSimBox()->GetAssemblyFailed());
	
		if(pMsg->Validate())
		{
			pMsg->SendP0();
		}
		else
		{
           // Handle error condition here
           std::cout << "pmSimBoxAssembly message failed on processor " << GetRank() << zEndl;
//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		}	
    }
    else
    {
        // Here P0 receives the failed flag from PN and copies it into a local variable
        // if the assembly failed.

        pMsg->Receive();

        if(pMsg->GetAssemblyFailed())
        {
            bFailed = true;
        }

        // Finally check the assembly on P0

        if(m_pSimBox->GetParallelSimBox()->GetAssemblyFailed())
        {
            bFailed = true;
        }
    }

	delete pMsg;

#endif

    return bFailed;
}

// Function to return a container of all the beads in the SimBox. We build this up
// from the CNT cells.

AbstractBeadVector ISimBox::GetBeads() const
{
	return m_pSimBox->GetAllBeadsInCNTCells();
}

// Function to return the number of beads currently in the SimBox.

long ISimBox::GetBeadTotal() const
{
	return m_pSimBox->GetAllBeadsInCNTCells().size();
}

