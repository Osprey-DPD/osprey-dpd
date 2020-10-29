// LogTextWarningMessage.h: interface for the CLogTextWarningMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTEXTWARNINGMESSAGE_H__A51B79A8_FD98_4074_9073_87B5C7057962__INCLUDED_)
#define AFX_LOGTEXTWARNINGMESSAGE_H__A51B79A8_FD98_4074_9073_87B5C7057962__INCLUDED_


#include "LogWarningMessage.h"

class CLogTextWarningMessage : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogTextWarningMessage(long time, const zString text);

	virtual ~CLogTextWarningMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogTextWarningMessage& rMsg);

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

	CLogTextWarningMessage(const CLogTextWarningMessage& oldMessage);
	CLogTextWarningMessage& operator=(const CLogTextWarningMessage& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Text;	// Text string to write to log file
	
};

#endif // !defined(AFX_LOGTEXTWARNINGMESSAGE_H__A51B79A8_FD98_4074_9073_87B5C7057962__INCLUDED_)
