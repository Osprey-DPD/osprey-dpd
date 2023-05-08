// prSAXS.h: interface for the prSAXS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRSAXS_H__61400260_244d_4846_9515_83b1ff18a708__INCLUDED_)
#define AFX_PRSAXS_H__61400260_244d_4846_9515_83b1ff18a708__INCLUDED_


// Forward declarations

class CSimState;


#include "xxProcess.h"


class prSAXS : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prSAXS();

	// Constructor for use by the commands that create this process

    prSAXS(const CSimState* const pSimState, long analysisPeriods, long qPoints, LongLongMap mBeadTypes);

    prSAXS(const CSimState* const pSimState, long analysisPeriods, long qPoints, double qMin, double qMax, LongLongMap mBeadTypes);

	virtual ~prSAXS();

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

    bool SetBeadTypeElectronNo(long beadType, double eno);
    
    
	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	void UpdateState_InnerLoop_ref(CSimState& rSimState, const ISimBox* const pISimBox);
	void UpdateState_InnerLoop_v1(CSimState& rSimState, const ISimBox* const pISimBox);
	void UpdateState_InnerLoop_v2(CSimState& rSimState, const ISimBox* const pISimBox);
	void UpdateState_InnerLoop_v3(CSimState& rSimState, const ISimBox* const pISimBox);
	void UpdateState_InnerLoop_v4(CSimState& rSimState, const ISimBox* const pISimBox);
	void UpdateState_InnerLoop_v4_threads(CSimState& rSimState, const ISimBox* const pISimBox);
	#if OSPREY_DPD_HAVE_OPENMP
	void UpdateState_InnerLoop_v4_omp(CSimState& rSimState, const ISimBox* const pISimBox);
	#endif
	#if OSPREY_DPD_HAVE_STL_PARALLEL
	void UpdateState_InnerLoop_v4_stl_parallel(CSimState& rSimState, const ISimBox* const pISimBox);
	#endif

	double DistanceMetric(const CAbstractBead *a, const CAbstractBead *b) const;
	float DistanceMetric(const float *a, const float *b) const;

	// ****************************************
	// Data members

private:								// Data specific to the process

    long         m_AnalysisPeriods;     // No of analysis periods to sample over
    long         m_QPoints;             // Number of q values to use in I(q)
    double       m_QMin;                // Minimum q value in range (inverse Angstrom)
    double       m_QMax;                // Maximum q value in range (inverse Angstrom)
    const double m_dQ;                  // Increment in the scattering wave vector magnitude q (inverse Angstrom)

    LongLongMap  m_mBeadTypes;          // Map of the bead numeric type ids to include in calculation
    
// Local data

    long  m_SamplePeriod;               // No of time-steps between samples
	long  m_SampleTotal;				// Total number of samples to take
	long  m_SamplesTaken;				// Number of samples taken
	
	BeadVector  m_vBeads;				// Set of beads whose RDF is calculated
	
    zDoubleVector  m_vIQ;               // Scattering function I(q)
    zDoubleVector  m_vIQSq;             // Sqaure of the scattering function I(q)*I(q)

    LongDoubleMap  m_mElectronNo;       // Map of bead type to electron number
};

#endif // !defined(AFX_PRSAXS_H__61400260_244d_4846_9515_83b1ff18a708__INCLUDED_)
