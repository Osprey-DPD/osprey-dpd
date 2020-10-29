// LogpmAngularMomentum.h: interface for the CLogpmAngularMomentum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPMANGULARMOMENTUM_H__E9627F72_5CF6_4C24_97F2_DF0DB4296699__INCLUDED_)
#define AFX_LOGPMANGULARMOMENTUM_H__E9627F72_5CF6_4C24_97F2_DF0DB4296699__INCLUDED_


#include "LogInfoMessage.h"

class CLogpmAngularMomentum : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpmAngularMomentum(long time, long beadTotal, double xam, double yam, double zam);

	virtual ~CLogpmAngularMomentum();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpmAngularMomentum& rMsg);

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

	CLogpmAngularMomentum(const CLogpmAngularMomentum& oldMessage);
	CLogpmAngularMomentum& operator=(const CLogpmAngularMomentum& rhs);

	// ****************************************
	// Data members
private:

	const long   m_BeadTotal;  // Number of beads used in calculation
	const double m_XAngMom;
	const double m_YAngMom;    // World angular momentum
	const double m_ZAngMom;


};

#endif // !defined(AFX_LOGPMANGULARMOMENTUM_H__E9627F72_5CF6_4C24_97F2_DF0DB4296699__INCLUDED_)
