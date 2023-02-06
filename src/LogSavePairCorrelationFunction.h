// LogSavePairCorrelationFunction.h: interface for the CLogSavePairCorrelationFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEPAIRCORRELATIONFUNCTION_H__8dd83e29_33bd_4d28_8ff2_fa60781e91ad__INCLUDED_)
#define AFX_LOGSAVEPAIRCORRELATIONFUNCTION_H__8dd83e29_33bd_4d28_8ff2_fa60781e91ad__INCLUDED_


#include "LogInfoMessage.h"

class CLogSavePairCorrelationFunction : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSavePairCorrelationFunction(long time, long analysisPeriods,
						   long dataPoints, long start, long end,
						   long samplePeriod, double rMax, zBoolVector vExcludedPolymers);

	virtual ~CLogSavePairCorrelationFunction();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSavePairCorrelationFunction& rMsg);

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

	CLogSavePairCorrelationFunction(const CLogSavePairCorrelationFunction& oldMessage);
	CLogSavePairCorrelationFunction& operator=(const CLogSavePairCorrelationFunction& rhs);


	// ****************************************
	// Data members
private:

    const long  m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long  m_TotalDataPoints;      // No of data points in histogram

	const long	m_Start;		        // Time at which analysis starts
	const long	m_End;			        // Time at which analysis ends
	const long  m_SamplePeriod;         // Simulation sample period

	const double m_RMax;				// Maximum distance out to which histogram is calculated
    zBoolVector  m_vExcludedPolymers;   // Polymers to exclude from the calculation

};

#endif // !defined(AFX_LOGSAVEPAIRCORRELATIONFUNCTION_H__8dd83e29_33bd_4d28_8ff2_fa60781e91ad__INCLUDED_)
