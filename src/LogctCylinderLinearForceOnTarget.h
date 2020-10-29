// LogctCylinderLinearForceOnTarget.h: interface for the CLogctCylinderLinearForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCYLINDERLINEARFORCEONTARGET_H__E1969EED_6E33_4C02_8550_40E0C2E44648__INCLUDED_)
#define AFX_LOGCTCYLINDERLINEARFORCEONTARGET_H__E1969EED_6E33_4C02_8550_40E0C2E44648__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctCylinderLinearForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctCylinderLinearForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctCylinderLinearForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctCylinderLinearForceOnTarget& rMsg);

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

	CLogctCylinderLinearForceOnTarget(const CLogctCylinderLinearForceOnTarget& oldMessage);
	CLogctCylinderLinearForceOnTarget& operator=(const CLogctCylinderLinearForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long	    m_X;
	const long	    m_Y;			// Normal vector along cylionder axis
	const long	    m_Z;
	const double	m_XC;
	const double	m_YC;			// Origin of force as fraction of SimBox sides
	const double	m_ZC;
	const double	m_Magnitude;	// Magnitude of force

};

#endif // !defined(AFX_LOGCTCYLINDERLINEARFORCEONTARGET_H__E1969EED_6E33_4C02_8550_40E0C2E44648__INCLUDED_)
