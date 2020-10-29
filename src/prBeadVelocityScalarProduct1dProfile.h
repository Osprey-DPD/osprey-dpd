// prBeadVelocityScalarProduct1dProfile.h: interface for the prBeadVelocityScalarProduct1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBEADVELOCITYSCALARPRODUCT1DPROFILE_H__07AC35F4_1C49_4FBB_A427_1572AAD445A8__INCLUDED_)
#define AFX_PRBEADVELOCITYSCALARPRODUCT1DPROFILE_H__07AC35F4_1C49_4FBB_A427_1572AAD445A8__INCLUDED_


// Forward declarations

class CSimState;


#include "xxProcess.h"

class prBeadVelocityScalarProduct1dProfile : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prBeadVelocityScalarProduct1dProfile();

	// Constructor for use by the command that creates this process

	prBeadVelocityScalarProduct1dProfile(const CSimState* const pSimState, 
                              long analysisPeriods, long dataPoints);

	virtual ~prBeadVelocityScalarProduct1dProfile();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// Function to allow process validation when it is created internally

	virtual bool InternalValidateData(const ISimState* const pISimState);


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:								// Data specific to the process

    long  m_TotalAnalysisPeriods;       // No of analysis periods to sample over
    long  m_TotalDataPoints;            // No of data points to output

    // Local data 

    long  m_SamplesTaken;               // Running total of samples in each average
    long  m_SamplesPerDataPoint;        // No of samples per output data point
    long  m_SamplePeriod;               // No of time-steps between samples

    long  m_BeadTypeTotal;              // Number of bead types

    zLongVector m_vBeadTotals;          // Number of beads of each type in samples
    zDoubleVector m_vBeadKE;            // Running average of bead KE

};

#endif // !defined(AFX_PRBEADVELOCITYSCALARPRODUCT1DPROFILE_H__07AC35F4_1C49_4FBB_A427_1572AAD445A8__INCLUDED_)
