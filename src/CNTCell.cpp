/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CNTCell.cpp: implementation of the CCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimMiscellaneousFlags.h"  // Needed for stress tensor in curvilinear coords calculation
#include "SimMPSFlags.h"
#include "CNTCell.h"
#include "AbstractBead.h"
#include "ISimBox.h"
#include "Monitor.h"			// Needed to receive stress tensor contributions for analysis
#include "LogCNTBeadError.h"	// Needed to write error messages to the log state
#include "mpsBorder.h"
#include "ExternalCNTCell.h"

#include "RandomNumberSequence.h"
#include "IGlobalSimBox.h"      // Needed to see if lg interactions are used

#include "mpsSimBox.h"

//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

bool   CCNTCell::m_bReadFileOnce  = true;
long   CCNTCell::m_NextRNIndex    = 0;
zDoubleVector CCNTCell::m_RandomNumbers;
long          CCNTCell::m_StringSize = 8;
const zString CCNTCell::m_StringSeparator = "-";
const zString CCNTCell::m_AlphabetChars   = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


long   CCNTCell::m_CNTXCellNo			= 0;
long   CCNTCell::m_CNTYCellNo			= 0;
long   CCNTCell::m_CNTZCellNo			= 0;
double CCNTCell::m_CNTXCellWidth		= 0.0;
double CCNTCell::m_CNTYCellWidth		= 0.0;
double CCNTCell::m_CNTZCellWidth		= 0.0;
double CCNTCell::m_SimBoxXLength		= 0.0;
double CCNTCell::m_SimBoxYLength		= 0.0;
double CCNTCell::m_SimBoxZLength		= 0.0;
double CCNTCell::m_HalfSimBoxXLength	= 0.0;
double CCNTCell::m_HalfSimBoxYLength	= 0.0;
double CCNTCell::m_HalfSimBoxZLength	= 0.0;
double CCNTCell::m_dt					= 0.0;
double CCNTCell::m_lambda				= 0.0;
double CCNTCell::m_cutoffradius			= 0.0;
double CCNTCell::m_coradius2			= 0.0;
double CCNTCell::m_kT					= 0.0;
double CCNTCell::m_halfdt				= 0.0;
double CCNTCell::m_halfdt2				= 0.0;
double CCNTCell::m_invrootdt			= 0.0;
double CCNTCell::m_lamdt				= 0.0;
double CCNTCell::m_lgnorm               = 0.0;
double CCNTCell::m_dtoverkt			    = 0.0;
double CCNTCell::m_dispmag			    = 0.0;
uint64_t   CCNTCell::m_RNGSeed	        = -1ull;
long double CCNTCell::m_2Power32             =  4294967296.0l;              // 2**32
long double CCNTCell::m_Inv2Power32          =  1.0l/CCNTCell::m_2Power32;  // Inverse of 2**32

CMonitor* CCNTCell::m_pMonitor				 = 0;
ISimBox* CCNTCell::m_pISimBox                = 0;

const zArray2dDouble* CCNTCell::m_pvvConsInt = 0;
const zArray2dDouble* CCNTCell::m_pvvDissInt = 0;
const zArray2dDouble* CCNTCell::m_pvvLGInt   = 0;
const zArray2dDouble* CCNTCell::m_pvvLJDepth = 0;
const zArray2dDouble* CCNTCell::m_pvvLJRange = 0;
const zArray2dDouble* CCNTCell::m_pvvSCDepth = 0;
const zArray2dDouble* CCNTCell::m_pvvSCRange = 0;

// Platform-dependent initialisation of static arrays

#if Platform == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC

zArray2dDouble	     CCNTCell::m_vvConsInt;
zArray2dDouble	     CCNTCell::m_vvDissInt;
zArray2dDouble	     CCNTCell::m_vvLGInt;
zArray2dDouble	     CCNTCell::m_vvConsIntBackup;
zArray2dDouble	     CCNTCell::m_vvDissIntBackup;
zArray2dDouble	     CCNTCell::m_vvLGIntBackup;
zArray2dDouble	     CCNTCell::m_vvLJDepth;
zArray2dDouble	     CCNTCell::m_vvLJRange;
zArray2dDouble	     CCNTCell::m_vvSCDepth;
zArray2dDouble	     CCNTCell::m_vvSCRange;
zArray2dDouble	     CCNTCell::m_vvLJDelta;
zArray2dDouble	     CCNTCell::m_vvLJSlope;
zArray2dDouble	     CCNTCell::m_vvSCDelta;
zArray2dDouble	     CCNTCell::m_vvSCSlope;

#elif Platform == XCMAC

zArray2dDouble	     CCNTCell::m_vvConsInt;
zArray2dDouble	     CCNTCell::m_vvDissInt;
zArray2dDouble	     CCNTCell::m_vvLGInt;
zArray2dDouble	     CCNTCell::m_vvConsIntBackup;
zArray2dDouble	     CCNTCell::m_vvDissIntBackup;
zArray2dDouble	     CCNTCell::m_vvLGIntBackup;
zArray2dDouble	     CCNTCell::m_vvLJDepth;
zArray2dDouble	     CCNTCell::m_vvLJRange;
zArray2dDouble	     CCNTCell::m_vvSCDepth;
zArray2dDouble	     CCNTCell::m_vvSCRange;
zArray2dDouble	     CCNTCell::m_vvLJDelta;
zArray2dDouble	     CCNTCell::m_vvLJSlope;
zArray2dDouble	     CCNTCell::m_vvSCDelta;
zArray2dDouble	     CCNTCell::m_vvSCSlope;

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

// Static arrays to hold the above data without a dereference being needed
// for every access.

zArray2dDouble	     CCNTCell::m_vvConsInt		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvDissInt		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvLGInt		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvConsIntBackup  = 0.0;
zArray2dDouble	     CCNTCell::m_vvDissIntBackup  = 0.0;
zArray2dDouble	     CCNTCell::m_vvLGIntBackup    = 0.0;
zArray2dDouble	     CCNTCell::m_vvLJDepth		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvLJRange		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvSCDepth		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvSCRange	  	  = 0.0;
zArray2dDouble	     CCNTCell::m_vvLJDelta		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvLJSlope		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvSCDelta		  = 0.0;
zArray2dDouble	     CCNTCell::m_vvSCSlope		  = 0.0;

#endif

// Function to set the static member variable that holds a pointer to the
// CMonitor object. This is used inside the force calculation loop to pass
// observable data to the monitor during the simulation.

void CCNTCell::SetMonitorPointer(CMonitor* pMon)
{
	CCNTCell::m_pMonitor = pMon;
}

// Function to set the static member variable that holds a pointer to the
// CMonitor object. This is used inside the force calculation loop to pass
// observable data to the monitor during the simulation.

void CCNTCell::SetISimBoxPointer(ISimBox* pISimBox)
{
	CCNTCell::m_pISimBox = pISimBox;
}

// Function to set the static member variable for the RNG seed. It can be 
// invoked by any object and resets the seed for all subsequent calls to
// the Randf() function. Note the default value of -1 (set above)
// until a call to SetRNGSeed() is made. The user-supplied seed is 32 bit
// and this is converted to 64 bit for the RNG. We take the abs value as the original
// RNG required a negative integeer, but now we want a positive one but I want to maintain
// backwards compatibility with the existing CDFs.

void CCNTCell::SetRNGSeed(long idum)
{
    CCNTCell::m_RNGSeed = static_cast<uint64_t>(abs(idum));
}

// Function to set the static member variables holding the size of the
// simulation box for use in applying the periodic boundary conditions.
// Note the default values of 0 until a call to this function is made.

void CCNTCell::SetSimBoxLengths( const long nx, const long ny, const long nz,
								 const double cntlx, const double cntly, const double cntlz )
{
	CCNTCell::m_CNTXCellNo			= nx;
	CCNTCell::m_CNTYCellNo			= ny;
	CCNTCell::m_CNTZCellNo			= nz;
	CCNTCell::m_CNTXCellWidth		= cntlx;
	CCNTCell::m_CNTYCellWidth		= cntly;
	CCNTCell::m_CNTZCellWidth		= cntlz;
	CCNTCell::m_SimBoxXLength		= static_cast<double>(nx)*cntlx;
	CCNTCell::m_SimBoxYLength		= static_cast<double>(ny)*cntly;
	CCNTCell::m_SimBoxZLength		= static_cast<double>(nz)*cntlz;
	CCNTCell::m_HalfSimBoxXLength	= 0.5*m_SimBoxXLength;
	CCNTCell::m_HalfSimBoxYLength	= 0.5*m_SimBoxYLength;
	CCNTCell::m_HalfSimBoxZLength	= 0.5*m_SimBoxZLength;
}

// Function to set the static member variables related to the timestep size.
// Note that they are all set to zero above until a call to this function
// is made. It also sets the potential cutoff radius used in MD simulations only.
//
// Also note that the prefactor to the random numbers in the UpdateForce() algorithm
// is determined by the requirements 
//
// Randf() returns numbers with <x> = 0.5 and Var(x) = 1/12
//
// We want random numbers with <y> = 0 and Var(y) = kT
//
// The random and dissipative force coefficients are related by
//
//								sigma**2 = 2*gamma*kT
//
// and also that the random force scales with timestep as 1/sqrt(dt).
//
// This leads to the prefactor of sqrt(12*2*kT/dt).


void CCNTCell::SetTimeStepConstants( const double dt, const double lambda, 
									 const double cutoffradius, const double kT )
{
	CCNTCell::m_dt					= dt;
	CCNTCell::m_lambda				= lambda;
	CCNTCell::m_cutoffradius		= cutoffradius;
	CCNTCell::m_coradius2			= cutoffradius*cutoffradius;
	CCNTCell::m_kT					= kT;
	CCNTCell::m_halfdt				= 0.5*dt;
	CCNTCell::m_halfdt2				= 0.5*dt*dt;
	CCNTCell::m_invrootdt			= sqrt(24.0*kT/dt);
	CCNTCell::m_lamdt				= lambda*dt;
    CCNTCell::m_lgnorm				= 15.0/xxBase::m_globalTwoPI;
	CCNTCell::m_dispmag		        = sqrt(2.0*dt); // Not including the diffusion coefficient

    if(kT > 0.0)
    {
	    CCNTCell::m_dtoverkt		= dt/kT; // Not including the diffusion coefficient
    }
    else
    {
	    CCNTCell::m_dtoverkt		= 0.0;
    }
}

// Command handler function to turn the DPD bead-bead interactions on and off.
//
// This zeros the conservative, dissipative and random forces between all 
// bead types. Note that we do not change the value of the time step itself, 
// we just use it to change the two constants that are used in the integration
// scheme for updating the beads' momenta and positions.
//
// m_halfdt is used in UpdateMom() to calculate the new bead momenta
// m_halfdt2 is used in UpdatePos() to calculate the new bead positions
//
// Passing in a zero time step prevents the forces calculated in UpdateForce()
// from propagating to the momenta and positions, but does not stop them
// being calculated. Hence, no speedup is obtained. The forces are still
// available in the CAbstractBead class. Also, the stress tensor for each
// bead is not zeroed as a result of this command.
//
// To restore the forces, the original time step is passed back in.

void CCNTCell::ToggleDPDBeadForces(const double dt)
{
	CCNTCell::m_halfdt	= 0.5*dt;
	CCNTCell::m_halfdt2	= 0.5*dt*dt;
}

// Command handler function to zero the DPD conservative bead-bead interactions. 
// The argument indicates whether the forces are zeroed or replaced by the original
// set. We copy the array of conservative force parameters into a backup array and
// replace its elements with zeroes. We ensure the backup array is empty before
// copying elements as we use a back_inserter that just expands the array size.

void CCNTCell::ToggleDPDBeadConservativeForces(bool bZero)
{
	if(bZero)
	{
		m_vvConsIntBackup.clear();
		m_vvConsIntBackup.resize(m_vvConsInt.size());

#if EnableDPDLG == ExperimentEnabled
        if(IGlobalSimBox::Instance()->IsDPDLG())
        {
		    m_vvLGIntBackup.clear();
		    m_vvLGIntBackup.resize(m_vvLGInt.size());
        }
#endif

		// Fill the backup array with the original values and replace them by 
		// zeroes in the normal arrays: note that the array is guaranteed to 
		// be square by construction. Also note, that the number of LG force 
        // parameters must be the same as the number of standard conservative
        // force parameters.
		
		const long rowSize = m_vvConsInt.size();

		for(long row=0; row<rowSize; row++)
		{
			copy(m_vvConsInt.at(row).begin(), m_vvConsInt.at(row).end(), back_inserter(m_vvConsIntBackup.at(row)));					
			m_vvConsInt.at(row).assign(rowSize, 0.0);

#if EnableDPDLG == ExperimentEnabled
        if(IGlobalSimBox::Instance()->IsDPDLG())
        {
			copy(m_vvLGInt.at(row).begin(), m_vvLGInt.at(row).end(), back_inserter(m_vvLGIntBackup.at(row)));					
			m_vvLGInt.at(row).assign(rowSize, 0.0);
        }
#endif
		}
	}
	else
	{
		// Restore the original interaction parameters

        const long rowSize = m_vvConsInt.size();

#if EnableDPDLG == ExperimentEnabled

		for(long row=0; row<rowSize; row++)
		{
			m_vvConsInt.at(row).clear();
			copy(m_vvConsIntBackup.at(row).begin(), m_vvConsIntBackup.at(row).end(), back_inserter(m_vvConsInt.at(row)));

        if(IGlobalSimBox::Instance()->IsDPDLG())
            {
                m_vvLGInt.at(row).clear();
			    copy(m_vvLGIntBackup.at(row).begin(), m_vvLGIntBackup.at(row).end(), back_inserter(m_vvLGInt.at(row)));
            }
        }

		m_vvConsIntBackup.clear();
        if(IGlobalSimBox::Instance()->IsDPDLG())
        {
		    m_vvLGIntBackup.clear();
        }

#elif EnableDPDLG == ExperimentDisabled

		for(long row=0; row<rowSize; row++)
		{
			m_vvConsInt.at(row).clear();
			copy(m_vvConsIntBackup.at(row).begin(), m_vvConsIntBackup.at(row).end(), back_inserter(m_vvConsInt.at(row)));
        }

		m_vvConsIntBackup.clear();

#endif

	}
}

// Command handler function to zero the DPD random and dissipative bead-bead 
// interactions. We zero the array of dissipation coefficients and because the 
// random forces are proportional to the dissipative ones, via the
// Fluctuation-Dissipation theorem, they are both zeroed.

void CCNTCell::ToggleDPDBeadThermostat(bool bZero)
{
	if(bZero)
	{
		m_vvDissIntBackup.clear();
		m_vvDissIntBackup.resize(m_vvDissInt.size());

		const long rowSize = m_vvDissInt.size();

		for(long row=0; row<rowSize; row++)
		{
			copy(m_vvDissInt.at(row).begin(), m_vvDissInt.at(row).end(), back_inserter(m_vvDissIntBackup.at(row)));
						
			m_vvDissInt.at(row).assign(rowSize, 0.0);
		}
	}
	else
	{		
		const long rowSize = m_vvDissInt.size();

		for(long row=0; row<rowSize; row++)
		{
			m_vvDissInt.at(row).clear();
			copy(m_vvDissIntBackup.at(row).begin(), m_vvDissIntBackup.at(row).end(), back_inserter(m_vvDissInt.at(row)));
		}

		m_vvDissIntBackup.clear();
	}
}

// Command handler function to change the integration time step.
// It also changes the parameters that depend on the time step. We make 
// a final check that the step size is positive definite before updating
// the values. 
//
// Note that we don't make any attempt to mitigate the effects of the step 
// size change on the bead forces, etc. If this is to be done it should be 
// done in the CSimBox that has access to all bead instances.

void CCNTCell::ChangeTimeStepConstants(const double dt)
{
	if(dt > 0.0)
	{
		CCNTCell::m_dt				= dt;
		CCNTCell::m_halfdt			= 0.5*dt;
		CCNTCell::m_halfdt2			= 0.5*dt*dt;
		CCNTCell::m_invrootdt		= sqrt(24.0*m_kT/dt);
		CCNTCell::m_lamdt			= m_lambda*dt;
	    CCNTCell::m_dispmag		    = sqrt(2.0*dt); // Not including the diffusion coefficient

        if(m_kT > 0.0)
        {
	        CCNTCell::m_dtoverkt	= dt/m_kT; // Not including the diffusion coefficient
        }
        else
        {
	        CCNTCell::m_dtoverkt	= 0.0;
        }
	}
}

// Functions to set the matrices holding the bead-bead interaction parameters
// and any other bead internal structure needed to calculate the forces between
// beads during the simulation.
// We use separate functions for each of the simulation types to allow extra
// data to be added to each independently.
//
// Note that the data pointed to is const but the pointers are not.
//
// We copy the DPD beda-bead interactions for the BD beads for now

void CCNTCell::SetBDBeadStructure(const zArray2dDouble* pvvConsInt,
								   const zArray2dDouble* pvvDissInt)
{
	CCNTCell::m_pvvConsInt = pvvConsInt;
	CCNTCell::m_pvvDissInt = pvvDissInt;
}

void CCNTCell::SetDPDBeadStructure(const zArray2dDouble* pvvConsInt,
								   const zArray2dDouble* pvvDissInt)
{
	CCNTCell::m_pvvConsInt = pvvConsInt;
	CCNTCell::m_pvvDissInt = pvvDissInt;
}

