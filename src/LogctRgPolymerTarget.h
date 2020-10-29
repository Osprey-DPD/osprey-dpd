// LogctRgPolymerTarget.h: interface for the CLogctRgPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTRGPOLYMERTARGET_H__adacb951_4092_4fa9_b91a_e972c2830f1e__INCLUDED_)
#define AFX_LOGCTRGPOLYMERTARGET_H__adacb951_4092_4fa9_b91a_e972c2830f1e__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctRgPolymerTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor to log the start of the measurement interval

	CLogctRgPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   long startTime, long endTime, long total);

	// Constructor to log the results at the end of the measurement interval

	CLogctRgPolymerTarget(long time, const zString targetlabel, const zString decLabel, 
						   double xcm, double ycm, double zcm, double rg);

	virtual ~CLogctRgPolymerTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRgPolymerTarget& rMsg);

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

	CLogctRgPolymerTarget(const CLogctRgPolymerTarget& oldMessage);
	CLogctRgPolymerTarget& operator=(const CLogctRgPolymerTarget& rhs);


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

#endif // !defined(AFX_LOGCTRGPOLYMERTARGET_H__adacb951_4092_4fa9_b91a_e972c2830f1e__INCLUDED_)
