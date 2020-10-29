// LogctSineForceOnTarget.h: interface for the CLogctSineForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSINEFORCEONTARGET_H__DB1F21C6_56CD_4FB0_BA0A_3AEF13FE405D__INCLUDED_)
#define AFX_LOGCTSINEFORCEONTARGET_H__DB1F21C6_56CD_4FB0_BA0A_3AEF13FE405D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSineForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSineForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							long xn, long yn, long zn, 
							double amp, double period, long startTime);

	virtual ~CLogctSineForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSineForceOnTarget& rMsg);

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

	CLogctSineForceOnTarget(const CLogctSineForceOnTarget& oldMessage);
	CLogctSineForceOnTarget& operator=(const CLogctSineForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long      m_X;
	const long      m_Y;			// Integer direction of force
	const long      m_Z;
	const double	m_Amplitude;	// Amplitude of force
	const double  	m_Period;	    // Period of force
	const long      m_StartTime;	// SimTime at which force is first applied

};

#endif // !defined(AFX_LOGCTSINEFORCEONTARGET_H__DB1F21C6_56CD_4FB0_BA0A_3AEF13FE405D__INCLUDED_)
