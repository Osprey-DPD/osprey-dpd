/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeActiveSimBox.cpp: implementation of the aeActiveSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimFunctionalFlags.h"
#include "ISimBox.h"
#include "aeActiveSimBox.h"
#include "aevActiveEvent.h"

// Active network classes

#include "aeArp23Network.h"
#include "aefActinNetwork.h"
#include "aeForminNetwork.h"
#include "aeReceptorNetwork.h"

// ACN processes for output of analsis data

#include "prfActin.h"
#include "prFormin.h"
#include "prReceptor.h"

// Active Event Interface wrapper classes

#include "IActivePolymerForms.h"
#include "IActivePolymerFormsBondTriple.h"
#include "IActivePolymerDissolves.h"
#include "IActiveBondBindsToPolymer.h"
#include "IActiveBondUnbindsFromPolymer.h"
#include "IActiveBondHydrolysesATP.h"
#include "IActiveBondReleasesPi.h"
#include "IActiveBondPhosphorylation.h"

// **********************************************************************
// Global Functions.
//
// Static member variable holding a pointer to the single instance of aeActiveSimBox.
// Note that it does not get assigned a value until the user creates the instance.

aeActiveSimBox* aeActiveSimBox::m_pInstance = 0;

// Public member function to create a single instance of the CSimBox class.

aeActiveSimBox* aeActiveSimBox::Instance(const ISimBox* const pISimBox)
{
	if(!m_pInstance)
		m_pInstance = new aeActiveSimBox(pISimBox);

	return m_pInstance;
}

// Static member function to allow active classes a global point of access to 
// the active SimBox.

