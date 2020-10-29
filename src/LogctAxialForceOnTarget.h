// LogctAxialForceOnTarget.h: interface for the CLogctAxialForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTAXIALFORCEONTARGET_H__D2974F93_F1D9_44CB_A1CD_BE6FCDD6A32B__INCLUDED_)
#define AFX_LOGCTAXIALFORCEONTARGET_H__D2974F93_F1D9_44CB_A1CD_BE6FCDD6A32B__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctAxialForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctAxialForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctAxialForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctAxialForceOnTarget& rMsg);

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

	CLogctAxialForceOnTarget(const CLogctAxialForceOnTarget& oldMessage);
	CLogctAxialForceOnTarget& operator=(const CLogctAxialForceOnTarget& rhs);


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

#endif // !defined(AFX_LOGCTAXIALFORCEONTARGET_H__D2974F93_F1D9_44CB_A1CD_BE6FCDD6A32B__INCLUDED_)
