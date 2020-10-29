// LogRestartStateBuilderError.h: interface for the CLogRestartStateBuilderError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRESTARTSTATEBUILDERERROR_H__586D2A36_7DC8_4596_9F6A_42D5FA43C24E__INCLUDED_)
#define AFX_LOGRESTARTSTATEBUILDERERROR_H__586D2A36_7DC8_4596_9F6A_42D5FA43C24E__INCLUDED_


#include "LogErrorMessage.h"

class CLogRestartStateBuilderError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogRestartStateBuilderError(long time, const zString message);

	virtual ~CLogRestartStateBuilderError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogRestartStateBuilderError& rMsg);

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

	CLogRestartStateBuilderError(const CLogRestartStateBuilderError& oldMessage);
	CLogRestartStateBuilderError& operator=(const CLogRestartStateBuilderError& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Message;	// Message describing the error condition

};

#endif // !defined(AFX_LOGRESTARTSTATEBUILDERERROR_H__586D2A36_7DC8_4596_9F6A_42D5FA43C24E__INCLUDED_)
