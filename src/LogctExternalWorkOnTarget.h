// LogctExternalWorkOnTarget.h: interface for the CLogctExternalWorkOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTEXTERNALWORKONTARGET_H__3A06CFA4_2503_4830_96BA_1E2B9E74312D__INCLUDED_)
#define AFX_LOGCTEXTERNALWORKONTARGET_H__3A06CFA4_2503_4830_96BA_1E2B9E74312D__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctExternalWorkOnTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctExternalWorkOnTarget(long time, const zString targetlabel, const zString forceLabel, const zString decLabel, 
							   long startTime, long endTime, const zString name, long type, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctExternalWorkOnTarget(long time, const zString targetlabel, const zString forceLabel, 
							   const zString decLabel, double work);

	virtual ~CLogctExternalWorkOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctExternalWorkOnTarget& rMsg);

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

	CLogctExternalWorkOnTarget(const CLogctExternalWorkOnTarget& oldMessage);
	CLogctExternalWorkOnTarget& operator=(const CLogctExternalWorkOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool	m_bStart;	// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;	// Command target label
	const zString	m_ForceLabel;	// Force decorator label
	const zString	m_DecLabel;	// Command wrapper label 
	const long	m_StartTime;	// Start of measuring interval
	const long	m_EndTime;	// End of measuring intervaL

	const zString	m_BeadName;	// Name of bead type in target
	const long	m_BeadType;	// Integer identifier for bead type
	const long	m_BeadTotal;	// Number of beads of specified type in target
	const double	m_Work;	// Work done on target beads

};
#endif // !defined(AFX_LOGCTEXTERNALWORKONTARGET_H__3A06CFA4_2503_4830_96BA_1E2B9E74312D__INCLUDED_)
