// LogpmCMVelocity.h: interface for the CLogpmCMVelocity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPMCMVELOCITY_H__E7FAD783_D0FA_4A4C_A51A_C90A452FAF9A__INCLUDED_)
#define AFX_LOGPMCMVELOCITY_H__E7FAD783_D0FA_4A4C_A51A_C90A452FAF9A__INCLUDED_


#include "LogInfoMessage.h"

class CLogpmCMVelocity : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpmCMVelocity(long time, long beadTotal, double xcmv, double ycmv, double zcmv);

	virtual ~CLogpmCMVelocity();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpmCMVelocity& rMsg);

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

	CLogpmCMVelocity(const CLogpmCMVelocity& oldMessage);
	CLogpmCMVelocity& operator=(const CLogpmCMVelocity& rhs);

	// ****************************************
	// Data members
private:

	const long   m_BeadTotal;  // Number of beads used in calculation
	const double m_XCMV;
	const double m_YCMV;  // World CM velocity
	const double m_ZCMV;
};

#endif // !defined(AFX_LOGPMCMVELOCITY_H__E7FAD783_D0FA_4A4C_A51A_C90A452FAF9A__INCLUDED_)
