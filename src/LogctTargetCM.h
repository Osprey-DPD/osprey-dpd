// LogctTargetCM.h: interface for the CLogctTargetCM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETCM_H__4480A877_0EA1_4577_BED6_148229C137D8__INCLUDED_)
#define AFX_LOGCTTARGETCM_H__4480A877_0EA1_4577_BED6_148229C137D8__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctTargetCM : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctTargetCM(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctTargetCM(long time, const zString targetlabel, const zString decLabel);

	virtual ~CLogctTargetCM();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetCM& rMsg);

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

	CLogctTargetCM(const CLogctTargetCM& oldMessage);
	CLogctTargetCM& operator=(const CLogctTargetCM& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL
	const long		m_Total;		    // Number of beads in target
};

#endif // !defined(AFX_LOGCTTARGETCM_H__4480A877_0EA1_4577_BED6_148229C137D8__INCLUDED_)
