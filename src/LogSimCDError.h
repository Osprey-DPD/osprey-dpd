// LogSimCDError.h: interface for the CLogSimCDError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSIMCDERROR_H__8DFBFA3E_022E_4C23_A476_957D8B96A85F__INCLUDED_)
#define AFX_LOGSIMCDERROR_H__8DFBFA3E_022E_4C23_A476_957D8B96A85F__INCLUDED_


#include "LogErrorMessage.h"

class CLogSimCDError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSimCDError(long time);

	virtual ~CLogSimCDError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSimCDError& rMsg);

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
	// by declaring them privately but providing NO definitions.

	CLogSimCDError(const CLogSimCDError& oldMessage);
	CLogSimCDError& operator=(const CLogSimCDError& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGSIMCDERROR_H__8DFBFA3E_022E_4C23_A476_957D8B96A85F__INCLUDED_)
