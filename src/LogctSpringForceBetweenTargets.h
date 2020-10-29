// LogctSpringForceOnTarget.h: interface for the CLogctSpringForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSPRINGFORCEBETWEENTARGETS_H__b36226c5_bfbf_4b13_bcf2_3c90d54537a1__INCLUDED_)
#define AFX_LOGCTSPRINGFORCEBETWEENTARGETS_H__b36226c5_bfbf_4b13_bcf2_3c90d54537a1__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSpringForceBetweenTargets : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSpringForceBetweenTargets(long time, const zString target1label, const zString target2label, const zString decLabel, 
							  double mag, double deltax, long period);

	virtual ~CLogctSpringForceBetweenTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSpringForceBetweenTargets& rMsg);

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

	CLogctSpringForceBetweenTargets(const CLogctSpringForceBetweenTargets& oldMessage);
	CLogctSpringForceBetweenTargets& operator=(const CLogctSpringForceBetweenTargets& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_Target1Label;	// Command target 1 label
	const zString	m_Target2Label;	// Command target 2 label
	const zString	m_DecLabel;		// Command wrapper label 
	const double	m_Magnitude;	// Magnitude of spring force
    const double    m_DeltaX;       // Step change in the target separation
    const long      m_Period;       // Number of time steps between step changes in separation

};

#endif // !defined(AFX_LOGCTSPRINGFORCEBETWEENTARGETS_H__b36226c5_bfbf_4b13_bcf2_3c90d54537a1__INCLUDED_)
