// LogctTranslateTargetToC1Point.h: interface for the CLogctTranslateTargetToC1Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTRANSLATETARGETTOC1POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_)
#define AFX_LOGCTTRANSLATETARGETTOC1POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctTranslateTargetToC1Point : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTranslateTargetToC1Point(long time, const zString targetlabel, double xc, double yc, double zc);

	virtual ~CLogctTranslateTargetToC1Point();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTranslateTargetToC1Point& rMsg);

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

	CLogctTranslateTargetToC1Point(const CLogctTranslateTargetToC1Point& oldMessage);
	CLogctTranslateTargetToC1Point& operator=(const CLogctTranslateTargetToC1Point& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const double	m_XC;			// Fractional distance along the X dimension of the point
	const double	m_YC;			// Fractional distance along the Y dimension of the point
	const double	m_ZC;			// Fractional distance along the Z dimension of the point

};

#endif // !defined(AFX_LOGCTTRANSLATETARGETTOC1POINT_H__b1afb91f_e3b0_44de_bfef_9f8abd19d9b4__INCLUDED_)
