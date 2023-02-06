// prPairCorrelationFunction.h: interface for the prPairCorrelationFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRPAIRCORRELATIONFUNCTION_H__cea70618_9e49_4b90_8cf4_3ada0dad639d__INCLUDED_)
#define AFX_PRPAIRCORRELATIONFUNCTION_H__cea70618_9e49_4b90_8cf4_3ada0dad639d__INCLUDED_


// Forward declarations

class CSimState;


#include "xxProcess.h"

class prPairCorrelationFunction : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prPairCorrelationFunction();

	// Constructor for use by the command that creates this process

	prPairCorrelationFunction(const CSimState* const pSimState,
					 long analysisPeriods,
					 long dataPoints, double rMax,
					 LongLongMap mPolyTypes);

	virtual ~prPairCorrelationFunction();

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

    // Function to integrate the RDF and produce I(q)
    
    void TrapezoidalRule(const double dh);

	// ****************************************
	// Data members

private:								// Data specific to the process

    long  m_AnalysisPeriods;       		// No of analysis periods to sample over
    long  m_DataPoints;            		// No of bins in the histogram


    double m_RMax; 						// Maximum distance out to which histogram is calculated
    
    LongLongMap  m_mPolyTypes;          // Map of the polymer numeric type ids to include in calculation
    
// Local data

    long  m_SamplePeriod;               // No of time-steps between samples
	long  m_SampleTotal;				// Total number of samples to take
	long  m_SamplesTaken;				// Number of samples taken
    long  m_QPoints;                    // Number of q values to use in I(q)
	
    double m_dr;                        // Radius of the spherical shells

	BeadVector  m_vBeads;				// Set of beads whose RDF is calculated
	
	zDoubleVector  m_vRDF;				// Vector of values in the RDF
    zDoubleVector  m_vIQ;               // Scattering function I(q)
};

#endif // !defined(AFX_PRPAIRCORRELATIONFUNCTION_H__cea70618_9e49_4b90_8cf4_3ada0dad639d__INCLUDED_)
