// LogActiveEventError.h: interface for the CLogActiveEventError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTIVEEVENTERROR_H__AE46AA65_7574_484D_AA6C_275368371237__INCLUDED_)
#define AFX_LOGACTIVEEVENTERROR_H__AE46AA65_7574_484D_AA6C_275368371237__INCLUDED_


#include "LogErrorMessage.h"

class CLogActiveEventError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogActiveEventError(long time, const zString message);

	virtual ~CLogActiveEventError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogActiveEventError& rMsg);

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

	CLogActiveEventError(const CLogActiveEventError& oldMessage);
	CLogActiveEventError& operator=(const CLogActiveEventError& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Message;	// Message describing the error condition

};

#endif // !defined(AFX_LOGACTIVEEVENTERROR_H__AE46AA65_7574_484D_AA6C_275368371237__INCLUDED_)
