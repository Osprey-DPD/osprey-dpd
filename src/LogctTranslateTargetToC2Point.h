// LogctTranslateTargetToC2Point.h: interface for the CLogctTranslateTargetToC2Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTRANSLATETARGETTOC2POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_)
#define AFX_LOGCTTRANSLATETARGETTOC2POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctTranslateTargetToC2Point : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTranslateTargetToC2Point(long time, const zString targetlabel, long xn, long yn, long zn, double xc, double yc, double zc, double separation);

	virtual ~CLogctTranslateTargetToC2Point();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTranslateTargetToC2Point& rMsg);

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

	CLogctTranslateTargetToC2Point(const CLogctTranslateTargetToC2Point& oldMessage);
	CLogctTranslateTargetToC2Point& operator=(const CLogctTranslateTargetToC2Point& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
    const long      m_XN;           // X component of C2 axis 
    const long      m_YN;           // Y component of C2 axis 
    const long      m_ZN;           // Z component of C2 axis 
	const double	m_XC;			// Fractional distance along the X dimension of the point
	const double	m_YC;			// Fractional distance along the Y dimension of the point
	const double	m_ZC;			// Fractional distance along the Z dimension of the point
    const double    m_Separation;   // Separation of head beads along symmetry axis

};

#endif // !defined(AFX_LOGCTTRANSLATETARGETTOC2POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_)
