/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aefActinBond.cpp: implementation of the aefActinBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "aefActinBond.h"
#include "aeBondInline.h"
#include "aeBondTriangle.h"
#include "aeBondHeptamer.h"
#include "aeBondOffRate.h"
#include "aeBondProximityOn.h"
#include "aeBondProximityOff.h"
#include "CNTCell.h"  // Needed for the RNG
#include "IGlobalSimBox.h"
#include "LogTextErrorMessage.h"


//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

// Total number of active bonds that can be connected to form an active polymer. 
// This is stored in a private static member for each class derived from
// aeActiveBond. This allows different types of active bond to represent, 
// for example, a receptor-ligand pair (only one bond per polymer) and 
// filamentous actin (arbitrary number of bonds per polymer).

long aefActinBond::m_ActiveBondsPerPolymer = 1000;

// Parameters setting the probabilities of an actin monomer's bound ATP
// making a transition between its allowed states. These values apply
// to all monomers. Their initial values are zero, so that the monomers'
// internal state does not influence their behaviour. They must be
// set by command.
//
double aefActinBond::m_ATPHydrolysisProb       = 0.0;  // Probability of ATP hydrolysis
double aefActinBond::m_ADPReleasePiProb        = 0.0;  // Probability of releasing Pi
double aefActinBond::m_ADPPhosphorylationProb  = 0.0;  // Probability of phosphorylation


double aefActinBond::m_HeadBasalOffRate    = 0.0;      // Probability for ATP monomer to detach from filament head
double aefActinBond::m_TailBasalOffRate    = 0.0;      // Probability for ATP monomer to detach from filament tail
double aefActinBond::m_HeadADPPiMultiplier = 0.0;      // Multiplier applied to basal rate when ATP is hydrolysed at head
double aefActinBond::m_TailADPPiMultiplier = 0.0;      // Multiplier applied to basal rate when ATP is hydrolysed at tail
double aefActinBond::m_HeadADPMultiplier   = 0.0;      // Multiplier applied to basal rate when Pi is released at head
double aefActinBond::m_TailADPMultiplier   = 0.0;      // Multiplier applied to basal rate when Pi is released at tail


// Function to set the probability of the transitions ATP --> ADP-Pi

void aefActinBond::SetATPHydrolysisProbability(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_ATPHydrolysisProb = prob;
    }
}

// Function to set the probability of the transition ADP-Pi --> ADP 

void aefActinBond::SetADPReleasePiProbability(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_ADPReleasePiProb = prob;
    }
}

// Function to set the probability of the transition ADP --> ATP

