// LogctRgBeadTarget.h: interface for the CLogctRgBeadTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTRGBEADTARGET_H__1e0934e5_6d8d_48e6_a544_45c39275f9ab__INCLUDED_)
#define AFX_LOGCTRGBEADTARGET_H__1e0934e5_6d8d_48e6_a544_45c39275f9ab__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctRgBeadTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctRgBeadTarget(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctRgBeadTarget(long time, const zString targetlabel, const zString decLabel, 
						   double xcm, double ycm, double zcm, double rg);

	virtual ~CLogctRgBeadTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRgBeadTarget& rMsg);

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

	CLogctRgBeadTarget(const CLogctRgBeadTarget& oldMessage);
	CLogctRgBeadTarget& operator=(const CLogctRgBeadTarget& rhs);


	// ****************************************
	// Data members
private:

	const bool		m_bStart;			// Flag showing if calculation is beginning or ending
	const zString	m_TargetLabel;		// Command target label
	const zString	m_DecLabel;			// Command wrapper label 
	const long		m_StartTime;		// Start of measuring interval
	const long		m_EndTime;			// End of measuring intervaL

	const long		m_Total;		    // Number of polymers in target
    const double    m_XCM;            // Final value of X CM of polymers
    const double    m_YCM;            // Final value of Y CM of polymers
    const double    m_ZCM;            // Final value of Z CM of polymers
	const double	m_Rg;		        // Final value of radius of gyration of polymers

};

#endif // !defined(AFX_LOGCTRGBEADTARGET_H__1e0934e5_6d8d_48e6_a544_45c39275f9ab__INCLUDED_)
