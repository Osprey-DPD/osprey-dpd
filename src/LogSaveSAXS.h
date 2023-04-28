// LogSaveSAXS.h: interface for the CLogSaveSAXS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVESAXS_H__6b74dcf4_24e3_4aab_ac59_fd17b11ed9e3__INCLUDED_)
#define AFX_LOGSAVESAXS_H__6b74dcf4_24e3_4aab_ac59_fd17b11ed9e3__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveSAXS : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveSAXS(long time, long analysisPeriods,
                 long dataPoints, double qMin, double qMax, long start, long end,
                 long samplePeriod, zBoolVector vIncludedBeads);

	virtual ~CLogSaveSAXS();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveSAXS& rMsg);

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

	CLogSaveSAXS(const CLogSaveSAXS& oldMessage);
	CLogSaveSAXS& operator=(const CLogSaveSAXS& rhs);


	// ****************************************
	// Data members
private:

    const long   m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long   m_TotalDataPoints;      // No of q values in I(q)
    const double m_QMin;                 // Minimum q value in range
    const double m_QMax;                 // Maximum q value in range

	const long	 m_Start;		         // Time at which analysis starts
	const long	 m_End;			         // Time at which analysis ends
	const long   m_SamplePeriod;         // Simulation sample period

    zBoolVector  m_vIncludedBeads;       // Beads to include in the calculation

};

#endif // !defined(AFX_LOGSAVESAXS_H__6b74dcf4_24e3_4aab_ac59_fd17b11ed9e3__INCLUDED_)
