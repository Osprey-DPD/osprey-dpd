// prPolymerBeadRDF.h: interface for the prPolymerBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRPOLYMERBEADRDF_H__cbdecf22_41a0_4085_9e2a_eeef73cd0edb__INCLUDED_)
#define AFX_PRPOLYMERBEADRDF_H__cbdecf22_41a0_4085_9e2a_eeef73cd0edb__INCLUDED_


// Forward declarations

class CSimState;


#include "xxProcess.h"

class prPolymerBeadRDF : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prPolymerBeadRDF();

	// Constructor for use by the command that creates this process

	prPolymerBeadRDF(const CSimState* const pSimState,
					 long analysisPeriods,
					 long dataPoints, double rMax,
					 long polymerType, long beadType);

	virtual ~prPolymerBeadRDF();

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

    long  m_AnalysisPeriods;       		// No of analysis periods to sample over
    long  m_DataPoints;            		// No of bins in the histogram
    long  m_PolymerType;				// Numeric identifier of polymer to analyse
    long  m_BeadType;					// Numeric identified of bead in polymer to analyse

    double m_RMax; 						// Maximum distance out to which histogram is calculated
    
// Local data

    long  m_SamplePeriod;               // No of time-steps between samples
	long  m_SampleTotal;				// Total number of samples to take
	long  m_SamplesTaken;				// Number of samples taken
	
	double m_dr;						// Radius of the spherical shells
	
	BeadVector  m_vBeads;				// Set of beads whose RDF is calculated
	
	zDoubleVector  m_vRDF;				// Vector of values in the RDF
};

#endif // !defined(AFX_PRPOLYMERBEADRDF_H__cbdecf22_41a0_4085_9e2a_eeef73cd0edb__INCLUDED_)
