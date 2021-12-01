// LogctMSDBeadTarget.h: interface for the CLogctMSDBeadTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTMSDBEADTARGET_H__8b694b28_063a_4ed0_8fb7_1ae5afc7fb91__INCLUDED_)
#define AFX_LOGCTMSDBEADTARGET_H__8b694b28_063a_4ed0_8fb7_1ae5afc7fb91__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctMSDBeadTarget : public CLogAnalysisMessage
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctMSDBeadTarget(long time, const zString targetlabel, const zString decLabel,
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctMSDBeadTarget(long time, const zString targetlabel, const zString decLabel,
						   double msd);

	virtual ~CLogctMSDBeadTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctMSDBeadTarget& rMsg);

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

	CLogctMSDBeadTarget(const CLogctMSDBeadTarget& oldMessage);
	CLogctMSDBeadTarget& operator=(const CLogctMSDBeadTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const long		m_Total;		    // Number of Beads in target
	const double	m_MSD;		        // Final value of MSD per Bead

};

#endif // !defined(AFX_LOGCTMSDBEADTARGET_H__8b694b28_063a_4ed0_8fb7_1ae5afc7fb91__INCLUDED_)
