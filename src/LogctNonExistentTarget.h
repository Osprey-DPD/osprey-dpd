// LogctNonExistentTarget.h: interface for the CLogctNonExistentTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTNONEXISTENTTARGET_H__1612DD90_D75B_4DDF_9435_9D3C0EBC59CD__INCLUDED_)
#define AFX_LOGCTNONEXISTENTTARGET_H__1612DD90_D75B_4DDF_9435_9D3C0EBC59CD__INCLUDED_


#include "LogWarningMessage.h"

class CLogctNonExistentTarget : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctNonExistentTarget(long time, const zString label);

	virtual ~CLogctNonExistentTarget();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctNonExistentTarget& rMsg);

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

	// ****************************************
	// Data members
private:


	const zString m_TargetLabel;	// Label for a command target instance

};

#endif // !defined(AFX_LOGCTNONEXISTENTTARGET_H__1612DD90_D75B_4DDF_9435_9D3C0EBC59CD__INCLUDED_)
