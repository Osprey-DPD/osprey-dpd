// LogctSpringForceOnTarget.h: interface for the CLogctSpringForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSPRINGFORCEONTARGET_H__53336557_b9a1_4ab0_ad8a_c319abbf5e94__INCLUDED_)
#define AFX_LOGCTSPRINGFORCEONTARGET_H__53336557_b9a1_4ab0_ad8a_c319abbf5e94__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSpringForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSpringForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctSpringForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSpringForceOnTarget& rMsg);

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

	CLogctSpringForceOnTarget(const CLogctSpringForceOnTarget& oldMessage);
	CLogctSpringForceOnTarget& operator=(const CLogctSpringForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const double	m_XC;
	const double	m_YC;			// Origin of force as fraction of SimBox sides
	const double	m_ZC;
	const double	m_Magnitude;	// Magnitude of force

};

#endif // !defined(AFX_LOGCTSPRINGFORCEONTARGET_H__53336557_b9a1_4ab0_ad8a_c319abbf5e94__INCLUDED_)