void CCNTCell::SetDPDBeadStructure(const zArray2dDouble* pvvConsInt,
								   const zArray2dDouble* pvvDissInt,
                                   const zArray2dDouble* pvvLGInt)
{
#if EnableDPDLG == ExperimentEnabled
	CCNTCell::m_pvvConsInt = pvvConsInt;
	CCNTCell::m_pvvDissInt = pvvDissInt;

        if(IGlobalSimBox::Instance()->IsDPDLG())
    {
        CCNTCell::m_pvvLGInt   = pvvLGInt;
    }
#endif
}

void CCNTCell::SetMDBeadStructure(const zArray2dDouble* pvvLJDepth,
								  const zArray2dDouble* pvvLJRange,
								  const zArray2dDouble* pvvSCDepth,
								  const zArray2dDouble* pvvSCRange)
{
	CCNTCell::m_pvvLJDepth = pvvLJDepth;
	CCNTCell::m_pvvLJRange = pvvLJRange;
	CCNTCell::m_pvvSCDepth = pvvSCDepth;
	CCNTCell::m_pvvSCRange = pvvSCRange;
}

// Functions used by the CSimState to change the value of the DPD conservative
// and dissipative interaction parameters between two bead types in response 
// to a command. Because the matrix is symmetric we have to set both corresponding
// elements when an off-diagonal element is specified.

void CCNTCell::SetDPDBeadConsInt(long firstType, long secondType, double newValue)
{
	// Check that the types are within the container range: note that the 
	// container is symmetric by construction so we only need to check one dimension.

	if(0 <= firstType  && firstType  < m_vvConsInt.size() &&
	   0 <= secondType && secondType < m_vvConsInt.size())
	{
		m_vvConsInt.at(firstType).at(secondType) = newValue;	
		m_vvConsInt.at(secondType).at(firstType) = newValue;	
	}

}

void CCNTCell::SetDPDBeadDissInt(long firstType, long secondType, double newValue)
{
	if(0 <= firstType  && firstType  < m_vvDissInt.size() &&
	   0 <= secondType && secondType < m_vvDissInt.size())
	{
		m_vvDissInt.at(firstType).at(secondType) = newValue;
		m_vvDissInt.at(secondType).at(firstType) = newValue;	
	}
}

// Function used by the CSimState to change the value of the DPD density-dependent
// LG force. This feature is only allowed when the EnableDPDLG flag is set.
// Because the matrix is symmetric we have to set both corresponding
// elements when an off-diagonal element is specified.

void CCNTCell::SetDPDBeadLGInt(long firstType, long secondType, double newValue)
{
	// Check that the types are within the container range: note that the 
	// container is symmetric by construction so we only need to check one dimension.

#if EnableDPDLG == ExperimentEnabled
        if(IGlobalSimBox::Instance()->IsDPDLG())
    {
	    if(0 <= firstType  && firstType  < m_vvLGInt.size() &&
	       0 <= secondType && secondType < m_vvLGInt.size())
	    {
		    m_vvLGInt.at(firstType).at(secondType) = newValue;	
		    m_vvLGInt.at(secondType).at(firstType) = newValue;	
	    }
    }
#endif
}

// Function used by the CSimBox to add a new DPD bead type to the 
// interaction matrix when a ccChangeBeadType command is executed.
// Other commands may also require calling this function.
// It copies the interactions from the original type into a new
// row and column of the matrix and sets the new type's self-interaction
// equal to its previous value. These values may be changed later using
// the SetDPDConsInt(), SetDPDDissInt() functions.
//
// We also copy the new values into the conservative and dissipative force
// backup arrays in case the forces are currently zeroed. Note that the
// backup array always contains non-zero interaction parameters whereas 
// the current arrays may be zero or non-zero. However, any changes made
// to the bead types' interaction parameters while the forces are zeroed
// will be lost when the backup arrays are restored. Adding new bead types
// and subsequently changing their interactions while the forces are zeroed
// is not very sensible.

void CCNTCell::AddDPDBeadType(long oldType)
{
    long row; // Counter used several times below

	// Add the original bead type's interactions to the end of
	// each row and create a new row at the end of the matrix

	for(row = 0; row < m_vvConsInt.size(); row++)
	{
		double oldConsInt = m_vvConsInt.at(row).at(oldType);
		double oldDissInt = m_vvDissInt.at(row).at(oldType);

		m_vvConsInt.at(row).push_back(oldConsInt);
		m_vvDissInt.at(row).push_back(oldDissInt);

		// If the conservative or dissipative arrays are currently zeroed, 
		// update the backup arrays as well. We store the original non-zero  
		// value for the new bead type's interaction parameter not 0.0.

		if(m_vvConsIntBackup.size() > 0)
		{
			m_vvConsIntBackup.at(row).push_back(m_vvConsIntBackup.at(row).at(oldType));
		}

		if(m_vvDissIntBackup.size() > 0)
		{
			m_vvDissIntBackup.at(row).push_back(m_vvDissIntBackup.at(row).at(oldType));
		}
	}

	// Copy the row corresponding to the original type to the
	// bottom of the matrix to create the new type's row

	m_vvConsInt.push_back(m_vvConsInt.at(oldType));
	m_vvDissInt.push_back(m_vvDissInt.at(oldType));

	if(m_vvConsIntBackup.size() > 0)
	{
		m_vvConsIntBackup.push_back(m_vvConsIntBackup.at(oldType));
	}

	if(m_vvDissIntBackup.size() > 0)
	{
		m_vvDissIntBackup.push_back(m_vvDissIntBackup.at(oldType));
	}

    // Now repeat for the density-dependent force parameters

#if EnableDPDLG == ExperimentEnabled
        if(IGlobalSimBox::Instance()->IsDPDLG())
    {
	    for(row = 0; row < m_vvConsInt.size(); row++)
	    {
		    double oldLGInt   = m_vvLGInt.at(row).at(oldType);

		    m_vvLGInt.at(row).push_back(oldLGInt);

		    if(m_vvLGIntBackup.size() > 0)
		    {
			    m_vvLGIntBackup.at(row).push_back(m_vvLGIntBackup.at(row).at(oldType));
		    }
	    }

	    m_vvLGInt.push_back(m_vvLGInt.at(oldType));

	    if(m_vvLGIntBackup.size() > 0)
	    {
		    m_vvLGIntBackup.push_back(m_vvLGIntBackup.at(oldType));
	    }
    }
#endif

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCNTCell::CCNTCell() : CAbstractCell(), m_bExternal(false)
{
	for(short int i=0; i<27; i++)	// We allow 3d values for 2d simulations to avoid
	{								// having to include SimDefs.h in the .h file
		m_aNNCells[i] = 0;
	}
	for(short int j=0; j<13; j++)
	{
		m_aIntNNCells[j] = 0;
	}
}

CCNTCell::CCNTCell(long index, long coords[3], bool flag) : CAbstractCell()
{
	// Initialise the member variables of CAbstractCell 

	m_id			= index;

	m_BLIndex[0]	= coords[0];
	m_BLIndex[1]	= coords[1];
	m_BLIndex[2]	= coords[2];

	m_BLCoord[0]	= static_cast<double>(coords[0])*m_CNTXCellWidth;
	m_BLCoord[1]	= static_cast<double>(coords[1])*m_CNTYCellWidth;
	m_BLCoord[2]	= static_cast<double>(coords[2])*m_CNTZCellWidth;

	m_TRCoord[0]	= m_BLCoord[0] + m_CNTXCellWidth;
	m_TRCoord[1]	= m_BLCoord[1] + m_CNTYCellWidth;
	m_TRCoord[2]	= m_BLCoord[2] + m_CNTZCellWidth;

	// Now the CCNTCell member variables

	m_bExternal		= flag;

	for(short int i=0; i<27; i++)	// We allow 3d values for 2d simulations to avoid
	{								// having to include SimDefs.h in the .h file
		m_aNNCells[i] = 0;
	}
	for(short int j=0; j<13; j++)
	{
		m_aIntNNCells[j] = 0;
	}
	
	// Copy the bead structure data from the static pointer members into static
	// storage to avoid having to dereference the arrays every time we access them.
	// This should only be done once, so we check to see if the first array is empty
	// before using the back_inserter() algorithm. 
	//
	// The containers are emptied in the destructor to prevent the static data 
	// persisting between successive runs. If this is not done, the test 
	// for the container being empty fails on all but the first of a sequence 
	// of runs and the data from the first run is used for all subsequent runs.

#if SimIdentifier == BD
// We copy the DPD bead-bead interactions to the BD case for now
	if(m_vvConsInt.empty())
	{
		copy(m_pvvConsInt->begin(), m_pvvConsInt->end(), back_inserter(m_vvConsInt));
		copy(m_pvvDissInt->begin(), m_pvvDissInt->end(), back_inserter(m_vvDissInt));
	}

#elif SimIdentifier == DPD

	if(m_vvConsInt.empty())
	{
		copy(m_pvvConsInt->begin(), m_pvvConsInt->end(), back_inserter(m_vvConsInt));
		copy(m_pvvDissInt->begin(), m_pvvDissInt->end(), back_inserter(m_vvDissInt));

#if EnableDPDLG == ExperimentEnabled
        // We don't have access to the IGlobalSimBox::Instance()->IsDPDLG() here
        // as the CNT cells are created from within the SimBox's constructor
        // and it has not filled up its base class instance variables yet.
        // We just check if the LG pointer is valid instead.
        if(m_pvvLGInt)
        {
		    copy(m_pvvLGInt->begin(), m_pvvLGInt->end(), back_inserter(m_vvLGInt));
        }
#endif
    }

#elif SimIdentifier == MD

	if(m_vvLJDepth.empty())
	{
		copy(m_pvvLJDepth->begin(), m_pvvLJDepth->end(), back_inserter(m_vvLJDepth));
		copy(m_pvvLJRange->begin(), m_pvvLJRange->end(), back_inserter(m_vvLJRange));
		copy(m_pvvSCDepth->begin(), m_pvvSCDepth->end(), back_inserter(m_vvSCDepth));
		copy(m_pvvSCRange->begin(), m_pvvSCRange->end(), back_inserter(m_vvSCRange));

		// Now store the shifted potentials and their slopes for each bead type pair,
		// and scale all potential ranges by the range of the first LJ potential. 
		// This means that the length and energy scales for the single-component LJ
		// fluid are always unity, and define the scales for all other potentials.
		// However, it also means that we have to set the cutoff radius manually and
		// use it in the UpdateForce() algorithm explicitly.
		// Notice that in the slope of the shifted 
		// potentials there is an extra factor of 1/m_cutoffradius that is absorbed
		// by the (dr - m_cutoffradius) term when it is evaluated.
		//
		// We expect the depth and range arrays for both the LJ and SC potentials
		// to have the same number of elements. We copy the original data in to the
		// delta and slope arrays simply to set their size. The values are overwritten 
		// with the actual shift and slope of the potentials.
		//
		// We keep a factor of 4 in the depth of both potentials to avoid having to
		// multiply by it inside the force loop. We still need to insert another factor
		// of 6 to the force calculation (but not the potential in GetPotentialEnergy()).

			copy(m_pvvLJDepth->begin(), m_pvvLJDepth->end(), back_inserter(m_vvLJDelta));
			copy(m_pvvLJRange->begin(), m_pvvLJRange->end(), back_inserter(m_vvLJSlope));
			copy(m_pvvSCDepth->begin(), m_pvvSCDepth->end(), back_inserter(m_vvSCDelta));
			copy(m_pvvSCRange->begin(), m_pvvSCRange->end(), back_inserter(m_vvSCSlope));

			const double enorm = 0.25*m_vvLJDepth.at(0).at(0);
			const double lnorm = m_vvLJRange.at(0).at(0);

			if(enorm > 0.0 && lnorm > 0.0)
			{
				m_cutoffradius	= m_cutoffradius/lnorm;
				m_coradius2		= m_cutoffradius*m_cutoffradius;

				for(long type1=0; type1<m_vvLJDepth.size(); type1++)
				{
					for(long type2=0; type2<m_vvLJDepth.at(0).size(); type2++)
					{				
						// Scale LJ and SC potential ranges and depths by those of 
						// the first LJ potential: if either of these is zero, then
						// set all potential depths to 0 and ranges to unity to simulate
						// an ideal gas of non-interacting particles.
						// We have to divide the cutoff radius by the first range as well.

						m_vvLJDepth.at(type1).at(type2) /= enorm;
						m_vvSCDepth.at(type1).at(type2) /= enorm;

						m_vvLJRange.at(type1).at(type2) /= lnorm;
						m_vvSCRange.at(type1).at(type2) /= lnorm;

						// Shifted LJ potential

						double eLJ        = m_vvLJDepth.at(type1).at(type2);	
						double sLJOverRC  = m_vvLJRange.at(type1).at(type2)/m_cutoffradius;
						double sLJR3	  = sLJOverRC*sLJOverRC*sLJOverRC;
						double sLJR6	  = sLJR3*sLJR3;

						double deltaLJ	  = eLJ*sLJR6*(sLJR6 - 1.0);
						double slopeLJ    = 6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0)/m_cutoffradius;

						m_vvLJDelta.at(type1).at(type2) = deltaLJ;
						m_vvLJSlope.at(type1).at(type2) = slopeLJ;

						// Shifted Soft-Core potential. 

						double eSC		  = m_vvSCDepth.at(type1).at(type2);	
						double sSCOverRC  = m_vvSCRange.at(type1).at(type2)/m_cutoffradius;
						double sSCR3	  = sSCOverRC*sSCOverRC*sSCOverRC;
						double sSCR9	  = sSCR3*sSCR3*sSCR3;

						double deltaSC    = eSC*sSCR9;
						double slopeSC    = 9.0*eSC*sSCR9/m_cutoffradius;

						m_vvSCDelta.at(type1).at(type2) = deltaSC;
						m_vvSCSlope.at(type1).at(type2) = slopeSC;			
					}
				}
			}
			else
			{
				for(long type1=0; type1<m_vvLJDepth.size(); type1++)
				{
					for(long type2=0; type2<m_vvLJDepth.at(0).size(); type2++)
					{				
						m_vvLJDepth.at(type1).at(type2) = 0.0;
						m_vvSCDepth.at(type1).at(type2) = 0.0;

						m_vvLJRange.at(type1).at(type2) = 1.0;
						m_vvSCRange.at(type1).at(type2) = 1.0;
					}
				}
			}
	}

#endif
}

// Copy constructor. We don't have to check any static members as they must have
// already been initialised when the first instance of this class was created, which 
// must have preceded the call to the copy constructor. All we have to do here is
// copy the pointers to the neighbouring cells.

CCNTCell::CCNTCell(const CCNTCell& oldCell) : CAbstractCell(oldCell), 
                                              m_bExternal(oldCell.m_bExternal)
{
	for(short int i=0; i<27; i++)	
	{
		m_aNNCells[i] = oldCell.m_aNNCells[i];
	}
	for(short int j=0; j<13; j++)
	{
		m_aIntNNCells[j] = oldCell.m_aIntNNCells[j];
	}
}


// Empty the static containers holding the bead-bead interaction parameters for
// BD, DPD and MD simulations. We ensure this is only done once by checking
// that the containers are not empty before doing it.

CCNTCell::~CCNTCell()
{
#if SimIdentifier == BD

	if(!m_vvConsInt.empty())
	{
		m_vvConsInt.clear();
		m_vvDissInt.clear();
	}

#elif SimIdentifier == DPD

	if(!m_vvConsInt.empty())
	{
		m_vvConsInt.clear();
		m_vvDissInt.clear();

#if EnableDPDLG == ExperimentEnabled
		m_vvLGInt.clear();
#endif
	}

#elif SimIdentifier == MD

	if(!m_vvLJDepth.empty())
	{
		m_vvLJDepth.clear();
		m_vvLJRange.clear();
		m_vvSCDepth.clear();
		m_vvSCRange.clear();
		m_vvLJDelta.clear();
		m_vvLJSlope.clear();
		m_vvSCDelta.clear();
		m_vvSCSlope.clear();
	}

#endif

}

//////////////////////////////////////////////////////////////////////


// Function to calculate the force on each bead in the current cell due to 
// interactions with other beads in the same cell and those in neighbouring cells.
// To avoid self-interactions we perform separate loops over other 
// beads in the same cell as the current bead and those in different cells.
// We only apply the PBCs to interactions between beads in different
// cells both of which are on the simulation box boundary.
//
// Conditional compilation is used to select which of the simulation methods
// is used. 
// 
// In BD simulations, the equation of motion is:
//
// r(t + dt) = r(t) + D*F(t)*dt/(kT) + R(t,dt)
//
// where r(t)    = Position of bead at start of timestep at time t
//       r(t+dt) = Position of bead at end of time-step at time t + dt
//       D       = bead translational diffusion coefficient (assuming spherical beads)
//       F(t)    = total force on bead at start of time-step at time t
//       kT      = Temperature
//       R(t,dt) = Random displacement satisfying <R> = 0, <R.R> = 2*D*dt
//       dt      = time-step
// 
// We use the same conservative non-bonded, bead-bead force law as in DPD.
// But because the random displacement is not actually a force we add it in 
// the UpdatePos() function not here.
//
// The DPD force is made up of three parts:
//
// 1 Conservative interaction
// 2 Dissipative interaction
// 3 Random iteraction
//
// in addition, DPD beads can have an explicit radius of interaction per bead type.
// This is selected by the flag UseDPDBeadRadii. If the flag is not set the
// radius of all beads is set to 0.5 and the test for interactions is compiled
// using a hard-wired if() statement. This is to avoid the overhead of calculating
// the explicit test of dr with bead interaction radii when they are not being used.
//
// whereas the MD force has two contributions:
//
// 1 Lennard-Jones interaction
// 2 Soft-Core repulsion 
//
// Beads interact in an MD simulation if their separation is less than a fixed 
// cut-off distance. This does not depend on the type of MD bead.
//
// This function performs sums over all interacting bead pairs and so it
// requires that all bead coordinates be at the same timestep and that 
// they not be updated until after it has been called for all CNT cells 
// and their contained beads.

