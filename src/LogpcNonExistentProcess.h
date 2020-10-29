// LogpcNonExistentProcess.h: interface for the CLogpcNonExistentProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCNONEXISTENTPROCESS_H__9D203A63_3E67_43D4_BED9_5CFCC773E0B7__INCLUDED_)
#define AFX_LOGPCNONEXISTENTPROCESS_H__9D203A63_3E67_43D4_BED9_5CFCC773E0B7__INCLUDED_


#include "LogWarningMessage.h"

class CLogpcNonExistentProcess : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcNonExistentProcess(long time);

	virtual ~CLogpcNonExistentProcess();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcNonExistentProcess& rMsg);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Public access functions
public:

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

	CLogpcNonExistentProcess(const CLogpcNonExistentProcess& oldMessage);
	CLogpcNonExistentProcess& operator=(const CLogpcNonExistentProcess& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGPCNONEXISTENTPROCESS_H__9D203A63_3E67_43D4_BED9_5CFCC773E0B7__INCLUDED_)
