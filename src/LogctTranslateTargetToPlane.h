// LogctTranslateTargetToZPlane.h: interface for the CLogctTranslateTargetToPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTRANSLATETARGETTOPLANE_H__cd888c03_88bd_469a_bef3_92d31a6bc09c__INCLUDED_)
#define AFX_LOGCTTRANSLATETARGETTOPLANE_H__cd888c03_88bd_469a_bef3_92d31a6bc09c__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctTranslateTargetToPlane : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTranslateTargetToPlane(long time, const zString targetlabel, const zString decLabel, 
							  long xn, long yn, long zn, 
							  double xc, double yc, double zc);

	virtual ~CLogctTranslateTargetToPlane();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTranslateTargetToPlane& rMsg);

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

	CLogctTranslateTargetToPlane(const CLogctTranslateTargetToPlane& oldMessage);
	CLogctTranslateTargetToPlane& operator=(const CLogctTranslateTargetToPlane& rhs);


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

};

#endif // !defined(AFX_LOGCTTRANSLATETARGETTOPLANE_H__cd888c03_88bd_469a_bef3_92d31a6bc09c__INCLUDED_)