void CCNTCell::UpdateForce()
{
//    const long targetBeadId = 4; // Write out interactions for this bead only

	mpsSimBox::GlobalCellCounter++;  // increment the counter for intra-cell force calculations
	
	long  localCellCellCounter = 0;

#if SimIdentifier == BD

	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3];
	double dr, dr2, drmax;

	double wr, wr2;
	double conForce;
    double newForce1[3], newForce2[3];
	

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		// Zero the stress tensor for this bead. Notice that because of the
		// way we only sum over beads distinct from the current bead in the same
		// cell, and over beads in cells to the right and above the current cell
		// we don't have to worry about double counting the pairwise forces. We
		// can hence store the N(N-1)/2 contributions to the stress tensor in the
		// beads that form the first ones accessed in the double loops.

		for(short int j=0; j<9; j++)
		{
			(*iterBead1)->m_Stress[j] = 0.0;
		}

		for( riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=(*iterBead1)->m_id; ++riterBead2 )
		{
			dx[0] = ((*iterBead1)->m_Pos[0] - (*riterBead2)->m_Pos[0]);
			dx[1] = ((*iterBead1)->m_Pos[1] - (*riterBead2)->m_Pos[1]);

#if SimDimension == 2
			dx[2] = 0.0;
#elif SimDimension == 3
			dx[2] = ((*iterBead1)->m_Pos[2] - (*riterBead2)->m_Pos[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
			dr = sqrt(dr2);
			drmax = (*iterBead1)->GetRadius()+(*riterBead2)->GetRadius();
			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = wr*wr;

// Conservative force magnitude

					conForce = m_vvConsInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*wr/dr;				
//                    conForce = 0.0;
					
                    for(short int i=0; i<3; i++)
                    {
                        newForce1[i] = conForce*dx[i];
 					    newForce2[i] = -conForce*dx[i];

                        (*iterBead1)->m_Force[i]  += newForce1[i];
					    (*riterBead2)->m_Force[i] += newForce2[i];
                    }

					// stress tensor summation

					(*iterBead1)->m_Stress[0] += dx[0]*newForce1[0];
					(*iterBead1)->m_Stress[1] += dx[1]*newForce1[0];
					(*iterBead1)->m_Stress[2] += dx[2]*newForce1[0];
					(*iterBead1)->m_Stress[3] += dx[0]*newForce1[1];
					(*iterBead1)->m_Stress[4] += dx[1]*newForce1[1];
					(*iterBead1)->m_Stress[5] += dx[2]*newForce1[1];
					(*iterBead1)->m_Stress[6] += dx[0]*newForce1[2];
					(*iterBead1)->m_Stress[7] += dx[1]*newForce1[2];
					(*iterBead1)->m_Stress[8] += dx[2]*newForce1[2];

					// Pass the stress tensor components to the CMonitor
					// for use in analysing the stress over slices.
					// Note that dx[2] might be close to zero and we should
					// check for this.
                    // Is there a problem in the stress calculation now that the 
                    // force on each bead in an interacting pair is not the same?
					
					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
					m_pMonitor->AddBeadStress(*iterBead1, *riterBead2, newForce1, dx);
#endif
				}
				else
				{
					TraceInt("same cell bead", (*iterBead1)->GetId());
					TraceInt("interacts with", (*riterBead2)->GetId());
					TraceVector("1st pos",	(*iterBead1)->GetXPos(),
											(*iterBead1)->GetYPos(), 
											(*iterBead1)->GetZPos() );
					TraceVector("2nd pos",	(*riterBead2)->GetXPos(),
											(*riterBead2)->GetYPos(), 
											(*riterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
			for( iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++ )
			{
				dx[0] = ((*iterBead1)->m_Pos[0] - (*iterBead2)->m_Pos[0]);
				dx[1] = ((*iterBead1)->m_Pos[1] - (*iterBead2)->m_Pos[1]);

#if SimDimension == 2
				dx[2] = 0.0;
#elif SimDimension == 3
				dx[2] = ((*iterBead1)->m_Pos[2] - (*iterBead2)->m_Pos[2]);
#endif

				if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
				{
					if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
					else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

					if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
					else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
					if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
					else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

				}

				dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we have to change the distance-dependence according
// to the bead interaction radii.

				dr = sqrt(dr2);
				drmax = (*iterBead1)->GetRadius()+(*iterBead2)->GetRadius();
				if( dr < drmax )
				{		
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr/drmax);
						wr2 = wr*wr;
 
 // Conservative force magnitude

					    conForce = m_vvConsInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr/dr;				
 //                       conForce = 0.0;

                        for(short int i=0; i<3; i++)
                        {
                            newForce1[i] = conForce*dx[i];
 					        newForce2[i] = -conForce*dx[i];

                            (*iterBead1)->m_Force[i] += newForce1[i];
					        (*iterBead2)->m_Force[i] += newForce2[i];
                        }
                       
						// stress tensor summation

						(*iterBead1)->m_Stress[0] += dx[0]*newForce1[0];
						(*iterBead1)->m_Stress[1] += dx[1]*newForce1[0];
						(*iterBead1)->m_Stress[2] += dx[2]*newForce1[0];
						(*iterBead1)->m_Stress[3] += dx[0]*newForce1[1];
						(*iterBead1)->m_Stress[4] += dx[1]*newForce1[1];
						(*iterBead1)->m_Stress[5] += dx[2]*newForce1[1];
						(*iterBead1)->m_Stress[6] += dx[0]*newForce1[2];
						(*iterBead1)->m_Stress[7] += dx[1]*newForce1[2];
						(*iterBead1)->m_Stress[8] += dx[2]*newForce1[2];

						// Pass the stress tensor components to the CMonitor
						// for use in analysing the stress over slices.
						// Note that dx[2] might be close to zero and we should
						// check for this.

					    // 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
						m_pMonitor->AddBeadStress(*iterBead1, *iterBead2, newForce1, dx);
#endif
					}
					else
					{
						TraceInt("neighbour cell bead", (*iterBead1)->GetId());
						TraceInt("interacts with", (*iterBead2)->GetId());
						TraceVector("1st pos",	(*iterBead1)->GetXPos(),
												(*iterBead1)->GetYPos(), 
												(*iterBead1)->GetZPos() );
						TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
												(*iterBead2)->GetYPos(), 
												(*iterBead2)->GetZPos() );
						TraceDouble("Bead distance", dr);
					}
				}
			}
		}
	}

#else
    // DPD and MD equations of motion

	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3], dv[3], newForce[3];
	double localStress[9];
	double dr, dr2;


#if SimIdentifier == DPD

	double gammap, rdotv, wr, wr2;
	double conForce, dissForce, randForce;

#ifdef UseDPDBeadRadii
	double drmax;
#endif

#elif SimIdentifier == MD

	double magLJ;						// LJ potential variables
	double eLJ, sLJOverR, sLJR3, sLJR6;

	double magSC;						// SC potential variables
	double eSC, sSCOverR, sSCR3;

#endif

//	if( m_lBeads.size() > 0 )
//	{
//		TraceInt2("UpdateForce: Cell No has beads with ids:", m_id, m_lBeads.size());
//		for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++)
//		{
//			TraceIntNoEndl((*iterBead1)->m_id);
//		}
//		Trace("");
//	}

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		// Zero the stress tensor for this bead. Notice that because of the
		// way we only sum over beads distinct from the current bead in the same
		// cell, and over beads in cells to the right and above the current cell
		// we don't have to worry about double counting the pairwise forces. We
		// can hence store the N(N-1)/2 contributions to the stress tensor in the
		// beads that form the first ones accessed in the double loops.

		for(short int j=0; j<9; j++)
		{
			(*iterBead1)->m_Stress[j] = 0.0;
		}

		for( riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=(*iterBead1)->m_id; ++riterBead2 )
		{
			dx[0] = ((*iterBead1)->m_Pos[0] - (*riterBead2)->m_Pos[0]);
			dv[0] = ((*iterBead1)->m_Mom[0] - (*riterBead2)->m_Mom[0]);

			dx[1] = ((*iterBead1)->m_Pos[1] - (*riterBead2)->m_Pos[1]);
			dv[1] = ((*iterBead1)->m_Mom[1] - (*riterBead2)->m_Mom[1]);

#if SimDimension == 2
			dx[2] = 0.0;
			dv[2] = 0.0;
#elif SimDimension == 3
			dx[2] = ((*iterBead1)->m_Pos[2] - (*riterBead2)->m_Pos[2]);
			dv[2] = ((*iterBead1)->m_Mom[2] - (*riterBead2)->m_Mom[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we use the flag UseDPDBeadRadii to determine whether
// the interaction radius is bead-specific or not. Note that when we use the
// interaction radius we have to compare the actual distance between beads
// not its square!

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
			if( dr2 < 1.0 )
			{		
				dr = sqrt(dr2);
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr);
					wr2 = wr*wr;
#else
			dr = sqrt(dr2);
			drmax = (*iterBead1)->GetRadius()+(*riterBead2)->GetRadius();
			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = wr*wr;
#endif

// Conservative force magnitude

					conForce  = m_vvConsInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*wr;				
//				    conForce = 0.0;		

// Dissipative and random force magnitudes. Note dr factor in newForce calculation

					rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
					gammap		= m_vvDissInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*wr2;

					dissForce	= -gammap*rdotv;				
					randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());
// Gauss RNG		randForce	= 0.288675*sqrt(gammap)*CCNTCell::m_invrootdt*CCNTCell::Gasdev();

					newForce[0] = (conForce + dissForce + randForce)*dx[0]/dr;
					newForce[1] = (conForce + dissForce + randForce)*dx[1]/dr;
					newForce[2] = (conForce + dissForce + randForce)*dx[2]/dr;

#elif SimIdentifier == MD

			if( dr2 < m_coradius2 )
			{		
				dr = sqrt(dr2);

				if( dr > 0.000000001 )
				{
					// Calculate common factors in the LJ potential and include shifted force.
					// Note that the shifted force term needs to be multiplied by the 
					// unit vector in the bead-bead separation whereas the force term 
					// has the full vector.

					eLJ		  = m_vvLJDepth.at((*iterBead1)->GetType()).at((*riterBead2)->GetType());	
					sLJOverR  = m_vvLJRange.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())/dr;
					sLJR3     = sLJOverR*sLJOverR*sLJOverR;
					sLJR6     = sLJR3*sLJR3;

					magLJ = (6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0) 
						     - dr*m_vvLJSlope.at((*iterBead1)->GetType()).at((*riterBead2)->GetType()))/dr2;

					// Only add the soft-core potential if the potential depth is non-zero

					eSC = m_vvSCDepth.at((*iterBead1)->GetType()).at((*riterBead2)->GetType());	

					if(eSC > 0.0)
					{
						sSCOverR = m_vvSCRange.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())/dr;
						sSCR3    = sSCOverR*sSCOverR*sSCOverR;
						
						magSC = (9.0*eSC*sSCR3*sSCR3*sSCR3
							     - dr*m_vvSCSlope.at((*iterBead1)->GetType()).at((*riterBead2)->GetType()))/dr2;
					}
					else
					{
						magSC = 0.0;
					}

					newForce[0] = (magLJ + magSC)*dx[0];
					newForce[1] = (magLJ + magSC)*dx[1];
					newForce[2] = (magLJ + magSC)*dx[2];
#endif

					(*iterBead1)->m_ForceCounter++;
					(*riterBead2)->m_ForceCounter++;
					
					(*iterBead1)->m_Force[0] += newForce[0];
					(*iterBead1)->m_Force[1] += newForce[1];
					(*iterBead1)->m_Force[2] += newForce[2];

					(*riterBead2)->m_Force[0] -= newForce[0];
					(*riterBead2)->m_Force[1] -= newForce[1];
					(*riterBead2)->m_Force[2] -= newForce[2];

					// stress tensor summation

					localStress[0] = dx[0]*newForce[0];
					localStress[1] = dx[1]*newForce[0];
					localStress[2] = dx[2]*newForce[0];
					localStress[3] = dx[0]*newForce[1];
					localStress[4] = dx[1]*newForce[1];
					localStress[5] = dx[2]*newForce[1];
					localStress[6] = dx[0]*newForce[2];
					localStress[7] = dx[1]*newForce[2];
					localStress[8] = dx[2]*newForce[2];
					
					(*iterBead1)->m_Stress[0] += localStress[0];
					(*iterBead1)->m_Stress[1] += localStress[1];
					(*iterBead1)->m_Stress[2] += localStress[2];
					(*iterBead1)->m_Stress[3] += localStress[3];
					(*iterBead1)->m_Stress[4] += localStress[4];
					(*iterBead1)->m_Stress[5] += localStress[5];
					(*iterBead1)->m_Stress[6] += localStress[6];
					(*iterBead1)->m_Stress[7] += localStress[7];
					(*iterBead1)->m_Stress[8] += localStress[8];

					// Pass the stress tensor components to the CMonitor
					// for use in analysing the stress over slices.
					// Note that dx[2] might be close to zero and we should
					// check for this.

					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
					m_pMonitor->AddBeadStress(*iterBead1, *riterBead2, newForce, dx);
#endif

#if EnableStressTensorSphere == SimMiscEnabled
                    // 11/05/10 Store the bead-bead stress in curvilinear coordinates in the two cells containing the interacting pair: same-cell beads so 
					// no other cells are involved. 
					// 23/05/10 We double-count the stress to compensate for the case where the beads are in different cells and the stress is added to both
					// 26/05/10 I removed the double-counting now that I have implemented the apportioning of the stress along the vector joining the two particles.
					// 18/06/10 I changed the stress storage from CNT cells into stress grid cells that are smaller than the CNT cells.
					
					m_pISimBox->AddBeadStress((*iterBead1)->m_Pos, (*riterBead2)->m_Pos, localStress);
#endif
				}
				else
				{
					TraceInt("same cell bead", (*iterBead1)->GetId());
					TraceInt("interacts with", (*riterBead2)->GetId());
					TraceVector("1st pos",	(*iterBead1)->GetXPos(),
											(*iterBead1)->GetYPos(), 
											(*iterBead1)->GetZPos() );
					TraceVector("2nd pos",	(*riterBead2)->GetXPos(),
											(*riterBead2)->GetYPos(), 
											(*riterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
/*		
		    if(xxParallelBase::GlobalGetRank() == 0)
			{
			    if(dynamic_cast<CExternalCNTCell*>(m_aIntNNCells[i]))
				{
		            std::cout << "Calculating External  cell-cell forces for " << GetId() << " " << m_aIntNNCells[i]->GetId() << zEndl;
				}
				else
				{
		            std::cout << "Calculating Internal  cell-cell forces for " << GetId() << " " << m_aIntNNCells[i]->GetId() << zEndl;
				}
			}
*/			
			localCellCellCounter++;  // Increment the local cell-cell inteaction counter
			
			for( iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++ )
			{
				dx[0] = ((*iterBead1)->m_Pos[0] - (*iterBead2)->m_Pos[0]);
				dv[0] = ((*iterBead1)->m_Mom[0] - (*iterBead2)->m_Mom[0]);

				dx[1] = ((*iterBead1)->m_Pos[1] - (*iterBead2)->m_Pos[1]);
				dv[1] = ((*iterBead1)->m_Mom[1] - (*iterBead2)->m_Mom[1]);

#if SimDimension == 2
				dx[2] = 0.0;
				dv[2] = 0.0;
#elif SimDimension == 3
				dx[2] = ((*iterBead1)->m_Pos[2] - (*iterBead2)->m_Pos[2]);
				dv[2] = ((*iterBead1)->m_Mom[2] - (*iterBead2)->m_Mom[2]);
#endif

				if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
				{
					if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
					else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

					if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
					else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
					if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
					else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

				}

				dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we have to change the distance-dependence according
// to the bead interaction radii.

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
				if( dr2 < 1.0 )
				{		
					dr = sqrt(dr2);
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr);
						wr2 = wr*wr;
#else
				dr = sqrt(dr2);
				drmax = (*iterBead1)->GetRadius()+(*iterBead2)->GetRadius();
				if( dr < drmax )
				{		
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr/drmax);
						wr2 = wr*wr;
#endif
						conForce	= m_vvConsInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr;				
//                        conForce = 0.0;

						rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
						gammap		= m_vvDissInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr2;

						dissForce	= -gammap*rdotv;				
						randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());
// Gauss RNG		    randForce	= 0.288675*sqrt(gammap)*CCNTCell::m_invrootdt*CCNTCell::Gasdev();

						newForce[0] = (conForce + dissForce + randForce)*dx[0]/dr;
						newForce[1] = (conForce + dissForce + randForce)*dx[1]/dr;
						newForce[2] = (conForce + dissForce + randForce)*dx[2]/dr;

#elif SimIdentifier == MD

				if( dr2 < m_coradius2 )
				{		
					dr = sqrt(dr2);

					if( dr > 0.000000001 )
					{
						// Calculate common factors in the LJ potential and include shifted force.
						// Note that the shifted force term needs to be multiplied by the 
						// unit vector in the bead-bead separation whereas the force term 
						// has the full vector.

						eLJ       = m_vvLJDepth.at((*iterBead1)->GetType()).at((*riterBead2)->GetType());	
						sLJOverR  = m_vvLJRange.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())/dr;
						sLJR3     = sLJOverR*sLJOverR*sLJOverR;
						sLJR6     = sLJR3*sLJR3;

						magLJ = (6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0) 
								 - dr*m_vvLJSlope.at((*iterBead1)->GetType()).at((*riterBead2)->GetType()))/dr2;

						// Only add the soft-core potential if the potential depth is non-zero

						eSC = m_vvSCDepth.at((*iterBead1)->GetType()).at((*iterBead2)->GetType());	

						if(eSC > 0.0)
						{
							sSCOverR = m_vvSCRange.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())/dr;
							sSCR3    = sSCOverR*sSCOverR*sSCOverR;

							magSC = (9.0*eSC*sSCR3*sSCR3*sSCR3
									 - dr*m_vvSCSlope.at((*iterBead1)->GetType()).at((*riterBead2)->GetType()))/dr2;
						}
						else
						{
							magSC = 0.0;
						}

						newForce[0] = (magLJ + magSC)*dx[0];
						newForce[1] = (magLJ + magSC)*dx[1];
						newForce[2] = (magLJ + magSC)*dx[2];
#endif

//                if(((*iterBead1)->GetId() == targetBeadId || (*iterBead2)->GetId() == targetBeadId))
//				{
//				    std::cout << "CNTCell " << GetId() << " calculating internal force with NN cell " << m_aIntNNCells[i]->GetId() << " for its beads " << (*iterBead1)->GetId() << " " << (*iterBead2)->GetId() << zEndl;
//				}

					    (*iterBead1)->m_ForceCounter++;
					    (*iterBead2)->m_ForceCounter++;
						
						(*iterBead1)->m_Force[0] += newForce[0];
						(*iterBead1)->m_Force[1] += newForce[1];
						(*iterBead1)->m_Force[2] += newForce[2];

						(*iterBead2)->m_Force[0] -= newForce[0];
						(*iterBead2)->m_Force[1] -= newForce[1];
						(*iterBead2)->m_Force[2] -= newForce[2];

						// stress tensor summation

					    localStress[0] = dx[0]*newForce[0];
					    localStress[1] = dx[1]*newForce[0];
					    localStress[2] = dx[2]*newForce[0];
					    localStress[3] = dx[0]*newForce[1];
					    localStress[4] = dx[1]*newForce[1];
					    localStress[5] = dx[2]*newForce[1];
					    localStress[6] = dx[0]*newForce[2];
					    localStress[7] = dx[1]*newForce[2];
					    localStress[8] = dx[2]*newForce[2];

						(*iterBead1)->m_Stress[0] += localStress[0];
						(*iterBead1)->m_Stress[1] += localStress[1];
						(*iterBead1)->m_Stress[2] += localStress[2];
						(*iterBead1)->m_Stress[3] += localStress[3];
						(*iterBead1)->m_Stress[4] += localStress[4];
						(*iterBead1)->m_Stress[5] += localStress[5];
						(*iterBead1)->m_Stress[6] += localStress[6];
						(*iterBead1)->m_Stress[7] += localStress[7];
						(*iterBead1)->m_Stress[8] += localStress[8];

						// Pass the stress tensor components to the CMonitor
						// for use in analysing the stress over slices.
						// Note that dx[2] might be close to zero and we should
						// check for this.

					    // 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
						m_pMonitor->AddBeadStress(*iterBead1, *iterBead2, newForce, dx);
#endif

#if EnableStressTensorSphere == SimMiscEnabled
                    // 11/05/10 Store the bead-bead stress in curvilinear coordinates in each cell containing the interacting pair: neighbour-cell beads so 
					// we have to add it to both cells and any intermediate cells traversed by the interaction vector.
					// 26/05/10  I implemented the proper apportioning of the stress to each cell along the vector joining the two particles.
					// 18/06/10  I changed the implementation of the stress calculation
					
					m_pISimBox->AddBeadStress((*iterBead1)->m_Pos, (*iterBead2)->m_Pos, localStress);
#endif
					}
					else
					{
						TraceInt("neighbour cell bead", (*iterBead1)->GetId());
						TraceInt("interacts with", (*iterBead2)->GetId());
						TraceVector("1st pos",	(*iterBead1)->GetXPos(),
												(*iterBead1)->GetYPos(), 
												(*iterBead1)->GetZPos() );
						TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
												(*iterBead2)->GetYPos(), 
												(*iterBead2)->GetZPos() );
						TraceDouble("Bead distance", dr);
					}
				}
			}
		}
	}

   // Divide the local cell-cell counter by the number of beads in this cell and add the result to the global cell-cell counter
	
	if(m_lBeads.size() > 0)
	{
	    localCellCellCounter /= m_lBeads.size();
	}
	else
	{
	    localCellCellCounter = 13;
	}
	
    mpsSimBox::GlobalCellCellIntCounter += localCellCellCounter;


