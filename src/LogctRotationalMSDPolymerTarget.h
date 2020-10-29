// LogctRotationalMSDPolymerTarget.h: interface for the CLogctRotationalMSDPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTROTATIONALMSDPOLYMERTARGET_H__C6CFF9B6_1B38_4206_AD17_0929F89EEDB5__INCLUDED_)
#define AFX_LOGCTROTATIONALMSDPOLYMERTARGET_H__C6CFF9B6_1B38_4206_AD17_0929F89EEDB5__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctRotationalMSDPolymerTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctRotationalMSDPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctRotationalMSDPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   double msd);

	virtual ~CLogctRotationalMSDPolymerTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRotationalMSDPolymerTarget& rMsg);

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

	CLogctRotationalMSDPolymerTarget(const CLogctRotationalMSDPolymerTarget& oldMessage);
	CLogctRotationalMSDPolymerTarget& operator=(const CLogctRotationalMSDPolymerTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const long		m_Total;		    // Number of polymers in target
	const double	m_MSD;		        // Final value of rotational MSD of the target

};

#endif // !defined(AFX_LOGCTROTATIONALMSDPOLYMERTARGET_H__C6CFF9B6_1B38_4206_AD17_0929F89EEDB5__INCLUDED_)
