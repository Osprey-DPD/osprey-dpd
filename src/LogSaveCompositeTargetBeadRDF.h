// LogSaveCompositeTargetBeadRDF.h: interface for the CLogSaveCompositeTargetBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LogSaveCompositeTargetBeadRDF_H__4cbcdd86_025f_4250_befd_b0e8cd7b5225__INCLUDED_)
#define AFX_LogSaveCompositeTargetBeadRDF_H__4cbcdd86_025f_4250_befd_b0e8cd7b5225__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveCompositeTargetBeadRDF : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveCompositeTargetBeadRDF(long time, long analysisPeriods,
						   long dataPoints, long start, long end,
						   long samplePeriod, double rMax, const zString targetName, const zString beadName);

	virtual ~CLogSaveCompositeTargetBeadRDF();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveCompositeTargetBeadRDF& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogSaveCompositeTargetBeadRDF(const CLogSaveCompositeTargetBeadRDF& oldMessage);
	CLogSaveCompositeTargetBeadRDF& operator=(const CLogSaveCompositeTargetBeadRDF& rhs);


	// ****************************************
	// Data members
private:

    const long  m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long  m_TotalDataPoints;      // No of data points in histogram

	const long	m_Start;		        // Time at which analysis starts
	const long	m_End;			        // Time at which analysis ends
	const long  m_SamplePeriod;         // Simulation sample period

	const double m_RMax;				// Maximum distance out to which histogram is calculated
	const zString m_TargetName;	    	// Composite target being analysed
	const zString m_BeadName;			// Bead within polymer whose RDF is calculated
};

#endif // !defined(AFX_LogSaveCompositeTargetBeadRDF_H__4cbcdd86_025f_4250_befd_b0e8cd7b5225__INCLUDED_)
