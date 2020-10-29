// LogctConstantSpecificAreaGhostTimeSeries.h: interface for the CLogctConstantSpecificAreaGhostTimeSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOSTTIMESERIES_H__414d0779_a875_49b3_b5f2_41665dddccdb__INCLUDED_)
#define AFX_LOGCTCONSTANTSPECIFICAREAGHOSTTIMESERIES_H__414d0779_a875_49b3_b5f2_41665dddccdb__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctConstantSpecificAreaGhostTimeSeries : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctConstantSpecificAreaGhostTimeSeries(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctConstantSpecificAreaGhostTimeSeries(long time, const zString targetlabel, const zString decLabel);

	virtual ~CLogctConstantSpecificAreaGhostTimeSeries();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctConstantSpecificAreaGhostTimeSeries& rMsg);

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

	CLogctConstantSpecificAreaGhostTimeSeries(const CLogctConstantSpecificAreaGhostTimeSeries& oldMessage);
	CLogctConstantSpecificAreaGhostTimeSeries& operator=(const CLogctConstantSpecificAreaGhostTimeSeries& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const long		m_Total;		    // Number of polymers in target
    // need to add the time series data here

};

#endif // !defined(AFX_LOGCTCONSTANTSPECIFICAREAGHOSTTIMESERIES_H__414d0779_a875_49b3_b5f2_41665dddccdb__INCLUDED_)