const IActiveSimBox* aeActiveSimBox::GetIActiveSimBox()
{
	return m_pInstance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeActiveSimBox::aeActiveSimBox(const ISimBox* const pISimBox) : m_pISimBox(pISimBox)
{
	// Ensure maps are empty

	m_mACN.clear();
	m_mModifiableACN.clear();
	m_mAccessACN.clear();
}

aeActiveSimBox::~aeActiveSimBox()
{
	// Delete all the enclosed active networks if any exist.

	for(StringActiveACNIterator iterACN=m_mACN.begin(); iterACN!=m_mACN.end(); iterACN++)
	{
		m_mModifiableACN.erase((*iterACN).second->GetACNType());
		m_mAccessACN.erase((*iterACN).second->GetACNType());

		delete (*iterACN).second;
	}

	m_mACN.clear();

	// Set the single aeActiveSimBox pointer to 0 to ensure that multiple simulations
	// use different objects, and zero the active event counter so that successive runs
    // label events starting at 1.

	m_pInstance = 0;

    aevActiveEvent::ZeroEventTotal();
}

// Functions that create and store instances of aeActiveCellNetwork-derived classes
// that keep track of specific active bonds and polymers.

bool aeActiveSimBox::IsArp23NetworkPresent() const
{
	return IsACNPresent(aeArp23Network::GetType());
}

bool aeActiveSimBox::IsfActinNetworkPresent() const
{
	return IsACNPresent(aefActinNetwork::GetType());
}

bool aeActiveSimBox::IsForminNetworkPresent() const
{
	return IsACNPresent(aeForminNetwork::GetType());
}

bool aeActiveSimBox::IsReceptorNetworkPresent() const
{
	return IsACNPresent(aeReceptorNetwork::GetType());
}

// Function to return a pointer to the enclosed Arp23 network instance.

aeArp23Network* aeActiveSimBox::GetArp23Network() const
{
	return dynamic_cast<aeArp23Network*>(GetACNFromType(aeArp23Network::GetType()));
}

// Function to return a pointer to the enclosed fActin network instance.

aefActinNetwork* aeActiveSimBox::GetfActinNetwork() const
{
	return dynamic_cast<aefActinNetwork*>(GetACNFromType(aefActinNetwork::GetType()));
}

// Function to return a pointer to the enclosed Formin network instance.

aeForminNetwork* aeActiveSimBox::GetForminNetwork() const
{
	return dynamic_cast<aeForminNetwork*>(GetACNFromType(aeForminNetwork::GetType()));
}

// Function to return a pointer to the enclosed Receptor network instance.

aeReceptorNetwork* aeActiveSimBox::GetReceptorNetwork() const
{
	return dynamic_cast<aeReceptorNetwork*>(GetACNFromType(aeReceptorNetwork::GetType()));
}

// Network to track the Arp2/3 protein monomers that regulate the formation
// of new fActin branches and nucleate actin filaments.

void aeActiveSimBox::CreateArp23Network(long xn, long yn, long zn)
{
	CreateACNFromType(aeArp23Network::GetType(), xn, yn, zn);
}

// Function to create an active network to track actin monomers and filaments.
// Note that the arguments specify the number of CNT cells per active cell, 
// not the number of active cells. We have already checked that the number 
// of CNT cells per active cell is a whole number so there is no rounding 
// to be done.

void aeActiveSimBox::CreatefActinNetwork(long xn, long yn, long zn)
{
	CreateACNFromType(aefActinNetwork::GetType(), xn, yn, zn);
}

// Function to create an active network to model formin molecules that
// assemble fActin filaments out of mActin monomders.
//
// Note that the arguments specify the number of CNT cells per active cell, 
// not the number of active cells. We have already checked that the number 
// of CNT cells per active cell is a whole number so there is no rounding 
// to be done.

void aeActiveSimBox::CreateForminNetwork(long xn, long yn, long zn)
{
	CreateACNFromType(aeForminNetwork::GetType(), xn, yn, zn);
}

// Function to create an active network to model receptor-ligand binding.
// Only a single ligand is allowed to bind to this type of receptor.
//
// Note that the arguments specify the number of CNT cells per active cell, 
// not the number of active cells. We have already checked that the number 
// of CNT cells per active cell is a whole number so there is no rounding 
// to be done.

void aeActiveSimBox::CreateReceptorNetwork(long xn, long yn, long zn)
{
	CreateACNFromType(aeReceptorNetwork::GetType(), xn, yn, zn);
}

// Main routine to evolve the state of all the active networks.
//
// It checks to see which networks have been instantiated and calls their Evolve() 
// functions to update the state of their active bonds and polymers.

void aeActiveSimBox::Evolve()
{
	for(StringActiveACNIterator iterACN=m_mACN.begin(); iterACN!=m_mACN.end(); iterACN++)
	{
		(*iterACN).second->Evolve();
	}
}

// Function to return a pointer to the IModifyActiveCellNetwork interface of
// an ACN given its unique string id as a key. If the ACN is not found,
// we return a null pointer that must be checked for in the calling routine.

IModifyActiveCellNetwork* aeActiveSimBox::GetIModifyActiveCellNetworkFromType(const zString type)
{
	IModifyActiveCellNetwork* pTargetACN = 0;

	cStringModifiableACNIterator citerACN2 = m_mModifiableACN.find(type);

	if(citerACN2 != m_mModifiableACN.end())
	{
		pTargetACN = citerACN2->second;
	}

	return pTargetACN;
}

// Function to return a pointer to the IACNAccessControl interface of
// an ACN given its unique string id as a key. If the ACN is not found,
// we return a null pointer that must be checked for in the calling routine.


IACNAccessControl* aeActiveSimBox::GetIACNAccessControlFromType(const zString type)
{
	IACNAccessControl* pTargetACN = 0;

	cStringAccessibleACNIterator citerACN2 = m_mAccessACN.find(type);

	if(citerACN2 != m_mAccessACN.end())
	{
		pTargetACN = citerACN2->second;
	}

	return pTargetACN;
}

// Protected functions to allow the ACNs access to the original SimBox data

const zString aeActiveSimBox::GetRunId() const
{
	return m_pISimBox->GetRunId();
}

long aeActiveSimBox::GetCurrentTime() const
{
	return m_pISimBox->GetCurrentTime();
}

double aeActiveSimBox::GetSimBoxXLength() const
{
	return m_pISimBox->GetSimBoxXLength();
}

double aeActiveSimBox::GetSimBoxYLength() const
{
	return m_pISimBox->GetSimBoxYLength();
}

double aeActiveSimBox::GetSimBoxZLength() const
{
	return m_pISimBox->GetSimBoxZLength();
}

double aeActiveSimBox::GetHalfSimBoxXLength() const
{
	return m_pISimBox->GetHalfSimBoxXLength();
}

double aeActiveSimBox::GetHalfSimBoxYLength() const
{
	return m_pISimBox->GetHalfSimBoxYLength();
}

double aeActiveSimBox::GetHalfSimBoxZLength() const
{
	return m_pISimBox->GetHalfSimBoxZLength();
}

// ****************************************
// Active Event Interfaces that are used by ACN-level events to gain access
// to the data they need to execute. The newly-created interface instances
// are owned, and destroyed, by the calling instances.

// IActivePolymerForms
// *******************
//
// Generic active polymer formation event that takes two free active bonds
// and assembles them into a new active polymer.

IActivePolymerForms* aeActiveSimBox::GetIActivePolymerForms()
{
	return new IActivePolymerForms(this);
}

// IActivePolymerFormsBondTriple
// *****************************
//
// Generic active polymer formation event that takes three free active bonds
// and assembles them into a new active polymer.

IActivePolymerFormsBondTriple* aeActiveSimBox::GetIActivePolymerFormsBondTriple()
{
	return new IActivePolymerFormsBondTriple(this);
}

// IActivePolymerDissolves
// ***********************
//
// Generic active polymer dissolution event that takes an existing polymer
// and dissolves it into its free active bond instances. 

IActivePolymerDissolves* aeActiveSimBox::GetIActivePolymerDissolves()
{
	return new IActivePolymerDissolves(this);
}

// IActiveBondBindsToPolymer
// *************************
//
// Generic event for adding bonds to either end of an active polymer.

IActiveBondBindsToPolymer* aeActiveSimBox::GetIActiveBondBindsToPolymer()
{
	return new IActiveBondBindsToPolymer(this);
}

// IActiveBondUnbindsFromPolymer
// *****************************
//
// Generic event for detaching bonds from either end of an active polymer.

IActiveBondUnbindsFromPolymer* aeActiveSimBox::GetIActiveBondUnbindsFromPolymer()
{
	return new IActiveBondUnbindsFromPolymer(this);
}

// IActiveBondHydrolysesATP
// ************************
//
// Event to model the transition ATP --> ADP-Pi for an active bond
// that represents a type of monomer that can bind ATP in various states.

IActiveBondHydrolysesATP* aeActiveSimBox::GetIActiveBondHydrolysesATP()
{
	return new IActiveBondHydrolysesATP(this);
}

// IActiveBondReleasesPi
// *********************
//
// Event to model the transition ADP-Pi --> ADP + Pi for an active bond
// that represents a type of monomer that can bind ATP in various states.

IActiveBondReleasesPi* aeActiveSimBox::GetIActiveBondReleasesPi()
{
	return new IActiveBondReleasesPi(this);
}

// IActiveBondPhosphorylation
// **************************
//
// Event to model the transition ADP-Pi --> ADP + Pi for an active bond
// that represents a type of monomer that can bind ATP in various states.

IActiveBondPhosphorylation* aeActiveSimBox::GetIActiveBondPhosphorylation()
{
	return new IActiveBondPhosphorylation(this);
}

// ****************************************
// Functions to handle the creation, deletion and access to all ACN's using
// their string identifiers (or label) to identify them.

// Function to indicate if an ACN specified by its label has been instantiated.

bool aeActiveSimBox::IsACNPresent(const zString acnType) const
{
	cStringActiveACNIterator citerACN = m_mACN.find(acnType);

	if(citerACN != m_mACN.end())
		return true;

	return false;
}

// Function to return a base aeActiveCellNetwork pointer to an ACN that is
// specified by its string identifier. If the ACN does not exist, we
// return a null pointer.

aeActiveCellNetwork* aeActiveSimBox::GetACNFromType(const zString acnType) const
{
	if(IsACNPresent(acnType))
	{
		cStringActiveACNIterator citerACN = m_mACN.find(acnType);
		return (*citerACN).second;
	}
		
	return 0;
}

// Function to instantiate an ACN given its string identifier. If the named
// ACN already exists, we ignore the call.

void aeActiveSimBox::CreateACNFromType(const zString acnType, long xn, long yn, long zn)
{
	if(!IsACNPresent(acnType))
	{
		// Get the number of CNT cells per dimension

		const long cntXNo = m_pISimBox->GetCNTXCellNo();
		const long cntYNo = m_pISimBox->GetCNTYCellNo();
		const long cntZNo = m_pISimBox->GetCNTZCellNo();

		// Calculate the (integer) number of active cells per dimension and their widths

		long xNo = cntXNo/xn;
		long yNo = cntYNo/yn;
		long zNo = cntZNo/zn;

		double xWidth = static_cast<double>(xn)*m_pISimBox->GetCNTXCellWidth();
		double yWidth = static_cast<double>(yn)*m_pISimBox->GetCNTYCellWidth();
		double zWidth = static_cast<double>(zn)*m_pISimBox->GetCNTZCellWidth();

		aeActiveCellNetwork* pACN = acfACNFactory::Instance()->Create(acnType, this, xNo, yNo, zNo, xWidth, yWidth, zWidth);

		// Add the newly-created network to the sSimBox's maps

		m_mACN.insert(zPairSACN(acnType, pACN));

		IModifyActiveCellNetwork* const pIMACN = dynamic_cast<IModifyActiveCellNetwork*>(pACN);
		m_mModifiableACN.insert(zPairSMACN(pIMACN->GetACNType(), pIMACN));

		IACNAccessControl* const pIAACN = pACN->GetIACNAccessControl();
		m_mAccessACN.insert(zPairSAACN(pIMACN->GetACNType(), pIAACN));

		// Create the process that will serialise the ACN's data, and set its
		// start and end times to the current time and total time. We call its 
		// ValidateData() function to ensure that the associated xxProcessState 
		// gets created,  pass it to the ISimBox for ownership, and finally
		// attach it to the ACN. 
		
		xxProcess* pProcess = acfProcessFactory::Instance()->Create(pACN->GetACNType());

		pProcess->SetStartTime(m_pISimBox->GetCurrentTime());
		pProcess->SetEndTime(m_pISimBox->GetTotalTime());

		pProcess->InternalValidateData(m_pISimBox->IISimState());
		m_pISimBox->AddProcess(pProcess);
		pACN->AttachProcess(pProcess);
	}
}

// Function to delete a named ACN and remove it from the active ACN map.
// First we remove the interface classes that wrap it from their maps,
// and then call its destructor. If the label does not correspond to an active ACN, 
// we ignore it.

void aeActiveSimBox::DeleteACNFromType(const zString acnType)
{
	if(IsACNPresent(acnType))
	{
		aeActiveCellNetwork* pACN = GetACNFromType(acnType);

		m_mACN.erase(acnType);
		m_mModifiableACN.erase(acnType);
		m_mAccessACN.erase(acnType);

		delete pACN;
	}
}

