// LogCommandQueueProcessError.h: interface for the CLogCommandQueueProcessError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDQUEUEPROCESSERROR_H__EB6A1870_9201_4656_86F0_176AB8630181__INCLUDED_)
#define AFX_LOGCOMMANDQUEUEPROCESSERROR_H__EB6A1870_9201_4656_86F0_176AB8630181__INCLUDED_


#include "LogErrorMessage.h"

class CLogCommandQueueProcessError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandQueueProcessError(long time, const zString message);

	virtual ~CLogCommandQueueProcessError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandQueueProcessError& rMsg);

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

	CLogCommandQueueProcessError(const CLogCommandQueueProcessError& oldMessage);
	CLogCommandQueueProcessError& operator=(const CLogCommandQueueProcessError& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Message;	// Message describing the error condition

};

#endif // !defined(AFX_LOGCOMMANDQUEUEPROCESSERROR_H__EB6A1870_9201_4656_86F0_176AB8630181__INCLUDED_)