void aefActinBond::SetADPPhosphorylationProbability(double prob)
{
    if(prob >= 0.0 && prob <= 1.0)
    {
        m_ADPPhosphorylationProb = prob;
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to wrap a CPolymer that represents an actin monomer in an
// active bond. This allows the monomers to be strung together into 
// active polymers that can grow and shrink to represent actin filaments.
//
// The force law for the actin filaments is a simple Hookean spring model
// for the bonds between adjacent monomers to set the filament's contour length, 
// and a bending stiffness to set the filament's rigidity. We use the aeBondTriangle
// connector class and define the same Hookean springs for the internal bonds and
// linker bonds, and set the bending stiffness from the user-supplied data.
//
// Note that the preferred bond angle is in radians.
// Note that the fixed off-rate is set in the body of the constructor.

aefActinBond::aefActinBond(CPolymer* const pTail, double k2, double l0, 
						   double k3, double phi0) : aeActiveBond(pTail, 
						   new aeBondHeptamer(0, pTail, 0, k2, l0, k2, l0, k3, phi0),
						   new aeBondProximityOn(2.0, 2.0),
						   new aeBondOffRate(0.0, 0.0)),
						   m_bATP(true), m_bPi(false),
                           m_HeadCurrentOffRate(0.0),
                           m_TailCurrentOffRate(0.0)
{
	// The internal state of the actin monomers depends on ATP. There are three
	// possible states and their associated transitions:
	//
	// 1) Bound ATP     - default state for new monomers 
	// 2) Bound ADP-Pi  - transition caused by calling Hydrolyse() function
	// 3) Bound ADP     - transition caused by calling RemovePi() function
	// 4) Bound ATP     - transition caused by calling Phosphorylate() function
 
    // Set the basal off-rate for the fixed probability detachment events.
    // Note that the first element is the head of the bond, and the second
    // is the tail. These are stored in static member variables.

    // Pass basal values to the fixed off-rate instances and set the 
    // current rates at the same time

    SetFixedHeadOffRate(m_HeadBasalOffRate);
    SetFixedTailOffRate(m_TailBasalOffRate);
    
	// The on-rate for creating a bond uses a simple distance check so that
	// two active bonds are bound together if they approach within a certain
	// fixed distance of each other. This is provided by the instance of the
	// aeBondProximityOn class.
	//
	// Similarly, a bond between two actin monomers in a filament is broken
	// if the separation of the two bonds exceeds a specified maximum distance.
	// This test is provided by the instance of the aeBondOffRate class.
	// The instances are passed to the aeActiveBond base class for storage.

}

aefActinBond::~aefActinBond()
{
}

// Function to return the maximum number of active bonds of this type
// permitted in each active polymer.

long aefActinBond::GetActiveBondsPerPolymer() const
{
	return m_ActiveBondsPerPolymer;
}

// PVF that is called to check if a target bond, or monomer, can bind to the 
// current bond that is at the end of an active polymer. The conditions under 
// which monomers will successfully bind are specific to each aeActiveBond-derived 
// class, and are specified by the instance of a class derived from aeBondOnCondition
// that is created in the bond class' constructor above. If the binding succeeds, 
// the function returns true, otherwise it returns false. 
//
// We explicitly prevent binding of a bond to itself by comparing the id 
// of the target bond with that of the current bond. This test is not
// made in the aeBondProximityOn class so that we can also use it for 
// dissimilar bonds that can be bound into a polymer, e.g., a receptor-ligand pair.
// Next, we check that the target bond is not already part of an active polymer
// and, finally, we apply the bond-specific binding criteria appropriate for
// a free bond that binds to a polymerised bond at its tail, if the polymerised
// bond is at the polymer's tail, or its head, if the bond is at the polymer's head.
// Note that head and tail refer to the polymerised bond not the free one. Also
// note that the bond that receives this message should not be a phantom

bool aefActinBond::Activate(aeActiveBond* const pTargetBond)
{
	if(GetId() != pTargetBond->GetId() && !IsPhantom() && !pTargetBond->IsPolymerised() && !pTargetBond->IsPhantom())
	{
        if(pTargetBond->GetHeadAdjacentBond() != 0 || pTargetBond->GetTailAdjacentBond() != 0)
        {
            const zString txt = "Target bond has neighbours " + IGlobalSimBox::Instance()->ToString(pTargetBond->GetHeadAdjacentBond()->GetId()) + " " + IGlobalSimBox::Instance()->ToString(pTargetBond->GetTailAdjacentBond()->GetId());
            new CLogTextErrorMessage(IGlobalSimBox::Instance()->GetCurrentTime(), txt);
        }

		if(GetHeadMonomer())	// Current bond is the tail of the active polymer
		{	
			// Do extra local processing here to incorporate the actin monomer's 
			// internal state into the  binding event

			return TailOnCondition(pTargetBond);
		}
		else	// Current bond is the head of the active polymer
		{
			return HeadOnCondition(pTargetBond);
		}
	}

	return false;
}

// PVF that is called to see if the end monomers of an active polymer can
// dissociate. Because this function is called for both ends of a polymer,
// we check whether the current bond is the tail of the active polymer,
// in which case its head pointer is occupied, or if it is the head, in which case
// its head pointer is null.

bool aefActinBond::Deactivate()
{
	if(GetHeadMonomer())	// Current bond is the tail of the active polymer
	{	
			// Do extra local processing here to incorporate the actin monomer's 
			// internal state into the  unbinding event

		return HeadOffCondition();
	}
	else	// Current bond is the head of the active polymer
	{
		return TailOffCondition();
	}

	return false;
}

// Command handler function to change the basal probability of an active bond 
// detaching from its neighbour via its head connection. If the bond does not 
// implement the appropriate aeBondOffCondition, it is not modified.

void aefActinBond::SetFixedHeadOffRate(double rate)
{
    if(rate >= 0.0)
    {
        m_HeadBasalOffRate = rate; // Store the new value even if the bond does
                                   // not use the fixed off rate
        m_HeadCurrentOffRate = rate; 

        if(dynamic_cast<aeBondOffRate*>(m_pOff))
        {
	        dynamic_cast<aeBondOffRate*>(m_pOff)->SetHeadProbability(rate);
        }
    }
}

// Command handler function to change the basal probability of an active bond 
// detaching from its neighbour via its tail connection. If the bond does not 
// implement the appropriate aeBondOffCondition, it is not modified.

void aefActinBond::SetFixedTailOffRate(double rate)
{
    if(rate >= 0.0)
    {
        m_TailBasalOffRate = rate; // Store the new value even if the bond does
                                  // not use the fixed off rate
        m_TailCurrentOffRate = rate; 

        if(dynamic_cast<aeBondOffRate*>(m_pOff))
        {
	        dynamic_cast<aeBondOffRate*>(m_pOff)->SetTailProbability(rate);
        }
    }
}

// Helper function to set the current value of the detachment probability
// for a bond to detach from a filament the bond's head end. The default 
// value for this probability is the static member m_HeadBasalOffRate.

void aefActinBond::SetCurrentHeadOffRate(double rate)
{
    if(rate >= 0.0)
    {
        m_HeadCurrentOffRate = rate; 

        if(dynamic_cast<aeBondOffRate*>(m_pOff))
        {
	        dynamic_cast<aeBondOffRate*>(m_pOff)->SetHeadProbability(rate);
        }
    }
}

// Helper function to set the current value of the detachment probability
// for a bond to detach from a filament the bond's tail end. The default 
// value for this probability is the static member m_TailBasalOffRate.

void aefActinBond::SetCurrentTailOffRate(double rate)
{
    if(rate >= 0.0)
    {
        m_TailCurrentOffRate = rate; 

        if(dynamic_cast<aeBondOffRate*>(m_pOff))
        {
	        dynamic_cast<aeBondOffRate*>(m_pOff)->SetTailProbability(rate);
        }
    }
}

// Function to change the maximum distance at which two fActin monomers can
// associate to form a filament. The test uses a simple proximity check 
// to see if the two active bonds are closer than a maximum separation.
// We check that the dynamic cast succeeds before attempting to change
// the separation, as the ACN may use a different criterion but the user
// issue this command.

void aefActinBond::SetOnRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOn*>(m_pOn))
    {
	    dynamic_cast<aeBondProximityOn*>(m_pOn)->SetSeparation(max);
    }
}

