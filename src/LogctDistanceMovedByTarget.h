// LogctDistanceMovedByTarget.h: interface for the CLogctDistanceMovedByTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTDISTANCEMOVEDBYTARGET_H__D5086955_8420_4A70_809A_3F772BB4091F__INCLUDED_)
#define AFX_LOGCTDISTANCEMOVEDBYTARGET_H__D5086955_8420_4A70_809A_3F772BB4091F__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctDistanceMovedByTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctDistanceMovedByTarget(long time, const zString targetlabel, const zString forceLabel, const zString decLabel, 
								long startTime, long endTime, const zString name, long type, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctDistanceMovedByTarget(long time, const zString targetlabel, const zString forceLabel, const zString decLabel, 
								double distance, double distancePerBead);

	virtual ~CLogctDistanceMovedByTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctDistanceMovedByTarget& rMsg);

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

	CLogctDistanceMovedByTarget(const CLogctDistanceMovedByTarget& oldMessage);
	CLogctDistanceMovedByTarget& operator=(const CLogctDistanceMovedByTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_ForceLabel;		// Force decorator label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const zString	m_BeadName;			// Name of bead type in target
	const long		m_BeadType;			// Integer identifier for bead type
	const long		m_BeadTotal;		// Number of beads of specified type in target
	const double	m_Distance;			// Accumulated distance moved by target beads
	const double	m_DistancePerBead;	// Average distance moved by target beads

};

#endif // !defined(AFX_LOGCTDISTANCEMOVEDBYTARGET_H__D5086955_8420_4A70_809A_3F772BB4091F__INCLUDED_)
