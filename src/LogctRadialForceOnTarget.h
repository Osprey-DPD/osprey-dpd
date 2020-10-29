// LogctRadialForceOnTarget.h: interface for the CLogctRadialForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTRADIALFORCEONTARGET_H__FD20EE3D_81F3_4E9D_9670_01CCBE7F3B2E__INCLUDED_)
#define AFX_LOGCTRADIALFORCEONTARGET_H__FD20EE3D_81F3_4E9D_9670_01CCBE7F3B2E__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctRadialForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctRadialForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctRadialForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRadialForceOnTarget& rMsg);

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

	CLogctRadialForceOnTarget(const CLogctRadialForceOnTarget& oldMessage);
	CLogctRadialForceOnTarget& operator=(const CLogctRadialForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long      m_X;
	const long      m_Y;			// Integer direction of force
	const long      m_Z;
	const double	m_XC;
	const double	m_YC;			// Origin of force as fraction of SimBox sides
	const double	m_ZC;
	const double	m_Magnitude;	// Magnitude of force

};

#endif // !defined(AFX_LOGCTRADIALFORCEONTARGET_H__FD20EE3D_81F3_4E9D_9670_01CCBE7F3B2E__INCLUDED_)
