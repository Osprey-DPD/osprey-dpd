// prCompositeTargetBeadRDF.h: interface for the prCompositeTargetBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRCompositeTargetBeadRDF_H__006594ed_437a_497b_be01_ba455475c9d7__INCLUDED_)
#define AFX_PRCompositeTargetBeadRDF_H__006594ed_437a_497b_be01_ba455475c9d7__INCLUDED_


// Forward declarations

class CCommandTargetComposite;
class CSimState;


#include "xxProcess.h"

class prCompositeTargetBeadRDF : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prCompositeTargetBeadRDF();

	// Constructor for use by the command that creates this process

	prCompositeTargetBeadRDF(const CSimState* const pSimState,
					 long analysisPeriods,
					 long dataPoints, double rMax,
                     zString targetName, long beadType, CCommandTargetComposite* const pTarget);

	virtual ~prCompositeTargetBeadRDF();

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
    zString  m_TargetName;	            // String identifier of the composite target to analyse
    long  m_BeadType;					// Numeric identified of bead in polymer to analyse
    CCommandTargetComposite*   m_pTarget;   // Composite target whose beads are to be analysed
    
    double m_RMax; 						// Maximum distance out to which histogram is calculated
    
// Local data

    long  m_SamplePeriod;               // No of time-steps between samples
	long  m_SampleTotal;				// Total number of samples to take
	long  m_SamplesTaken;				// Number of samples taken
	
	double m_dr;						// Radius of the spherical shells
	
	BeadVector  m_vBeads;				// Set of beads whose RDF is calculated
	
	zDoubleVector  m_vRDF;				// Vector of values in the RDF
};

#endif // !defined(AFX_PRCompositeTargetBeadRDF_H__006594ed_437a_497b_be01_ba455475c9d7__INCLUDED_)