#endif
}

// Function to update the position and intermediate velocity of the beads 
// from the old values for position, velocity and force. 
// Because the BD equation of motion is quite different from those of DPD and MD,
// we conditionally compile it in only for a BD simulation. The treatment of the
// PBC's and beads moving from cell to cell is the same for all simulation types.
// 
// This function only changes single bead properties,i.e., 
// the update equation for the coordinates of a bead only depend on 
// properties of that bead at the previous timestep. However, a complication
// arises because we loop over CNT cells and then over beads within the cells.
// If a bead crosses a cell boundary to another cell which comes later in the
// cell sequence it could be moved twice. We use an IsMovable flag to indicate if
// a bead should have its position changed in this routine. If the bead crosses 
// a cell boundary the flag is set to false, and only reset in the 
// CSimBox::UpdateMom() routine.
//
// Note that we also store the old velocity and force and zero the current
// force arrays for later use in CSimBox::UpdateForce() and CSimBox::UpdateMom().
// This is necessary because looping over CNT cells means we cannot zero the force
// on a bead when we are in any particular cell.
//
// Also note that the erase() function increments the iterator but if the bead
// does not change cell we have to increment it by hand.

void CCNTCell::UpdatePos()
{
//	if( m_lBeads.size() > 0 )
//	{
//		TraceInt2("UpdatePos: Cell No has beads:", GetId(), CellBeadTotal());
//		for( iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
//		{
//			TraceIntNoEndl((*iterBead)->GetId());
//			TraceIntNoEndl((*iterBead)->GetType());
//			TraceIntNoEndl((*iterBead)->GetMovable());
//			TraceDoubleNoEndl((*iterBead)->GetXPos());
//			TraceDoubleNoEndl((*iterBead)->GetYPos());
//			TraceDoubleNoEndl((*iterBead)->GetZPos());
//			TraceDoubleNoEndl((*iterBead)->GetunPBCXPos());
//			TraceDoubleNoEndl((*iterBead)->GetunPBCYPos());
//			TraceDoubleNoEndl((*iterBead)->GetunPBCZPos());
//			TraceDoubleNoEndl((*iterBead)->m_Mom[0]);
//			TraceDoubleNoEndl((*iterBead)->m_Mom[1]);
//			TraceDoubleNoEndl((*iterBead)->m_Mom[2]);
//			TraceDoubleNoEndl((*iterBead)->m_Force[0]);
//			TraceDoubleNoEndl((*iterBead)->m_Force[1]);
//			TraceDoubleNoEndl((*iterBead)->m_Force[2]);
//			TraceEndl();
//		}
//		TraceEndl();
//	}

	// Note that absence of an increment step here. If a bead changes cells the
	// iterator is incremented by the erase() function that removes it from 
	// its original cell; if no change occurs we increment the iterator by hand.
	// However, the Cray list<>::erase function does not return the next iterator
	// so we have to provide a method for incrementing it manually.

#if Platform == CRAYJ90
	BeadListIterator nextBead = NULL;
#endif

	double dx[3];

	for(BeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); )
	{
		// Only allow bead to move if its IsMovable flag is true. This allows
		// us to indicate when a bead has already crossed a cell boundary and
		// should not be moved again in this timestep.
		
		if((*iterBead)->GetMovable())
		{
#if SimIdentifier == BD
            // BD simulations
			// Store current values of position and force for later use. Note that
            // the velocity of the Brownian particles is identically zero.

			(*iterBead)->m_oldPos[0] = (*iterBead)->m_Pos[0];
			(*iterBead)->m_oldPos[1] = (*iterBead)->m_Pos[1];
			(*iterBead)->m_oldPos[2] = (*iterBead)->m_Pos[2];

			(*iterBead)->m_oldForce[0] = (*iterBead)->m_Force[0];
			(*iterBead)->m_oldForce[1] = (*iterBead)->m_Force[1];
			(*iterBead)->m_oldForce[2] = (*iterBead)->m_Force[2];

	
			// Update position coordinates. Generate a Gaussian-distributed
            // random displacement for each bead

			dx[0] = m_dtoverkt*(*iterBead)->m_Force[0] + m_dispmag*Gasdev();
			dx[1] = m_dtoverkt*(*iterBead)->m_Force[1] + m_dispmag*Gasdev();
			dx[2] = m_dtoverkt*(*iterBead)->m_Force[2] + m_dispmag*Gasdev();

//          dx[0] = m_dtoverkt*(*iterBead)->m_Force[0] + m_dispmag*GetExternalRandomNumber();
//			dx[1] = m_dtoverkt*(*iterBead)->m_Force[1] + m_dispmag*GetExternalRandomNumber();
//			dx[2] = m_dtoverkt*(*iterBead)->m_Force[2] + m_dispmag*GetExternalRandomNumber();
 
//          dx[0] = m_dtoverkt*(*iterBead)->m_Force[0] + 0.1*m_dispmag;
//			dx[1] = m_dtoverkt*(*iterBead)->m_Force[1] + 0.1*m_dispmag;
//			dx[2] = m_dtoverkt*(*iterBead)->m_Force[2] + 0.1*m_dispmag;

			(*iterBead)->m_Pos[0] += dx[0];
			(*iterBead)->m_Pos[1] += dx[1];
			(*iterBead)->m_Pos[2] += dx[2];

//            std::cout << m_dtoverkt << "  " << m_dispmag << zEndl;
//            std::cout << "Bead id " << (*iterBead)->GetId() << zEndl;
//            std::cout << "cons forces " << (*iterBead)->m_Force[0] << "  " <<(*iterBead)->m_Force[1] << "  " <<(*iterBead)->m_Force[2] << zEndl; 
//            std::cout << "old pos     " << (*iterBead)->m_oldPos[0] << "  " << (*iterBead)->m_oldPos[1] << "  " << (*iterBead)->m_oldPos[2] << zEndl; 
//            std::cout << "dx[]        " << dx[0] << "  " << dx[1] << "  " <<dx[2] << zEndl; 
//            std::cout << "new pos     " << (*iterBead)->GetXPos() << "  " << (*iterBead)->GetYPos() << "  " << (*iterBead)->GetZPos() << zEndl; 

			// Update the unPBC coordinates for use in calculating bond lengths
			// where we don't want to have to check for beads at opposite side
			// of the simulation box.

			(*iterBead)->m_unPBCPos[0] += dx[0];
			(*iterBead)->m_unPBCPos[1] += dx[1];
			(*iterBead)->m_unPBCPos[2] += dx[2];

			// Store position increments for later ease of use

			(*iterBead)->m_dPos[0] = dx[0];
			(*iterBead)->m_dPos[1] = dx[1];
			(*iterBead)->m_dPos[2] = dx[2];

			// Zero current force on beads so that UpdateForce() just has to form
			// a sum of all bead-bead interactions
			
			(*iterBead)->m_Force[0] = 0.0;
			(*iterBead)->m_Force[1] = 0.0;
			(*iterBead)->m_Force[2] = 0.0;

#else
            // DPD and MD simulations
			// Store current values of position, velocity and force for later use

			(*iterBead)->m_oldPos[0] = (*iterBead)->m_Pos[0];
			(*iterBead)->m_oldPos[1] = (*iterBead)->m_Pos[1];
			(*iterBead)->m_oldPos[2] = (*iterBead)->m_Pos[2];

			(*iterBead)->m_oldMom[0] = (*iterBead)->m_Mom[0];
			(*iterBead)->m_oldMom[1] = (*iterBead)->m_Mom[1];
			(*iterBead)->m_oldMom[2] = (*iterBead)->m_Mom[2];

			(*iterBead)->m_oldForce[0] = (*iterBead)->m_Force[0];
			(*iterBead)->m_oldForce[1] = (*iterBead)->m_Force[1];
			(*iterBead)->m_oldForce[2] = (*iterBead)->m_Force[2];
	
			// Update position coordinates

			dx[0] = m_dt*(*iterBead)->m_Mom[0] + m_halfdt2*(*iterBead)->m_Force[0];
			dx[1] = m_dt*(*iterBead)->m_Mom[1] + m_halfdt2*(*iterBead)->m_Force[1];
			dx[2] = m_dt*(*iterBead)->m_Mom[2] + m_halfdt2*(*iterBead)->m_Force[2];

			(*iterBead)->m_Pos[0] += dx[0];
			(*iterBead)->m_Pos[1] += dx[1];
			(*iterBead)->m_Pos[2] += dx[2];

			// Update the unPBC coordinates for use in calculating bond lengths
			// where we don't want to have to check for beads at opposite side
			// of the simulation box.

			(*iterBead)->m_unPBCPos[0] += dx[0];
			(*iterBead)->m_unPBCPos[1] += dx[1];
			(*iterBead)->m_unPBCPos[2] += dx[2];

			// Store position increments for later ease of use

			(*iterBead)->m_dPos[0] = dx[0];
			(*iterBead)->m_dPos[1] = dx[1];
			(*iterBead)->m_dPos[2] = dx[2];

			// Update intermediate velocity

			(*iterBead)->m_Mom[0] = (*iterBead)->m_oldMom[0] + m_lamdt*(*iterBead)->m_oldForce[0];
			(*iterBead)->m_Mom[1] = (*iterBead)->m_oldMom[1] + m_lamdt*(*iterBead)->m_oldForce[1];
			(*iterBead)->m_Mom[2] = (*iterBead)->m_oldMom[2] + m_lamdt*(*iterBead)->m_oldForce[2];

			// Zero current force on beads so that UpdateForce() just has to form
			// a sum of all bead-bead interactions

			(*iterBead)->m_ForceCounter = 0;

			(*iterBead)->m_Force[0] = 0.0;
			(*iterBead)->m_Force[1] = 0.0;
			(*iterBead)->m_Force[2] = 0.0;
#endif

// **********************************************************************
// Check to see if the a bead moves from the current CNT cell to a 
// neighbouring one. If not do not bother with PBCs, but if it does, 
// and both the original cell and the new one are external, then apply PBCs.
// We use the information on which direction the bead moved to
// limit the number of tests to do when applying the pbcs.
// Notice that we use the push_front() member function so that
// we can access the bead in the new cell (if it moved) by front().

			if( (*iterBead)->m_Pos[0] > m_TRCoord[0] )
			{
				if( (*iterBead)->m_Pos[1] > m_TRCoord[1] )	
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UTR
					{
						if(m_bExternal && m_aNNCells[26]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[26]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DTR
					{
						if(m_bExternal && m_aNNCells[8]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[8]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves TR
					{
						if(m_bExternal && m_aNNCells[17]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[17]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[8]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
							(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
						if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
							(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[8]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else if( (*iterBead)->m_Pos[1] < m_BLCoord[1] )
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UBR
					{
						if(m_bExternal && m_aNNCells[20]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[20]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DBR
					{
						if(m_bExternal && m_aNNCells[2]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[2]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves BR
					{
						if(m_bExternal && m_aNNCells[11]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[11]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[2]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
							(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
						if( (*iterBead)->m_Pos[1] < 0.0 )
							(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
					}
						(*iterBead)->SetNotMovable();
					m_aNNCells[2]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else	// no change in Y direction
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UR
					{
						if(m_bExternal && m_aNNCells[23]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[23]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DR
					{
						if(m_bExternal && m_aNNCells[5]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[5]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves R
					{
						if(m_bExternal && m_aNNCells[14]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
								(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[14]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[5]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] > m_SimBoxXLength )
							(*iterBead)->m_Pos[0]-= m_SimBoxXLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[5]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
			}
			else if( (*iterBead)->m_Pos[0] < m_BLCoord[0] )	
			{
				if( (*iterBead)->m_Pos[1] > m_TRCoord[1] )	
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UTL
					{
						if(m_bExternal && m_aNNCells[24]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[24]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DTL
					{
						if(m_bExternal && m_aNNCells[6]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[6]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves TL
					{
						if(m_bExternal && m_aNNCells[15]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[15]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[6]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] < 0.0 )
							(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
						if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
							(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[6]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else if( (*iterBead)->m_Pos[1] < m_BLCoord[1] )	
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UBL
					{
						if(m_bExternal && m_aNNCells[18]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[18]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DBL
					{
						if(m_bExternal && m_aNNCells[0]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[0]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves BL
					{
						if(m_bExternal && m_aNNCells[9]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[9]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[0]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] < 0.0 )
							(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
						if( (*iterBead)->m_Pos[1] < 0.0 )
							(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[0]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else	// no change in Y direction
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UL
					{
						if(m_bExternal && m_aNNCells[21]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[21]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DL
					{
						if(m_bExternal && m_aNNCells[3]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[3]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves L
					{
						if(m_bExternal && m_aNNCells[12]->IsExternal())
						{
							if( (*iterBead)->m_Pos[0] < 0.0 )
								(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[12]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[3]->IsExternal())
					{
						if( (*iterBead)->m_Pos[0] < 0.0 )
							(*iterBead)->m_Pos[0]+= m_SimBoxXLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[3]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
			}
			else	// no change in x direction
			{
				if( (*iterBead)->m_Pos[1] > m_TRCoord[1] )
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UT
					{
						if(m_bExternal && m_aNNCells[25]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[25]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DT
					{
						if(m_bExternal && m_aNNCells[7]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[7]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves T
					{
						if(m_bExternal && m_aNNCells[16]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
								(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[16]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[7]->IsExternal())
					{
						if( (*iterBead)->m_Pos[1] > m_SimBoxYLength )
							(*iterBead)->m_Pos[1]-= m_SimBoxYLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[7]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else if( (*iterBead)->m_Pos[1] < m_BLCoord[1] )
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves UB
					{
						if(m_bExternal && m_aNNCells[19]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[19]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves DB
					{
						if(m_bExternal && m_aNNCells[1]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[1]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else	// bead moves B
					{
						if(m_bExternal && m_aNNCells[10]->IsExternal())
						{
							if( (*iterBead)->m_Pos[1] < 0.0 )
								(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[10]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
#elif SimDimension == 2
					if(m_bExternal && m_aNNCells[1]->IsExternal())
					{
						if( (*iterBead)->m_Pos[1] < 0.0 )
							(*iterBead)->m_Pos[1]+= m_SimBoxYLength;
					}
					(*iterBead)->SetNotMovable();
					m_aNNCells[1]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
#endif
				}
				else	// no change in X, Y directions
				{
#if SimDimension == 3
					if( (*iterBead)->m_Pos[2] > m_TRCoord[2] )	// bead moves U
					{
						if(m_bExternal && m_aNNCells[22]->IsExternal())
						{
							if( (*iterBead)->m_Pos[2] > m_SimBoxZLength )
								(*iterBead)->m_Pos[2]-= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[22]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else if( (*iterBead)->m_Pos[2] < m_BLCoord[2] )	// bead moves D
					{
						if(m_bExternal && m_aNNCells[4]->IsExternal())
						{
							if( (*iterBead)->m_Pos[2] < 0.0 )
								(*iterBead)->m_Pos[2]+= m_SimBoxZLength;
						}
						(*iterBead)->SetNotMovable();
						m_aNNCells[4]->m_lBeads.push_front((*iterBead));
#if Platform == CRAYJ90
						nextBead = iterBead;
						nextBead++;
						m_lBeads.erase(iterBead);
						iterBead = nextBead;
#else
						iterBead = m_lBeads.erase(iterBead);
#endif
					}
					else
					{
						// If the bead did not change cells increment the
						// iterator by hand.
						
						(*iterBead)->SetNotMovable();
						iterBead++;	
					}
#elif SimDimension == 2
					(*iterBead)->SetNotMovable();
					iterBead++;
#endif
				}
			}
		}
		else
		{
			iterBead++;	// if the bead is not movable increment the iterator
		}
// **********************************************************************
	}
}

// Function to update the velocity of the beads using the old
// velocity and the old and new values for the force. Note that this
// function replaces the intermediate velocity (which is stored in
// m_Mom[]) with the new value. The intermediate value is only needed
// in the force calculation done in UpdateForce(). 
//
// This routine tells all beads derived from CAbstractBead to reset their 
// movable flag using a virtual function that allows each derived class to 
// override it. The CWallBead class, for example, ignores the setting because
// wall beads cannot move. Beads that have been frozen also ignore the command
// because the CBead::SetMovable() function takes the state of the m_bIsFrozen
// flag into account when checking if a bead is movable.
//
// The angular momentum of the beads is also stored to check that it vanishes
// on the average throughout the simulation.

void CCNTCell::UpdateMom()
{
	for( BeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++ )
	{
// Conditionally compile out the update of the momenta for those simulation types
// that do not use them; but make sure the beads' SetMovable() function is called.

#if SimIdentifier == BD
        (*iterBead)->SetMovable();
#else
		if((*iterBead)->SetMovable())	// flag ignored by immovable beads
		{
			(*iterBead)->m_Mom[0] = (*iterBead)->m_oldMom[0] + 
									 m_halfdt*((*iterBead)->m_Force[0] + (*iterBead)->m_oldForce[0]);
			(*iterBead)->m_Mom[1] = (*iterBead)->m_oldMom[1] + 
									 m_halfdt*((*iterBead)->m_Force[1] + (*iterBead)->m_oldForce[1]);
			(*iterBead)->m_Mom[2] = (*iterBead)->m_oldMom[2] + 
									 m_halfdt*((*iterBead)->m_Force[2] + (*iterBead)->m_oldForce[2]);		

			(*iterBead)->m_AngMom[0]	= (*iterBead)->m_Pos[1]*(*iterBead)->m_Mom[2] - 
										  (*iterBead)->m_Pos[2]*(*iterBead)->m_Mom[1];

			(*iterBead)->m_AngMom[1]	= -(*iterBead)->m_Pos[0]*(*iterBead)->m_Mom[2] + 
										   (*iterBead)->m_Pos[2]*(*iterBead)->m_Mom[0];

			(*iterBead)->m_AngMom[2]	= (*iterBead)->m_Pos[0]*(*iterBead)->m_Mom[1] - 
										  (*iterBead)->m_Pos[1]*(*iterBead)->m_Mom[0];
		}
#endif
	}
}

// Function to map the index of a nearest-neighbour cell to a pointer
// to the cell. Notice that the current cell is one of the possible 
// values.
//
void CCNTCell::SetNNCellIndex(long index, CCNTCell *pCell)
{

#if SimDimension == 2
	if( index >=0 && index < 9 )
#elif SimDimension == 3
	if( index >= 0 && index < 27 )
#endif
	{
// map cell index to cell pointer using an array
		m_aNNCells[index] = pCell;
// map cell index to cell pointer using STL
//		m_mNNCells.insert( make_pair(index, pCell) );
	}
	else
	{
		TraceInt("ERROR: Illegal index for NN cell mapping", index );
	}

}

// Function to add a bead to the current cell.

void CCNTCell::AddBeadtoCell(CAbstractBead *pBead)
{
	m_lBeads.push_front(pBead);
}

// Function to remove a bead from the current cell.

void CCNTCell::RemoveBeadFromCell(CAbstractBead* const pBead)
{
	m_lBeads.remove(pBead);
}

// Function to remove all beads from the current cell. Note that this does NOT
// delete the beads, it only removes their pointers from the cell.

void CCNTCell::RemoveAllBeadsFromCell()
{
	m_lBeads.clear();
}

long CCNTCell::CellBeadTotal() const
{
	return m_lBeads.size();
}

// Function to store a pointer to a nearest-neighbour cell used in 
// calculating the interactions between beads. Because the forces are
// all pairwise we only have to loop over bead pairs once. Hence we store
// pointers to those cells that are to the right of, or above the current cell.


void CCNTCell::SetIntNNCellIndex( long index, CCNTCell* pCell )
{

#if SimDimension == 2
	if( index >=0 && index < 4 )
#elif SimDimension == 3
	if( index >= 0 && index < 13 )
#endif
	{
// map cell index to cell pointer using an array
		m_aIntNNCells[index] = pCell;
	}
	else
	{
		TraceInt("ERROR: Illegal index for NN cell mapping", index );
	}

}

// Function to return random number from a uniform generator. Note that 
// Randf() alters some static variables inside itself. Also this is only 
// used by other objects not within the main simulation loop. It is provided 
// to avoid the calling routine from having to pass in a seed.

double CCNTCell::GetRandomNo()
{
	return CCNTCell::Randf();
}

// Function to return an exponentially-distributed random number.

double CCNTCell::GetExponentialRandomNo()
{
	return CCNTCell::Expdev();
}

// Function to return a Gaussian-distributed random number.

double CCNTCell::GetGaussRandomNo()
{
	return CCNTCell::Gasdev();
}

// Private helper function to return a "random" number that is read in from 
// an external file. This is useful for testing the integration scheme by 
// forcing it to use a pre-defined sequence of numbers. The numbers are read in 
// using the CRandomNumberSequence class that handles all the file IO. 
// The numbers are stored in the containing instance for later use.

double CCNTCell::GetExternalRandomNumber()
{
    // Ensure the numbers are only read in once
    if(m_bReadFileOnce)
    {
        m_bReadFileOnce = false;
        m_RandomNumbers.clear();

        CRandomNumberSequence file("RandomNumbers500.txt", 500);
        file.Serialize();

        for(long i=0; i<file.GetActualSequenceSize(); i++)
        {
            m_RandomNumbers.push_back(file.GetNextNumber());
        }
     }

    double x = 0.0;

    if(m_NextRNIndex == m_RandomNumbers.size())
    {
        m_NextRNIndex = 0;  // If we run out of numbers re-use the set.
    }

    x = m_RandomNumbers.at(m_NextRNIndex++);

//    std::cout << "random number = " << x << zEndl;

    return x;

}

// **********************************************************************
// Random number generator using a 64-bit lcg.
                                                
double CCNTCell::Randf()
{
    return static_cast<double>(CCNTCell::lcg(CCNTCell::m_RNGSeed))*CCNTCell::m_Inv2Power32;
}

// Private static helper function for the lcg RNG

uint32_t CCNTCell::lcg(uint64_t &state)
{
    state=6364136223846793005ull * state + 1;
    return state>>32; // Return the top bits, as they are the best
}
                                
// Gaussian random number generator.
// It returns a normally distributed deviate with zero mean and unit variance, using one of the above
// functions (Randf) as the source of uniform deviates.
//
// NOT IMPLEMENTED YET

double CCNTCell::Gasdev()
{
    return 0.0;
}

// Exponentially-distributed random number generator.
// It returns a value, y, that is distributed according to exp(-y) and has
// unit mean. It uses Randf() to supply a uniformly-distributed random number.

double CCNTCell::Expdev()
{
    double value;

    do{
        value = CCNTCell::Randf();
    } while (value == 0.0);

    return -log(value);
}

// Static member function to return a randomly-generated string. We provide 
// several overloads for this function that return strings of various types.
// If any of the required sub-strings are empty (ie, a blank space), a random
// string is generated to fill them. In this way, a completely random string
// can be created.
//
// Sample string containing 8 digits:  12345678

const zString CCNTCell::GetRandomString()
{
    double rfloat = CCNTCell::GetRandomNo();

    while(rfloat < 0.00000001)
    {
        rfloat = CCNTCell::GetRandomNo();
    }

    const zString rstring =  IGlobalSimBox::Instance()->ToString(static_cast<long>(1.e+08*rfloat));

    return rstring;
}

// String that has a user-defined prefix and a suffix separated by a user-defined
// separator character. If the prefix is empty or contains only spaces, we 
// generate a random string of letters (taken from m_AlphabetChars), and 
// if the separator contains only spaces we use the character specified 
// in the static member m_StringSeparator. But we allow the separator to be empty 
// as this just concatenates the prefix with the randomly-generated string.
//
// A sample string returned by this function is:
//
// Bead-12345678 where "Bead" is the prefix and "-" is the separator.

const zString CCNTCell::GetRandomString(const zString prefix, const zString separator)
{
    zString actualSeparator;

    if(separator.find_first_of(" ") < separator.length())
    {
        actualSeparator = CCNTCell::m_StringSeparator;
    }
    else
    {
        actualSeparator = separator;
    }

    zString actualPrefix;

    if(prefix.empty() || prefix.find_first_of(" ") < prefix.length())
    {
        actualPrefix = GetRandomAlphabeticString(CCNTCell::m_StringSize);
    }
    else
    {
        actualPrefix = prefix;
    }

    const zString rstring = actualPrefix + actualSeparator + GetRandomString();

    return rstring;
}

// String that has a user-defined prefix and an integer counter, and returns
// a random string of the form:
//
// e.g., Bead-12345678-1 where "-" is the separator and "1" is the counter
//
// Note that if  the prefix is empty, a random string is generated by the
// GetRandomString() overload above, but we have to check that the separator is
// not blank here as well as we use it explicitly.

const zString CCNTCell::GetRandomString(const zString prefix, const zString separator, long counter)
{
    zString actualSeparator;

    if(separator.find_first_of(" ") < separator.length())
    {
        actualSeparator = CCNTCell::m_StringSeparator;
    }
    else
    {
        actualSeparator = separator;
    }

    
    const zString rstring = GetRandomString(prefix, separator) + separator + IGlobalSimBox::Instance()->ToString(counter);
    return rstring;
}

// Static member function to return a random alphabetic string. This can be used
// to construct names for bead and polymer types which must begin with a letter.
// We just generate a random integer between 1 and 52 and return the corresponding
// upper or lowercase letter.
// The argument is the number of characters in the string; if this is zero,
// we use the number stored in the static member m_StringSize.

const zString CCNTCell::GetRandomAlphabeticString(long size)
{
    if(size < 1)
    {
        size = CCNTCell::m_StringSize;
    }

    zString rstring = "";

    const long charSet = m_AlphabetChars.size();

    for(long i=0; i<size; ++i)
    {
        double rfloat = GetRandomNo();
        long index = static_cast<long>(1.e+08*rfloat) % charSet;

        while(index < 0 || index >= charSet)
        {
            rfloat = GetRandomNo();
            index = static_cast<long>(1.e+08*rfloat) % charSet;
        }
        
        rstring += m_AlphabetChars.at(index);
    }

    return rstring;
}

// Function to check that all the beads in the current CNTCell have coordinates
// within the cell's boundaries.

bool CCNTCell::CheckBeadsinCell()
{
	bool bBeadsFound = true;

	long index1, ix, iy, iz;

	for(BeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
	{
		ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
#elif SimDimension == 3
		iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);		
#endif

		index1 = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

		if(index1 != GetId())
		{
			bBeadsFound = false;

			CAbstractBead* pBead = (*iterBead);

			// Post a warning message about the error. but only write to the screen
			// during debug. Note that the time returned by the Monitor may be incorrect
			// if the SamplePeriod is not 1, but it will be out by at most SamplePeriod.
			// This is because the Monitor only updates its current time when the
			// data is sampled.

			new CLogCNTBeadError(m_pMonitor->GetCurrentTime(), 
														  ix, iy, iz, index1, GetId(), pBead);

#ifdef TraceOn
			TraceInt2(  "Cell has beads", GetId(), m_lBeads.size());
			TraceInt2(  "  Bead has false cell index", (*iterBead)->GetId(), index1);
			TraceInt3(  "  Cell BL index",  m_BLIndex[0], m_BLIndex[1], m_BLIndex[2]);
			TraceVector("  Cell BL coords", m_BLCoord[0], m_BLCoord[1], m_BLCoord[2]);
			TraceVector("  Cell TR coords", m_TRCoord[0], m_TRCoord[1], m_TRCoord[2]);
			TraceVector("  Bead coords", (*iterBead)->GetXPos(), (*iterBead)->GetYPos(), (*iterBead)->GetZPos());

			for(BeadListIterator iterBead2=m_lBeads.begin(); iterBead2!=m_lBeads.end(); iterBead2++)
			{
				TraceInt("Bead", (*iterBead2)->GetId());
				TraceVector("  Pos",      (*iterBead2)->GetXPos(), (*iterBead2)->GetYPos(), (*iterBead2)->GetZPos());
				TraceVector("  old Pos",  (*iterBead2)->m_oldPos[0], (*iterBead2)->m_oldPos[1], (*iterBead2)->m_oldPos[2]);
				TraceVector("  Vel",      (*iterBead2)->GetXMom(), (*iterBead2)->GetYMom(), (*iterBead2)->GetZMom());
				TraceVector("  old Vel",  (*iterBead2)->m_oldMom[0], (*iterBead2)->m_oldMom[1], (*iterBead2)->m_oldMom[2]);
				TraceVector("  Force",    (*iterBead2)->m_Force[0], (*iterBead2)->m_Force[1], (*iterBead2)->m_Force[2]);
				TraceVector("  old Force",(*iterBead2)->m_oldForce[0], (*iterBead2)->m_oldForce[1], (*iterBead2)->m_oldForce[2]);

//				Trace("its NNCells are:");
//				for(long ic=0; ic<27; ic++)
//				{
//					TraceInt2("", ic, m_aNNCells[ic]->GetId()); 
//				}
				Trace("**********");
			}
#endif

		}
	}

	return bBeadsFound;
}

// Function to return the potential energy of a bead interacting with all the
// beads in the current cell and its neighbours. The bead itself must NOT be in 
// the cell's bead list as this will lead to self-interactions. The calling 
// routine must ensure that it is removed prior to calling this function. It 
// should also only be called in an MD simulation because it uses the LJ and 
// soft-core potential parameters,
//
// We use a method similar to the UpdateForce() algorithm to add the interactions
// that are within range of the potentials, but we calculate the PE not the force.
// The potentials are assumed to depend only on the current positions of the 
// particles.

double CCNTCell::GetPotentialEnergy(CAbstractBead *pBead) const
{
	double totalPE = 0.0;

#if SimIdentifier == MD

	double dx[3];
	double dr, dr2;

	double magLJ;						// LJ potential variables
	double eLJ, sLJOverR, sLJR3, sLJR6;

	double magSC;						// SC potential variables
	double eSC, sSCOverR, sSCR3;

	for(cBeadListIterator iterBead2=m_lBeads.begin(); iterBead2!=m_lBeads.end(); iterBead2++)
	{
		dx[0] = pBead->m_Pos[0] - (*iterBead2)->m_Pos[0];
		dx[1] = pBead->m_Pos[1] - (*iterBead2)->m_Pos[1];

#if SimDimension == 2
		dx[2] = 0.0;
#elif SimDimension == 3
		dx[2] = pBead->m_Pos[2] - (*iterBead2)->m_Pos[2];
#endif

		dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

		if( dr2 < m_coradius2 )
		{		
			dr = sqrt(dr2);

			if( dr > 0.000000001 )
			{
				// Calculate common factors in the LJ potential

				eLJ       = m_vvLJDepth.at(pBead->GetType()).at((*iterBead2)->GetType());	
				sLJOverR  = m_vvLJRange.at(pBead->GetType()).at((*iterBead2)->GetType())/dr;
				sLJR3	  = sLJOverR*sLJOverR*sLJOverR;
				sLJR6	  = sLJR3*sLJR3;
				magLJ	  = eLJ*sLJR6*(sLJR6 - 1.0);

				// Note that the cutoff radius has to be scaled by the first LJ potential
				// range as well as the other terms.

					double mype = (magLJ - m_vvLJDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
									  + m_vvLJSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));

				totalPE += (magLJ - m_vvLJDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
					              + m_vvLJSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));

				// Only add the soft-core potential if the potential depth is non-zero

				eSC = m_vvSCDepth.at(pBead->GetType()).at((*iterBead2)->GetType());	

				if(eSC > 0.0)
				{
					sSCOverR  = m_vvSCRange.at(pBead->GetType()).at((*iterBead2)->GetType())/dr;
					sSCR3     = sSCOverR*sSCOverR*sSCOverR;
					magSC     = eSC*sSCR3*sSCR3*sSCR3;

					double myscpe = (magSC - m_vvSCDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
									  + m_vvSCSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));

						
					totalPE += (magSC - m_vvSCDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
								  + m_vvSCSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));
				}		
			}
			else
			{
				TraceInt("same cell bead", pBead->GetId());
				TraceInt("interacts with", (*iterBead2)->GetId());
				TraceVector("1st pos",	pBead->GetXPos(),
										pBead->GetYPos(), 
										pBead->GetZPos() );
				TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
										(*iterBead2)->GetYPos(), 
										(*iterBead2)->GetZPos() );
				TraceDouble("Bead distance", dr);
			}
		}
	}

	// Next add in interactions with beads in neighbouring cells taking the
	// PBCs into account and the presence of a wall. The PBCs are only applied
	// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
	for( int i=0; i<4; i++ )
#elif SimDimension == 3
	for( int i=0; i<13; i++ )
#endif
	{
		for(cBeadListIterator iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++)
		{
			dx[0] = pBead->m_Pos[0] - (*iterBead2)->m_Pos[0];
			dx[1] = pBead->m_Pos[1] - (*iterBead2)->m_Pos[1];

#if SimDimension == 2
			dx[2] = 0.0;
#elif SimDimension == 3
			dx[2] = pBead->m_Pos[2] - (*iterBead2)->m_Pos[2];
#endif

			if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
			{
				if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
					dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
				else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
					dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

				if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
					dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
				else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
					dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
				if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
					dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
				else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
					dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

			}

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

			if( dr2 < m_coradius2 )
			{		
				dr = sqrt(dr2);

				if( dr > 0.000000001 )
				{
					// Calculate common factors in the LJ potential

					eLJ       = m_vvLJDepth.at(pBead->GetType()).at((*iterBead2)->GetType());	
					sLJOverR  = m_vvLJRange.at(pBead->GetType()).at((*iterBead2)->GetType())/dr;
					sLJR3	  = sLJOverR*sLJOverR*sLJOverR;
					sLJR6	  = sLJR3*sLJR3;
					magLJ	  = eLJ*sLJR6*(sLJR6 - 1.0);

					// Note that the cutoff radius has to be scaled by the first LJ potential
					// range as well as the other terms.

					totalPE += (magLJ - m_vvLJDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
									  + m_vvLJSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));
						
					// Only add the soft-core potential if the potential depth is non-zero

					eSC = m_vvSCDepth.at(pBead->GetType()).at((*iterBead2)->GetType());	

					if(eSC > 0.0)
					{
						sSCOverR  = m_vvSCRange.at(pBead->GetType()).at((*iterBead2)->GetType())/dr;
						sSCR3	  = sSCOverR*sSCOverR*sSCOverR;
						magSC	  = eSC*sSCR3*sSCR3*sSCR3;

						totalPE += (magSC - m_vvSCDelta.at(pBead->GetType()).at((*iterBead2)->GetType())
										  + m_vvSCSlope.at(pBead->GetType()).at((*iterBead2)->GetType())*(dr - m_cutoffradius));
					}
				}
				else
				{
					TraceInt("neighbour cell bead", pBead->GetId());
					TraceInt("interacts with", (*iterBead2)->GetId());
					TraceVector("1st pos",	pBead->GetXPos(),
											pBead->GetYPos(), 
											pBead->GetZPos() );
					TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
											(*iterBead2)->GetYPos(), 
											(*iterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}
	}

#endif

	return totalPE;
}

// Function to calculate the total kinetic energy and bead-bead potential energies 
// for all beads in a CNT cell. This involves bead pairs in the current cell and 
// adjacent cells. Bond and bondpair potential energies are not calculated here,
// but are calculated from within CMonitor::ZeroTotalEnergy(). Bead energy data 
// are not stored locally but passed directly to the CMonitor class for 
// output to the CHistoryState object. 
//
// The kinetic energy of beads in the current cell is obtained directly from
// the bead momenta, and their potential energy is obtained by iterating over all
// beads in the cell, adding their mutual interaction energies, and then over all 
// interactions between beads in the current cell and the neighbouring cells.
// Self-interactions of beads in the same cell are prevented by checking bead ids.
// Double-counting of bead pairs in adjacent CNT cells is prevented by only
// adding contributions from bead pairs in which the adjacent cell bead has an
// id higher than the current cell bead.
//
// NOTE. The function CMonitor::ZeroTotalEnergy() must be called before using this 
// routine as the energy is added to a running total in CMonitor::AddBeadEnergy().

void CCNTCell::UpdateTotalEnergy(double* const pKinetic, double* const pPotential) const
{
#if SimIdentifier != BD
	// Common local data

	double dx[3], dv[3];
	double dr, dr2;
	double v2;
    double pe;
	double kinetic;
	double potential;

	// DPD-specific local data

#if SimIdentifier == DPD

	double wr, wr2;

#ifdef UseDPDBeadRadii
	double drmax;
#endif

	// MD-specific local data

#elif SimIdentifier == MD

	double magLJ;						// LJ potential variables
	double eLJ, sLJOverR, sLJR3, sLJR6;

	double magSC;						// SC potential variables
	double eSC, sSCOverR, sSCR3;

#endif

    pe        = 0.0;
	kinetic	  = 0.0;	// Zero running totals
	potential = 0.0;

	for(cBeadListIterator iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
		CAbstractBead* pBead1 = *iterBead1;
		const long beadType1 = pBead1->GetType();

		// Bead kinetic energy

		v2 = pBead1->m_Mom[0]*pBead1->m_Mom[0] +
			 pBead1->m_Mom[1]*pBead1->m_Mom[1] +
			 pBead1->m_Mom[2]*pBead1->m_Mom[2];

		kinetic += v2;

		// Bead potential energy.
		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		for(crBeadListIterator riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=pBead1->m_id; ++riterBead2 )
		{
			CAbstractBead* const pBead2 = *riterBead2;
			const long beadType2 = pBead2->GetType();

			dx[0] = (pBead1->m_Pos[0] - pBead2->m_Pos[0]);
			dv[0] = (pBead1->m_Mom[0] - pBead2->m_Mom[0]);

			dx[1] = (pBead1->m_Pos[1] - pBead2->m_Pos[1]);
			dv[1] = (pBead1->m_Mom[1] - pBead2->m_Mom[1]);

#if SimDimension == 2
			dx[2] = 0.0;
			dv[2] = 0.0;
#elif SimDimension == 3
			dx[2] = (pBead1->m_Pos[2] - pBead2->m_Pos[2]);
			dv[2] = (pBead1->m_Mom[2] - pBead2->m_Mom[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
			if( dr2 < 1.0 )
			{		
				dr = sqrt(dr2);
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr);
					wr2 = wr*wr;
#else
			dr = sqrt(dr2);
			drmax = pBead1->GetRadius() + pBead2->GetRadius();

			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = drmax*wr*wr;
#endif

// Conservative potential energy: note the difference from the force calculation
// in that wr2 includes the sum of the bead radii if the UseDPDBeadRadii flag is set.

					pe  = 0.5*m_vvConsInt.at(beadType1).at(beadType2)*wr2;				

#elif SimIdentifier == MD

			if( dr2 < m_coradius2 )
			{		
				dr = sqrt(dr2);

				if( dr > 0.000000001 )
				{
					// Calculate common factors in the LJ potential

					eLJ       = m_vvLJDepth.at(beadType1).at(beadType2);	
					sLJOverR  = m_vvLJRange.at(beadType1).at(beadType2)/dr;
					sLJR3	  = sLJOverR*sLJOverR*sLJOverR;
					sLJR6	  = sLJR3*sLJR3;
					magLJ	  = eLJ*sLJR6*(sLJR6 - 1.0);

					// Note that the cutoff radius has to be scaled by the first LJ potential
					// range as well as the other terms.

					pe = (magLJ - m_vvLJDelta.at(beadType1).at(beadType2)
							+ m_vvLJSlope.at(beadType1).at(beadType2)*(dr - m_cutoffradius));

					// Only add the soft-core potential if the potential depth is non-zero

					eSC = m_vvSCDepth.at(beadType1).at(beadType2);	

					if(eSC > 0.0)
					{
						sSCOverR  = m_vvSCRange.at(beadType1).at(beadType2)/dr;
						sSCR3     = sSCOverR*sSCOverR*sSCOverR;
						magSC     = eSC*sSCR3*sSCR3*sSCR3;
							
						pe += (magSC - m_vvSCDelta.at(beadType1).at(beadType2)
								+ m_vvSCSlope.at(beadType1).at(beadType2)*(dr - m_cutoffradius));
					}	
					
#endif
				}
				else
				{
					TraceInt("same cell bead", pBead1->GetId());
					TraceInt("interacts with", pBead2->GetId());
					TraceVector("1st pos",	pBead1->GetXPos(),
											pBead1->GetYPos(), 
											pBead1->GetZPos() );
					TraceVector("2nd pos",	pBead2->GetXPos(),
											pBead2->GetYPos(), 
											pBead2->GetZPos() );
					TraceDouble("Bead distance", dr);
				}

				potential += pe;
			}
		}

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.
		// We avoid double counting the contributions from beads in adjacent cells
		// by only doing the calculation if the adjacent cell bead has an id
		// higher than the current cell bead.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
			for(BeadListIterator iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++ )
			{
				if((*iterBead2)->GetId() > pBead1->GetId())
				{
					CAbstractBead* const pBead2 = *iterBead2;
					const long beadType2 = pBead2->GetType();

					dx[0] = (pBead1->m_Pos[0] - pBead2->m_Pos[0]);
					dv[0] = (pBead1->m_Mom[0] - pBead2->m_Mom[0]);

					dx[1] = (pBead1->m_Pos[1] - pBead2->m_Pos[1]);
					dv[1] = (pBead1->m_Mom[1] - pBead2->m_Mom[1]);

	#if SimDimension == 2
					dx[2] = 0.0;
					dv[2] = 0.0;
	#elif SimDimension == 3
					dx[2] = (pBead1->m_Pos[2] - pBead2->m_Pos[2]);
					dv[2] = (pBead1->m_Mom[2] - pBead2->m_Mom[2]);
	#endif

					if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
					{
						if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
							dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
						else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
							dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

						if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
							dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
						else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
							dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

	#if SimDimension == 3
						if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
							dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
						else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
							dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
	#endif

					}

					dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

	// Calculate the interactions between the two beads for each simulation type.

	#if SimIdentifier == DPD

	#ifndef UseDPDBeadRadii
					if( dr2 < 1.0 )
					{		
						dr = sqrt(dr2);
						if( dr > 0.000000001 )
						{
							wr = (1.0 - dr);
							wr2 = wr*wr;

	#else
					dr = sqrt(dr2);
					drmax = pBead1->GetRadius() + pBead2->GetRadius();
					if( dr < drmax )
					{		
						if( dr > 0.000000001 )
						{
							wr = (1.0 - dr/drmax);
							wr2 = drmax*wr*wr;
	#endif

	// Conservative potential energy: note the difference from the force calculation
	// in that wr2 includes the sum of the bead radii if the UseDPDBeadRadii flag is set.

							pe  = 0.5*m_vvConsInt.at(beadType1).at(beadType2)*wr2;				

	#elif SimIdentifier == MD

					if( dr2 < m_coradius2 )
					{		
						dr = sqrt(dr2);

						if( dr > 0.000000001 )
						{
							// Calculate common factors in the LJ potential

							eLJ       = m_vvLJDepth.at(beadType1).at(beadType2);	
							sLJOverR  = m_vvLJRange.at(beadType1).at(beadType2)/dr;
							sLJR3	  = sLJOverR*sLJOverR*sLJOverR;
							sLJR6	  = sLJR3*sLJR3;
							magLJ	  = eLJ*sLJR6*(sLJR6 - 1.0);

							// Note that the cutoff radius has to be scaled by the first LJ potential
							// range as well as the other terms.

							pe = (magLJ - m_vvLJDelta.at(beadType1).at(beadType2)
									+ m_vvLJSlope.at(beadType1).at(beadType2)*(dr - m_cutoffradius));

							// Only add the soft-core potential if the potential depth is non-zero

							eSC = m_vvSCDepth.at(beadType1).at(beadType2);	

							if(eSC > 0.0)
							{
								sSCOverR  = m_vvSCRange.at(beadType1).at(beadType2)/dr;
								sSCR3     = sSCOverR*sSCOverR*sSCOverR;
								magSC     = eSC*sSCR3*sSCR3*sSCR3;
									
								pe += (magSC - m_vvSCDelta.at(beadType1).at(beadType2)
										+ m_vvSCSlope.at(beadType1).at(beadType2)*(dr - m_cutoffradius));
							}	
	#endif

						}
						else
						{
							TraceInt("neighbour cell bead", pBead1->GetId());
							TraceInt("interacts with", pBead2->GetId());
							TraceVector("1st pos",	pBead1->GetXPos(),
													pBead1->GetYPos(), 
													pBead1->GetZPos() );
							TraceVector("2nd pos",	pBead2->GetXPos(),
													pBead2->GetYPos(), 
													pBead2->GetZPos() );
							TraceDouble("Bead distance", dr);
						}

						potential += pe;
					}
				}
			}
		}
	}

	// Return the kinetic and potential energies via the arguments

	*pKinetic   = kinetic;
	*pPotential = potential;

	// Pass the total kinetic and potential energies for the beads in the current cell
	// to the CMonitor for summation and output. 

	m_pMonitor->AddBeadEnergy(kinetic, potential);

#endif
}

// Function that replaces UpdateForce() when the DPD density-dependent force
// is included. This allows the appearance of liquid-gas interfaces in the 
// simulation. It is only included if the EnableDPDLG flag is set, but even then
// should only be called if the LG force is actually included as it is 
// less efficient than the standard force calculation.
//
// It peforms an extra calculation of the local bead density around each of the
// pair of interacting beads and adds in a density-dependent force after
// Warren PRL 2001.

void CCNTCell::UpdateLGForce()
{
#if EnableDPDLG == ExperimentEnabled

	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3], dv[3], newForce[3];
	double dr, dr2;
	double gammap, rdotv, wr, wr2;
	double conForce, dissForce, randForce;
	double drmax;

    double lgForce, lgPrefactor;
    double wrd, drdmax;  // Parameters needed for DPDLG force

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		// Zero the stress tensor for this bead. Notice that because of the
		// way we only sum over beads distinct from the current bead in the same
		// cell, and over beads in cells to the right and above the current cell
		// we don't have to worry about double counting the pairwise forces. We
		// can hence store the N(N-1)/2 contributions to the stress tensor in the
		// beads that form the first ones accessed in the double loops.

		for(short int j=0; j<9; j++)
		{
			(*iterBead1)->m_Stress[j] = 0.0;
		}

		for( riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=(*iterBead1)->m_id; ++riterBead2 )
		{
			dx[0] = ((*iterBead1)->m_Pos[0] - (*riterBead2)->m_Pos[0]);
			dv[0] = ((*iterBead1)->m_Mom[0] - (*riterBead2)->m_Mom[0]);

			dx[1] = ((*iterBead1)->m_Pos[1] - (*riterBead2)->m_Pos[1]);
			dv[1] = ((*iterBead1)->m_Mom[1] - (*riterBead2)->m_Mom[1]);

#if SimDimension == 2
			dx[2] = 0.0;
			dv[2] = 0.0;
#elif SimDimension == 3
			dx[2] = ((*iterBead1)->m_Pos[2] - (*riterBead2)->m_Pos[2]);
			dv[2] = ((*iterBead1)->m_Mom[2] - (*riterBead2)->m_Mom[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two DPD beads including the 
// density-dependent force. Note that the LG force prefactor depends on
// the cube of the interaction range, and this may depend on bead type.
// If all beads have the same LG range, then it is a constant factor and
// it would be more efficient to include in the constant m_lgnorm.

			dr = sqrt(dr2);
			drmax  = (*iterBead1)->GetRadius()+(*riterBead2)->GetRadius();
			drdmax = (*iterBead1)->GetLGRadius()+(*riterBead2)->GetLGRadius();

			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = wr*wr;
					wrd = (1.0 - dr/drdmax);
                    lgPrefactor = m_lgnorm/(drdmax*drdmax*drdmax);

// Conservative force magnitude

					conForce  = m_vvConsInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*wr;				
//					conForce = 0.0;

// Density-dependent force magnitude: first we get the raw interaction parameter
// then we multiply it by the local density function. This requires that the 
// bead densities for every bead must be calculated before this loop.

					lgForce  = lgPrefactor*m_vvLGInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*((*iterBead1)->GetLGDensity() + (*riterBead2)->GetLGDensity())*wrd;				

// Dissipative and random force magnitudes. Note dr factor in newForce calculation

					rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
					gammap		= m_vvDissInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*wr2;

					dissForce	= -gammap*rdotv;				
					randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());

					newForce[0] = (conForce + lgForce + dissForce + randForce)*dx[0]/dr;
					newForce[1] = (conForce + lgForce + dissForce + randForce)*dx[1]/dr;
					newForce[2] = (conForce + lgForce + dissForce + randForce)*dx[2]/dr;

					(*iterBead1)->m_Force[0] += newForce[0];
					(*iterBead1)->m_Force[1] += newForce[1];
					(*iterBead1)->m_Force[2] += newForce[2];

					(*riterBead2)->m_Force[0] -= newForce[0];
					(*riterBead2)->m_Force[1] -= newForce[1];
					(*riterBead2)->m_Force[2] -= newForce[2];

					// stress tensor summation

					(*iterBead1)->m_Stress[0] += dx[0]*newForce[0];
					(*iterBead1)->m_Stress[1] += dx[1]*newForce[0];
					(*iterBead1)->m_Stress[2] += dx[2]*newForce[0];
					(*iterBead1)->m_Stress[3] += dx[0]*newForce[1];
					(*iterBead1)->m_Stress[4] += dx[1]*newForce[1];
					(*iterBead1)->m_Stress[5] += dx[2]*newForce[1];
					(*iterBead1)->m_Stress[6] += dx[0]*newForce[2];
					(*iterBead1)->m_Stress[7] += dx[1]*newForce[2];
					(*iterBead1)->m_Stress[8] += dx[2]*newForce[2];

					// Pass the stress tensor components to the CMonitor
					// for use in analysing the stress over slices.
					// Note that dx[2] might be close to zero and we should
					// check for this.

//					m_pMonitor->AddBeadStress(*iterBead1, *riterBead2, newForce, dx);
				}
				else
				{
					TraceInt("same cell bead", (*iterBead1)->GetId());
					TraceInt("interacts with", (*riterBead2)->GetId());
					TraceVector("1st pos",	(*iterBead1)->GetXPos(),
											(*iterBead1)->GetYPos(), 
											(*iterBead1)->GetZPos() );
					TraceVector("2nd pos",	(*riterBead2)->GetXPos(),
											(*riterBead2)->GetYPos(), 
											(*riterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
			for( iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++ )
			{
				dx[0] = ((*iterBead1)->m_Pos[0] - (*iterBead2)->m_Pos[0]);
				dv[0] = ((*iterBead1)->m_Mom[0] - (*iterBead2)->m_Mom[0]);

				dx[1] = ((*iterBead1)->m_Pos[1] - (*iterBead2)->m_Pos[1]);
				dv[1] = ((*iterBead1)->m_Mom[1] - (*iterBead2)->m_Mom[1]);

#if SimDimension == 2
				dx[2] = 0.0;
				dv[2] = 0.0;
#elif SimDimension == 3
				dx[2] = ((*iterBead1)->m_Pos[2] - (*iterBead2)->m_Pos[2]);
				dv[2] = ((*iterBead1)->m_Mom[2] - (*iterBead2)->m_Mom[2]);
#endif

				if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
				{
					if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
					else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

					if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
					else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
					if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
					else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

				}

				dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
				dr = sqrt(dr2);
				drmax  = (*iterBead1)->GetRadius()+(*iterBead2)->GetRadius();
				drdmax = (*iterBead1)->GetLGRadius()+(*iterBead2)->GetLGRadius();

				if( dr < drmax )
				{		
					if( dr > 0.000000001 )
					{
					   wr = (1.0 - dr/drmax);
                       wr2 = wr*wr;
                       wrd = (1.0 - dr/drdmax);
                       lgPrefactor = m_lgnorm/(drdmax*drdmax*drdmax);

                       conForce	    = m_vvConsInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr;				
//                       conForce = 0.0;

// Density-dependent force magnitude: first we get the raw interaction parameter
// then we multiply it by the local density function

					    lgForce     = lgPrefactor*m_vvLGInt.at((*iterBead1)->GetType()).at((*riterBead2)->GetType())*((*iterBead1)->GetLGDensity() + (*riterBead2)->GetLGDensity())*wrd;				

						rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
						gammap		= m_vvDissInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr2;

						dissForce	= -gammap*rdotv;				
						randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());

						newForce[0] = (conForce + lgForce + dissForce + randForce)*dx[0]/dr;
						newForce[1] = (conForce + lgForce + dissForce + randForce)*dx[1]/dr;
						newForce[2] = (conForce + lgForce + dissForce + randForce)*dx[2]/dr;

						(*iterBead1)->m_Force[0] += newForce[0];
						(*iterBead1)->m_Force[1] += newForce[1];
						(*iterBead1)->m_Force[2] += newForce[2];

						(*iterBead2)->m_Force[0] -= newForce[0];
						(*iterBead2)->m_Force[1] -= newForce[1];
						(*iterBead2)->m_Force[2] -= newForce[2];

						// stress tensor summation

						(*iterBead1)->m_Stress[0] += dx[0]*newForce[0];
						(*iterBead1)->m_Stress[1] += dx[1]*newForce[0];
						(*iterBead1)->m_Stress[2] += dx[2]*newForce[0];
						(*iterBead1)->m_Stress[3] += dx[0]*newForce[1];
						(*iterBead1)->m_Stress[4] += dx[1]*newForce[1];
						(*iterBead1)->m_Stress[5] += dx[2]*newForce[1];
						(*iterBead1)->m_Stress[6] += dx[0]*newForce[2];
						(*iterBead1)->m_Stress[7] += dx[1]*newForce[2];
						(*iterBead1)->m_Stress[8] += dx[2]*newForce[2];

						// Pass the stress tensor components to the CMonitor
						// for use in analysing the stress over slices.
						// Note that dx[2] might be close to zero and we should
						// check for this.

//						m_pMonitor->AddBeadStress(*iterBead1, *iterBead2, newForce, dx);

					}
					else
					{
						TraceInt("neighbour cell bead", (*iterBead1)->GetId());
						TraceInt("interacts with", (*iterBead2)->GetId());
						TraceVector("1st pos",	(*iterBead1)->GetXPos(),
												(*iterBead1)->GetYPos(), 
												(*iterBead1)->GetZPos() );
						TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
												(*iterBead2)->GetYPos(), 
												(*iterBead2)->GetZPos() );
						TraceDouble("Bead distance", dr);
					}
				}
			}
		}
	}

#endif
}

// Function to calculate the local bead density around each bead in the cell.
// This is used to weight the density-dependent force on the beads. We perform
// a separate loop over the cells so that the density can be calculated for all
// beads before we update the total force.

void CCNTCell::UpdateLGDensity()
{
#if EnableDPDLG == ExperimentEnabled
    // Get a list of all beads in the current cell and its 26 neighbours

    BeadList localBeads;
    localBeads.clear();

    BeadList lNNBeads;
    lNNBeads.clear();

    for(short int i=0; i<27; i++)
    {
        lNNBeads = m_aNNCells[i]->GetBeads();
		copy(lNNBeads.begin(), lNNBeads.end(), back_inserter(localBeads));
    }

    // Now calculate the local density around each bead in the cell.
    // Note that this does not include the constant normalisation factor that
    // ensures the integral over the range is unity; that is factored into
    // the density-dependent interaction parameter itself. We loop over all beads
    // in the current cell and its 26 nearest neighbours, but as this includes
    // the bead itself twice, we have to remove the self-interaction.

    double dx[3];
    double dr, dr2, drdmax, drdmax2;
    double wr2;

    double density;

	for(BeadListIterator iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++)
	{
        density = 0.0;

		for(BeadListIterator iterBead2=localBeads.begin(); iterBead2!=localBeads.end(); iterBead2++)
		{
			dx[0] = ((*iterBead1)->m_Pos[0] - (*iterBead2)->m_Pos[0]);
			dx[1] = ((*iterBead1)->m_Pos[1] - (*iterBead2)->m_Pos[1]);
			dx[2] = ((*iterBead1)->m_Pos[2] - (*iterBead2)->m_Pos[2]);

            // If the current CNT cell is on the SimBox boundary, we check the 
            // PBCs for all bead pairs as we don't do a loop over neighbouring 
            // CNT cells here. As long as there are not too many beads in the 
            // current cell this should not be a resource drain. 

			if(m_bExternal)
			{
				if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
					dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
				else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
					dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

				if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
					dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
				else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
					dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;
				if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
					dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
				else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
					dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
			}


			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
			drdmax = (*iterBead1)->GetLGRadius()+(*iterBead2)->GetLGRadius();
            drdmax2 = drdmax*drdmax;

            // The second test should remove self-interactions

            if(dr2 < drdmax2 && dr2 > 0.00000001)
            {
			    dr = sqrt(dr2);
                wr2 = (1.0 - dr/drdmax)*(1.0 - dr/drdmax);

                density += wr2;
            }
        }

        (*iterBead1)->SetLGDensity(density);
	}
#endif
}

// Function a cell uses to calculate non-bonded, bead-bead forces between
// beads in the current cell and its neighbouring cells that belong to adjacent
// processors and that may be either instances of CCNTCell or CExternalCNTCell.
// It is only used in parallel runs.
//
// Depending on its position in the owning processor's Border region, the cell
// has a specific number of external cell neighbours that are hardwired into
// its m_aExtNNCells[] array. These instances are CExternalCNTCell pointers not
// CCNTCell pointers. Hence, we iterate over them retrieving the bead data that
// has been collected there by the mpsSimBox before the force loop is entered.
// Bead data are collected simply as (id, type, x, y, z, vx, vy, vz) and used to
// calculate the forces on the beads in the current cell. The forces are then also
// stored using the ids of the beads belonging to the adjacent processor so that they
// can be sent back to update the other processor's beads.

void CCNTCell::UpdateForceP()
{
	mpsSimBox::GlobalCellCounter++;
	
	long localCellCellCounter = 0;

    // Now we iterate over the neighbouring cells calling different functions
    // depending on whether each neighbour is internal or external

#if SimIdentifier == BD

    CAbstractBead*   pBead1;
	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3];
	double dr, dr2, drmax;

	double wr, wr2;
	double conForce;
    double newForce1[3], newForce2[3];
		

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
        pBead1 = *iterBead1;

		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		// Zero the stress tensor for this bead. Notice that because of the
		// way we only sum over beads distinct from the current bead in the same
		// cell, and over beads in cells to the right and above the current cell
		// we don't have to worry about double counting the pairwise forces. We
		// can hence store the N(N-1)/2 contributions to the stress tensor in the
		// beads that form the first ones accessed in the double loops.

		for(short int j=0; j<9; j++)
		{
			pBead1->m_Stress[j] = 0.0;
		}

		for( riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=pBead1->m_id; ++riterBead2 )
		{
			dx[0] = (pBead1->m_Pos[0] - (*riterBead2)->m_Pos[0]);
			dx[1] = (pBead1->m_Pos[1] - (*riterBead2)->m_Pos[1]);

#if SimDimension == 2
			dx[2] = 0.0;
#elif SimDimension == 3
			dx[2] = (pBead1->m_Pos[2] - (*riterBead2)->m_Pos[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
			dr = sqrt(dr2);
			drmax = pBead1->GetRadius()+(*riterBead2)->GetRadius();
			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = wr*wr;

// Conservative force magnitude

					conForce = m_vvConsInt.at(pBead1->GetType()).at((*riterBead2)->GetType())*wr/dr;				
//				    conForce = 0.0;		
 
                    for(short int i=0; i<3; i++)
                    {
                        newForce1[i] = conForce*dx[i];
 					    newForce2[i] = -conForce*dx[i];

                        pBead1->m_Force[i]  += newForce1[i];
					    (*riterBead2)->m_Force[i] += newForce2[i];
                    }

					// stress tensor summation

					pBead1->m_Stress[0] += dx[0]*newForce1[0];
					pBead1->m_Stress[1] += dx[1]*newForce1[0];
					pBead1->m_Stress[2] += dx[2]*newForce1[0];
					pBead1->m_Stress[3] += dx[0]*newForce1[1];
					pBead1->m_Stress[4] += dx[1]*newForce1[1];
					pBead1->m_Stress[5] += dx[2]*newForce1[1];
					pBead1->m_Stress[6] += dx[0]*newForce1[2];
					pBead1->m_Stress[7] += dx[1]*newForce1[2];
					pBead1->m_Stress[8] += dx[2]*newForce1[2];

					// Pass the stress tensor components to the CMonitor
					// for use in analysing the stress over slices.
					// Note that dx[2] might be close to zero and we should
					// check for this.
                    // Is there a problem in the stress calculation now that the 
                    // force on each bead in an interacting pair is not the same?

					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
					m_pMonitor->AddBeadStress(*iterBead1, *riterBead2, newForce1, dx);
#endif
				}
				else
				{
					TraceInt("same cell bead", pBead1->GetId());
					TraceInt("interacts with", (*riterBead2)->GetId());
					TraceVector("1st pos",	pBead1->GetXPos(),
											pBead1->GetYPos(), 
											pBead1->GetZPos() );
					TraceVector("2nd pos",	(*riterBead2)->GetXPos(),
											(*riterBead2)->GetYPos(), 
											(*riterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}

		// Next add in interactions with beads in neighbouring cells.
        // We have to do different processing depending on whether the neighbouring
        // cell is internal or external. In the latter case, we pass the 
        // bead pointer to the external cell so that it can calculate all 
        // forces involving that bead, and the current cell's external pointer
        // so the neighouring cell knows whether to apply the PBCs.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
            CCNTCell* const pNNCell = m_aIntNNCells[i];

            pNNCell->UpdateForceBetweenCells(IsExternal(), pBead1);
        }
    }

#else
    // DPD and MD equations of motion

    CAbstractBead*   pBead1;
	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3], dv[3], newForce[3];
	double dr, dr2;

#if SimIdentifier == DPD

	double gammap, rdotv, wr, wr2;
	double conForce, dissForce, randForce;

#ifdef UseDPDBeadRadii
	double drmax;
#endif

#elif SimIdentifier == MD

	double magLJ;						// LJ potential variables
	double eLJ, sLJOverR, sLJR3, sLJR6;

	double magSC;						// SC potential variables
	double eSC, sSCOverR, sSCR3;

#endif

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{
        pBead1 = *iterBead1;

		// First add interactions between beads in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring beads until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the bead ids for
		// the terminating condition.

		// Zero the stress tensor for this bead. Notice that because of the
		// way we only sum over beads distinct from the current bead in the same
		// cell, and over beads in cells to the right and above the current cell
		// we don't have to worry about double counting the pairwise forces. We
		// can hence store the N(N-1)/2 contributions to the stress tensor in the
		// beads that form the first ones accessed in the double loops.

		for(short int j=0; j<9; j++)
		{
			pBead1->m_Stress[j] = 0.0;
		}

		for( riterBead2=m_lBeads.rbegin(); (*riterBead2)->m_id!=pBead1->m_id; ++riterBead2 )
		{
			dx[0] = (pBead1->m_Pos[0] - (*riterBead2)->m_Pos[0]);
			dv[0] = (pBead1->m_Mom[0] - (*riterBead2)->m_Mom[0]);

			dx[1] = (pBead1->m_Pos[1] - (*riterBead2)->m_Pos[1]);
			dv[1] = (pBead1->m_Mom[1] - (*riterBead2)->m_Mom[1]);

#if SimDimension == 2
			dx[2] = 0.0;
			dv[2] = 0.0;
#elif SimDimension == 3
			dx[2] = (pBead1->m_Pos[2] - (*riterBead2)->m_Pos[2]);
			dv[2] = (pBead1->m_Mom[2] - (*riterBead2)->m_Mom[2]);
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we use the flag UseDPDBeadRadii to determine whether
// the interaction radius is bead-specific or not. Note that when we use the
// interaction radius we have to compare the actual distance between beads
// not its square!

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
			if( dr2 < 1.0 )
			{		
				dr = sqrt(dr2);
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr);
					wr2 = wr*wr;
#else
			dr = sqrt(dr2);
			drmax = pBead1->GetRadius()+(*riterBead2)->GetRadius();
			if( dr < drmax )
			{		
				if( dr > 0.000000001 )
				{
					wr = (1.0 - dr/drmax);
					wr2 = wr*wr;
#endif

// Conservative force magnitude

					conForce  = m_vvConsInt.at(pBead1->GetType()).at((*riterBead2)->GetType())*wr;				
//				    conForce = 0.0;		

// Dissipative and random force magnitudes. Note dr factor in newForce calculation

					rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
					gammap		= m_vvDissInt.at(pBead1->GetType()).at((*riterBead2)->GetType())*wr2;

					dissForce	= -gammap*rdotv;				
					randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());

					newForce[0] = (conForce + dissForce + randForce)*dx[0]/dr;
					newForce[1] = (conForce + dissForce + randForce)*dx[1]/dr;
					newForce[2] = (conForce + dissForce + randForce)*dx[2]/dr;

#elif SimIdentifier == MD

			if( dr2 < m_coradius2 )
			{		
				dr = sqrt(dr2);

				if( dr > 0.000000001 )
				{
					// Calculate common factors in the LJ potential and include shifted force.
					// Note that the shifted force term needs to be multiplied by the 
					// unit vector in the bead-bead separation whereas the force term 
					// has the full vector.

					eLJ		  = m_vvLJDepth.at(pBead1->GetType()).at((*riterBead2)->GetType());	
					sLJOverR  = m_vvLJRange.at(pBead1->GetType()).at((*riterBead2)->GetType())/dr;
					sLJR3     = sLJOverR*sLJOverR*sLJOverR;
					sLJR6     = sLJR3*sLJR3;

					magLJ = (6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0) 
						     - dr*m_vvLJSlope.at(pBead1->GetType()).at((*riterBead2)->GetType()))/dr2;

					// Only add the soft-core potential if the potential depth is non-zero

					eSC = m_vvSCDepth.at(pBead1->GetType()).at((*riterBead2)->GetType());	

					if(eSC > 0.0)
					{
						sSCOverR = m_vvSCRange.at(pBead1->GetType()).at((*riterBead2)->GetType())/dr;
						sSCR3    = sSCOverR*sSCOverR*sSCOverR;
						
						magSC = (9.0*eSC*sSCR3*sSCR3*sSCR3
							     - dr*m_vvSCSlope.at(pBead1->GetType()).at((*riterBead2)->GetType()))/dr2;
					}
					else
					{
						magSC = 0.0;
					}

					newForce[0] = (magLJ + magSC)*dx[0];
					newForce[1] = (magLJ + magSC)*dx[1];
					newForce[2] = (magLJ + magSC)*dx[2];
#endif
				    pBead1->m_ForceCounter++;
				    (*riterBead2)->m_ForceCounter++;

					pBead1->m_Force[0] += newForce[0];
					pBead1->m_Force[1] += newForce[1];
					pBead1->m_Force[2] += newForce[2];

					(*riterBead2)->m_Force[0] -= newForce[0];
					(*riterBead2)->m_Force[1] -= newForce[1];
					(*riterBead2)->m_Force[2] -= newForce[2];

					// stress tensor summation

					pBead1->m_Stress[0] += dx[0]*newForce[0];
					pBead1->m_Stress[1] += dx[1]*newForce[0];
					pBead1->m_Stress[2] += dx[2]*newForce[0];
					pBead1->m_Stress[3] += dx[0]*newForce[1];
					pBead1->m_Stress[4] += dx[1]*newForce[1];
					pBead1->m_Stress[5] += dx[2]*newForce[1];
					pBead1->m_Stress[6] += dx[0]*newForce[2];
					pBead1->m_Stress[7] += dx[1]*newForce[2];
					pBead1->m_Stress[8] += dx[2]*newForce[2];

					// Pass the stress tensor components to the CMonitor
					// for use in analysing the stress over slices.
					// Note that dx[2] might be close to zero and we should
					// check for this.

					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
					m_pMonitor->AddBeadStress(*iterBead1, *riterBead2, newForce, dx);
#endif
				}
				else
				{
					TraceInt("same cell bead", pBead1->GetId());
					TraceInt("interacts with", (*riterBead2)->GetId());
					TraceVector("1st pos",	pBead1->GetXPos(),
											pBead1->GetYPos(), 
											pBead1->GetZPos() );
					TraceVector("2nd pos",	(*riterBead2)->GetXPos(),
											(*riterBead2)->GetYPos(), 
											(*riterBead2)->GetZPos() );
					TraceDouble("Bead distance", dr);
				}
			}
		}

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{

		    localCellCellCounter++;
			
            CCNTCell* const pNNCell = m_aIntNNCells[i];
			
/*			
		    if(xxParallelBase::GlobalGetRank() == 0)
			{
		
			    if(dynamic_cast<CExternalCNTCell*>(pNNCell))
				{
		            std::cout << "Calculating External  cell-cell forces for " << GetId() << " " << pNNCell->GetId() << zEndl;
				}
				else
				{
		            std::cout << "Calculating Internal  cell-cell forces for " << GetId() << " " << pNNCell->GetId() << zEndl;
				}
			}

*/

            pNNCell->UpdateForceBetweenCells(IsExternal(), pBead1);
        }
    }
#endif

	if(m_lBeads.size() > 0)
	{
	    localCellCellCounter /= m_lBeads.size();
	}
	else
	{
	    localCellCellCounter = 13;
	}
	
    mpsSimBox::GlobalCellCellIntCounter += localCellCellCounter;



}

// VF used by this class to calculate forces between beads in two adjacent
// cells that are both internal to one processor. The argument is a pointer
// to a bead in the original cell being iterated over within UpdateForceP(), 
// and the current cell is one of the nearest-neighbours of the original cell.
// We perform the loop over all the beads in the current cell calculating their
// forces with the original particle. 

void CCNTCell::UpdateForceBetweenCells(bool bExternal, CAbstractBead* const pBead)
{
//    const long targetBeadId = 4; // Write out interactions for this bead only

#if SimIdentifier == BD

	BeadListIterator iterBead1;
	BeadListIterator iterBead2;
	rBeadListIterator riterBead2;

	double dx[3];
	double dr, dr2, drmax;

	double wr, wr2;
	double conForce;
    double newForce1[3], newForce2[3];

	for( iterBead1=m_lBeads.begin(); iterBead1!=m_lBeads.end(); iterBead1++ )
	{

		// Next add in interactions with beads in neighbouring cells taking the
		// PBCs into account and the presence of a wall. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

#if SimDimension == 2
		for( int i=0; i<4; i++ )
#elif SimDimension == 3
		for( int i=0; i<13; i++ )
#endif
		{
			for( iterBead2=m_aIntNNCells[i]->m_lBeads.begin(); iterBead2!=m_aIntNNCells[i]->m_lBeads.end(); iterBead2++ )
			{
				dx[0] = ((*iterBead1)->m_Pos[0] - (*iterBead2)->m_Pos[0]);
				dx[1] = ((*iterBead1)->m_Pos[1] - (*iterBead2)->m_Pos[1]);

#if SimDimension == 2
				dx[2] = 0.0;
#elif SimDimension == 3
				dx[2] = ((*iterBead1)->m_Pos[2] - (*iterBead2)->m_Pos[2]);
#endif

				if( m_bExternal && m_aIntNNCells[i]->IsExternal() )
				{
					if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
					else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

					if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
					else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
					if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
					else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

				}

				dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we have to change the distance-dependence according
// to the bead interaction radii.

				dr = sqrt(dr2);
				drmax = (*iterBead1)->GetRadius()+(*iterBead2)->GetRadius();
				if( dr < drmax )
				{		
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr/drmax);
						wr2 = wr*wr;
 
 // Conservative force magnitude

					    conForce = m_vvConsInt.at((*iterBead1)->GetType()).at((*iterBead2)->GetType())*wr/dr;				
//                        conForce = 0.0;
						
                        for(short int i=0; i<3; i++)
                        {
                            newForce1[i] = conForce*dx[i];
 					        newForce2[i] = -conForce*dx[i];

                            (*iterBead1)->m_Force[i] += newForce1[i];
					        (*iterBead2)->m_Force[i] += newForce2[i];
                        }
                       
						// stress tensor summation

						(*iterBead1)->m_Stress[0] += dx[0]*newForce1[0];
						(*iterBead1)->m_Stress[1] += dx[1]*newForce1[0];
						(*iterBead1)->m_Stress[2] += dx[2]*newForce1[0];
						(*iterBead1)->m_Stress[3] += dx[0]*newForce1[1];
						(*iterBead1)->m_Stress[4] += dx[1]*newForce1[1];
						(*iterBead1)->m_Stress[5] += dx[2]*newForce1[1];
						(*iterBead1)->m_Stress[6] += dx[0]*newForce1[2];
						(*iterBead1)->m_Stress[7] += dx[1]*newForce1[2];
						(*iterBead1)->m_Stress[8] += dx[2]*newForce1[2];

						// Pass the stress tensor components to the CMonitor
						// for use in analysing the stress over slices.
						// Note that dx[2] might be close to zero and we should
						// check for this.

					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
						m_pMonitor->AddBeadStress(*iterBead1, *iterBead2, newForce1, dx);
#endif
					}
					else
					{
						TraceInt("neighbour cell bead", (*iterBead1)->GetId());
						TraceInt("interacts with", (*iterBead2)->GetId());
						TraceVector("1st pos",	(*iterBead1)->GetXPos(),
												(*iterBead1)->GetYPos(), 
												(*iterBead1)->GetZPos() );
						TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
												(*iterBead2)->GetYPos(), 
												(*iterBead2)->GetZPos() );
						TraceDouble("Bead distance", dr);
					}
				}
			}
		}
	}

#else
    // DPD and MD equations of motion

	BeadListIterator iterBead2;

	double dx[3], dv[3], newForce[3];
	double dr, dr2;

#if SimIdentifier == DPD

	double gammap, rdotv, wr, wr2;
	double conForce, dissForce, randForce;

#ifdef UseDPDBeadRadii
	double drmax;
#endif

#elif SimIdentifier == MD

	double magLJ;						// LJ potential variables
	double eLJ, sLJOverR, sLJR3, sLJR6;

	double magSC;						// SC potential variables
	double eSC, sSCOverR, sSCR3;

#endif

		// Add in interactions of the passed-in bead with beads in the current
        // cell taking the PBCs into account. The PBCs are only applied
		// if both the current CNT cell and the neighbouring one are external.

			for( iterBead2=m_lBeads.begin(); iterBead2!=m_lBeads.end(); iterBead2++ )
			{
				dx[0] = (pBead->m_Pos[0] - (*iterBead2)->m_Pos[0]);
				dv[0] = (pBead->m_Mom[0] - (*iterBead2)->m_Mom[0]);

				dx[1] = (pBead->m_Pos[1] - (*iterBead2)->m_Pos[1]);
				dv[1] = (pBead->m_Mom[1] - (*iterBead2)->m_Mom[1]);

#if SimDimension == 2
				dx[2] = 0.0;
				dv[2] = 0.0;
#elif SimDimension == 3
				dx[2] = (pBead->m_Pos[2] - (*iterBead2)->m_Pos[2]);
				dv[2] = (pBead->m_Mom[2] - (*iterBead2)->m_Mom[2]);
#endif

				if( m_bExternal && bExternal )
				{
					if( dx[0] > CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] - CCNTCell::m_SimBoxXLength;
					else if( dx[0] < -CCNTCell::m_HalfSimBoxXLength )
						dx[0] = dx[0] + CCNTCell::m_SimBoxXLength;

					if( dx[1] > CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] - CCNTCell::m_SimBoxYLength;
					else if( dx[1] < -CCNTCell::m_HalfSimBoxYLength )
						dx[1] = dx[1] + CCNTCell::m_SimBoxYLength;

#if SimDimension == 3
					if( dx[2] > CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] - CCNTCell::m_SimBoxZLength;
					else if( dx[2] < -CCNTCell::m_HalfSimBoxZLength )
						dx[2] = dx[2] + CCNTCell::m_SimBoxZLength;
#endif

				}

				dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

// Calculate the interactions between the two beads for each simulation type.
// For the DPD interactions we have to change the distance-dependence according
// to the bead interaction radii.

#if SimIdentifier == DPD

#ifndef UseDPDBeadRadii
				if( dr2 < 1.0 )
				{		
					dr = sqrt(dr2);
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr);
						wr2 = wr*wr;
#else
				dr = sqrt(dr2);
				drmax = pBead->GetRadius()+(*iterBead2)->GetRadius();
				if( dr < drmax )
				{		
					if( dr > 0.000000001 )
					{
						wr = (1.0 - dr/drmax);
						wr2 = wr*wr;
#endif
						conForce	= m_vvConsInt.at(pBead->GetType()).at((*iterBead2)->GetType())*wr;				
//                        conForce    = 0.0;

						rdotv		= (dx[0]*dv[0] + dx[1]*dv[1] + dx[2]*dv[2])/dr;
						gammap		= m_vvDissInt.at(pBead->GetType()).at((*iterBead2)->GetType())*wr2;

						dissForce	= -gammap*rdotv;				
						randForce	= sqrt(gammap)*CCNTCell::m_invrootdt*(0.5 - CCNTCell::Randf());

						newForce[0] = (conForce + dissForce + randForce)*dx[0]/dr;
						newForce[1] = (conForce + dissForce + randForce)*dx[1]/dr;
						newForce[2] = (conForce + dissForce + randForce)*dx[2]/dr;

#elif SimIdentifier == MD

				if( dr2 < m_coradius2 )
				{		
					dr = sqrt(dr2);

					if( dr > 0.000000001 )
					{
						// Calculate common factors in the LJ potential and include shifted force.
						// Note that the shifted force term needs to be multiplied by the 
						// unit vector in the bead-bead separation whereas the force term 
						// has the full vector.

						eLJ       = m_vvLJDepth.at(pBead->GetType()).at((*riterBead2)->GetType());	
						sLJOverR  = m_vvLJRange.at(pBead->GetType()).at((*riterBead2)->GetType())/dr;
						sLJR3     = sLJOverR*sLJOverR*sLJOverR;
						sLJR6     = sLJR3*sLJR3;

						magLJ = (6.0*eLJ*sLJR6*(2.0*sLJR6 - 1.0) 
								 - dr*m_vvLJSlope.at(pBead->GetType()).at((*riterBead2)->GetType()))/dr2;

						// Only add the soft-core potential if the potential depth is non-zero

						eSC = m_vvSCDepth.at(pBead->GetType()).at((*iterBead2)->GetType());	

						if(eSC > 0.0)
						{
							sSCOverR = m_vvSCRange.at(pBead->GetType()).at((*riterBead2)->GetType())/dr;
							sSCR3    = sSCOverR*sSCOverR*sSCOverR;

							magSC = (9.0*eSC*sSCR3*sSCR3*sSCR3
									 - dr*m_vvSCSlope.at(pBead->GetType()).at((*riterBead2)->GetType()))/dr2;
						}
						else
						{
							magSC = 0.0;
						}

						newForce[0] = (magLJ + magSC)*dx[0];
						newForce[1] = (magLJ + magSC)*dx[1];
						newForce[2] = (magLJ + magSC)*dx[2];
#endif

//		if((pBead->GetId() == targetBeadId || (*iterBead2)->GetId() == targetBeadId))
//		{
//			std::cout << "CNTCell " << GetId() << " calculating internal force with NN cell " << " for its beads " << pBead->GetId() << " " << (*iterBead2)->GetId() << zEndl;
//		}

					    pBead->m_ForceCounter++;
					    (*iterBead2)->m_ForceCounter++;
						
						pBead->m_Force[0] += newForce[0];
						pBead->m_Force[1] += newForce[1];
						pBead->m_Force[2] += newForce[2];

						(*iterBead2)->m_Force[0] -= newForce[0];
						(*iterBead2)->m_Force[1] -= newForce[1];
						(*iterBead2)->m_Force[2] -= newForce[2];

						// stress tensor summation

						pBead->m_Stress[0] += dx[0]*newForce[0];
						pBead->m_Stress[1] += dx[1]*newForce[0];
						pBead->m_Stress[2] += dx[2]*newForce[0];
						pBead->m_Stress[3] += dx[0]*newForce[1];
						pBead->m_Stress[4] += dx[1]*newForce[1];
						pBead->m_Stress[5] += dx[2]*newForce[1];
						pBead->m_Stress[6] += dx[0]*newForce[2];
						pBead->m_Stress[7] += dx[1]*newForce[2];
						pBead->m_Stress[8] += dx[2]*newForce[2];

						// Pass the stress tensor components to the CMonitor
						// for use in analysing the stress over slices.
						// Note that dx[2] might be close to zero and we should
						// check for this.

					// 12/02/10 This is disabled for the parallel code as no analysis is implemented yet
#if EnableParallelSimBox == SimMPSDisabled
						m_pMonitor->AddBeadStress(pBead, *iterBead2, newForce, dx);
#endif
					}
					else
					{
						TraceInt("neighbour cell bead", pBead->GetId());
						TraceInt("interacts with", (*iterBead2)->GetId());
						TraceVector("1st pos",	pBead->GetXPos(),
												pBead->GetYPos(), 
												pBead->GetZPos() );
						TraceVector("2nd pos",	(*iterBead2)->GetXPos(),
												(*iterBead2)->GetYPos(), 
												(*iterBead2)->GetZPos() );
						TraceDouble("Bead distance", dr);
					}
				}
			}
#endif
}


