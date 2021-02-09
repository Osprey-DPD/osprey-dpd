// LogSavePolymerBeadRDF.h: interface for the CLogSavePolymerBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEPOLYMERBEADRDF_H__fd31aedc_0092_4b0c_b4d2_41dd0be77174__INCLUDED_)
#define AFX_LOGSAVEPOLYMERBEADRDF_H__fd31aedc_0092_4b0c_b4d2_41dd0be77174__INCLUDED_


#include "LogInfoMessage.h"

class CLogSavePolymerBeadRDF : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSavePolymerBeadRDF(long time, long analysisPeriods,
						   long dataPoints, long start, long end,
						   long samplePeriod, double rMax, const zString polymerName, const zString beadName);

	virtual ~CLogSavePolymerBeadRDF();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSavePolymerBeadRDF& rMsg);

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

	CLogSavePolymerBeadRDF(const CLogSavePolymerBeadRDF& oldMessage);
	CLogSavePolymerBeadRDF& operator=(const CLogSavePolymerBeadRDF& rhs);


	// ****************************************
	// Data members
private:

    const long  m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long  m_TotalDataPoints;      // No of data points in histogram

	const long	m_Start;		        // Time at which analysis starts
	const long	m_End;			        // Time at which analysis ends
	const long  m_SamplePeriod;         // Simulation sample period

	const double m_RMax;				// Maximum distance out to which histogram is calculated
	const zString m_PolymerName;		// Polymer being analysed
	const zString m_BeadName;			// Bead within polymer whose RDF is calculated
};

#endif // !defined(AFX_LOGSAVEPOLYMERBEADRDF_H__fd31aedc_0092_4b0c_b4d2_41dd0be77174__INCLUDED_)