void aefActinBond::SetOffRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOff*>(m_pOff))
    {
	    dynamic_cast<aeBondProximityOff*>(m_pOff)->SetSeparation(max);
    }
}

void aefActinBond::SetHeadOnRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOn*>(m_pOn))
    {
	    dynamic_cast<aeBondProximityOn*>(m_pOn)->SetHeadSeparation(max);
    }
}

void aefActinBond::SetHeadOffRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOff*>(m_pOff))
    {
	    dynamic_cast<aeBondProximityOff*>(m_pOff)->SetHeadSeparation(max);
    }
}

void aefActinBond::SetTailOnRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOn*>(m_pOn))
    {
	    dynamic_cast<aeBondProximityOn*>(m_pOn)->SetTailSeparation(max);
    }
}

void aefActinBond::SetTailOffRateSeparation(double max)
{
    if(dynamic_cast<aeBondProximityOff*>(m_pOff))
    {
	    dynamic_cast<aeBondProximityOff*>(m_pOff)->SetTailSeparation(max);
    }
}


// Function to make the transition ATP --> ADP-Pi with a specified
// probability.

bool aefActinBond::Hydrolyse()
{
    if(CCNTCell::GetRandomNo() < m_ATPHydrolysisProb)
    {
	    m_bATP = false;
	    m_bPi  = true;

        // Change the probabilities of binding/unbinding according
        // to a factor that represents the effects of ATP hydrolysis
        // on the process.

        SetCurrentHeadOffRate(m_HeadADPPiMultiplier*m_HeadBasalOffRate);
        SetCurrentTailOffRate(m_TailADPPiMultiplier*m_TailBasalOffRate);

        return true;
    }
    else
    {
        return false;
    }
}

// Function to make the transition ADP-Pi --> ADP with a specified
// probability.

bool aefActinBond::ReleasePi()
{
    if(CCNTCell::GetRandomNo() < m_ADPReleasePiProb)
    {
	    m_bATP = false;
	    m_bPi  = false;

        // Change the probabilities of binding/unbinding according
        // to a factor that represents the effects of ADP releasing
        // Pi on the process.

        SetCurrentHeadOffRate(m_HeadADPMultiplier*m_HeadBasalOffRate);
        SetCurrentTailOffRate(m_TailADPMultiplier*m_TailBasalOffRate);

        return true;
    }
    else
    {
        return false;
    }
}

// Function to make the transition ADP --> ATP with a specified
// probability

bool aefActinBond::Phosphorylate()
{
    if(CCNTCell::GetRandomNo() < m_ADPPhosphorylationProb)
    {
	    m_bATP = true;
	    m_bPi  = false;

        // Change the probabilities of binding/unbinding according
        // to a factor that represents the effects of ADP being
        // phosphorylated to ATP on the process.

        SetCurrentHeadOffRate(m_HeadBasalOffRate);
        SetCurrentTailOffRate(m_TailBasalOffRate);

        return true;
    }
    else
    {
        return false;
    }
}

// Command handler function to change the multiplier that is applied
// to the basal rate for the bond detaching at its head when it is in
// the ADP-Pi state.

void aefActinBond::SetHeadADPPiMultiplier(double factor)
{
    if(factor >= 0.0)
    {
        m_HeadADPPiMultiplier = factor;
    }
}

// Command handler function to change the multiplier that is applied
// to the basal rate for the bond detaching at its tail when it is in
// the ADP-Pi state.

void aefActinBond::SetTailADPPiMultiplier(double factor)
{
    if(factor >= 0.0)
    {
        m_TailADPPiMultiplier = factor;
    }
}

// Command handler function to change the multiplier that is applied
// to the basal rate for the bond detaching at its head when it is in
// the ADP state.

void aefActinBond::SetHeadADPMultiplier(double factor)
{
    if(factor >= 0.0)
    {
        m_HeadADPMultiplier = factor;
    }
}

// Command handler function to change the multiplier that is applied
// to the basal rate for the bond detaching at its tail when it is in
// the ADP state.

void aefActinBond::SetTailADPMultiplier(double factor)
{
    if(factor >= 0.0)
    {
        m_TailADPMultiplier = factor;
    }
}

