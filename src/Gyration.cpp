/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Gyration.cpp: implementation of the CGyration class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Gyration.h"
#include "GyrationData.h"
#include "SimState.h"
#include "ISimBox.h"



#if Platform == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
	using std::transform;
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
#elif Platform == XCMAC
#elif Platform == NEWPLATFORM1
#elif Platform == NEWPLATFORM2
#elif Platform == NEWPLATFORM3
#elif Platform == NEWPLATFORM4
#elif Platform == NEWPLATFORM5
#elif Platform == NEWPLATFORM6
#elif Platform == NEWPLATFORM7
#elif Platform == NEWPLATFORM8
#elif Platform == NEWPLATFORM9
#elif Platform == NEWPLATFORM10
#else							
	using std::transform;
#endif					

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that we cannot set the types of the polymers and beads in the constructor
// because we don't have access to the maps at the point the CAnalysis-derived
// objects are created. Instead, a map of the head and tail bead types onto 
// polymer types is stored in the CAnalysisState object and a virtual function,
// CAnalysis::ConvertNames() is used to perform the mapping for any CAnalysis 
// objects that need to.

CGyration::CGyration(const zString runId, long sample, 
                     const CGyrationData* const pAD) : CAnalysis(runId, sample, pAD),
                     m_Polymers(pAD->m_Polymers), m_bByType(pAD->m_bByType)
{
	
}

CGyration::~CGyration()
{
}

// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.

void CGyration::ConvertNames(const CAnalysisState& raState)
{

}

// Main function to measure the radius of gyration of a set of polymers.
// This is a simplified analysis that calculates the eigenvalues of the Rg tensor,
// and their sum (equivalent to the Rg**2 value) for all the beads in the polymers specified.
//
// Rg**2 = 1/N Sum( Ri - Rcm )**2
//
// where Rcm = 1/N Sum( Ri )
//
// and N is the number of points.
//
// We calculate the following quantities:
//
// if(!m_bByType)
//     only calculate Rg**2 for all polymers together
// else
//     calculate Rg**2 for all polymers, and for each type of polymer.
//
// In both cases, we use the GetCM) function in CPolymer to calculate the CM of each instance and then
// add them all up. This works because the CM is an additive quanity. Then we sum over each polymer's CM
// individually.

void CGyration::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{

    // If this is the first call, initialise all the observables to hold the data
    
    
    
    
    // On subsequent calls, do the calculation
    
    
    // Calculate the Rg**2 for all polymers first on the grounds that it is better to do extra work if the
    // m_bByType flag is true, than always to do this work if it is not needed.
    
    // Calculate the CM of all polymers
    
    
    // Now find the Rg**2
    
    
    
    // If the user has specified a breakdown, calculate the Rg**2 for each polymer type separately.
    
    if(m_bByType)
    {
        // loop over types
        
        // get Rcm for each type
        
        
        // get Rg**2 for each type
        
        
        

        
        
    }
    
    // Create observables to hold the Rg**2 data
    
    // Store the observables
    
//    m_vObservables.push_back(pxxStressPro);

    
    
    
    
}

