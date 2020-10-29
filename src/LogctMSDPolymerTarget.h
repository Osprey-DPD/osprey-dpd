// LogctMSDPolymerTarget.h: interface for the CLogctMSDPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTMSDPOLYMERTARGET_H__BC38DE32_9D7E_4FAB_9D65_DCD94D852BAE__INCLUDED_)
#define AFX_LOGCTMSDPOLYMERTARGET_H__BC38DE32_9D7E_4FAB_9D65_DCD94D852BAE__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctMSDPolymerTarget : public CLogAnalysisMessage
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctMSDPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctMSDPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   double msd);

	virtual ~CLogctMSDPolymerTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctMSDPolymerTarget& rMsg);

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

	CLogctMSDPolymerTarget(const CLogctMSDPolymerTarget& oldMessage);
	CLogctMSDPolymerTarget& operator=(const CLogctMSDPolymerTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const long		m_Total;		    // Number of polymers in target
	const double	m_MSD;		        // Final value of MSD per polymer

};

#endif // !defined(AFX_LOGCTMSDPOLYMERTARGET_H__BC38DE32_9D7E_4FAB_9D65_DCD94D852BAE__INCLUDED_)
