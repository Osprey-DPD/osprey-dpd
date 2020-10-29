// LogctPlanarAnchorForceOnTarget.h: interface for the CLogctPlanarAnchorForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTPLANARANCHORFORCEONTARGET_H__545b77b6_75e9_4bad_89ed_0fa53dfa64b9__INCLUDED_)
#define AFX_LOGCTPLANARANCHORFORCEONTARGET_H__545b77b6_75e9_4bad_89ed_0fa53dfa64b9__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctPlanarAnchorForceOnTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctPlanarAnchorForceOnTarget(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc, double mag);

	virtual ~CLogctPlanarAnchorForceOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctPlanarAnchorForceOnTarget& rMsg);

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

	CLogctPlanarAnchorForceOnTarget(const CLogctPlanarAnchorForceOnTarget& oldMessage);
	CLogctPlanarAnchorForceOnTarget& operator=(const CLogctPlanarAnchorForceOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long	    m_X;
	const long	    m_Y;			// Normal vector to the plane to which the particle is anchored
	const long	    m_Z;
	const double	m_XC;
	const double	m_YC;			// Coordinates of an arbitrary point in the plane as fractions of SimBox sides
	const double	m_ZC;
	const double	m_Spring;	    // Spring constant of the Hookean restoring force

};

#endif // !defined(AFX_LOGCTPLANARANCHORFORCEONTARGET_H__545b77b6_75e9_4bad_89ed_0fa53dfa64b9__INCLUDED_)
