// LogctEdgeClampForceOnTarget.h: interface for the CLogctEdgeClampForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTEDGECLAMPFORCEONTARGET_H__d3e034fc_89e5_4e9a_b09a_efd0c8e6e61a__INCLUDED_)
#define AFX_LOGCTEDGECLAMPFORCEONTARGET_H__d3e034fc_89e5_4e9a_b09a_efd0c8e6e61a__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctEdgeClampForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctEdgeClampForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctEdgeClampForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctEdgeClampForceOnTarget& rMsg);

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

	CLogctEdgeClampForceOnTarget(const CLogctEdgeClampForceOnTarget& oldMessage);
	CLogctEdgeClampForceOnTarget& operator=(const CLogctEdgeClampForceOnTarget& rhs);


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

#endif // !defined(AFX_LOGCTEDGECLAMPFORCEONTARGET_H__d3e034fc_89e5_4e9a_b09a_efd0c8e6e61a__INCLUDED_)
