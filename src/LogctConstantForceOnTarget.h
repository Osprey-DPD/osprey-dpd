// LogctConstantForceOnTarget.h: interface for the CLogctConstantForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCONSTANTFORCEONTARGET_H__89BA9D54_A4A4_46CA_ABB1_66399468B3E9__INCLUDED_)
#define AFX_LOGCTCONSTANTFORCEONTARGET_H__89BA9D54_A4A4_46CA_ABB1_66399468B3E9__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctConstantForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctConstantForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
								long xn, long yn, long zn, double mag);

	virtual ~CLogctConstantForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctConstantForceOnTarget& rMsg);

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

	CLogctConstantForceOnTarget(const CLogctConstantForceOnTarget& oldMessage);
	CLogctConstantForceOnTarget& operator=(const CLogctConstantForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long	    m_X;
	const long	    m_Y;			// Integer direction of force
	const long	    m_Z;
	const double	m_Magnitude;	// Magnitude of force

};

#endif // !defined(AFX_LOGCTCONSTANTFORCEONTARGET_H__89BA9D54_A4A4_46CA_ABB1_66399468B3E9__INCLUDED_)
