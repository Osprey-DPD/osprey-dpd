// LogTextErrorMessage.h: interface for the CLogTextErrorMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTEXTERRORMESSAGE_H__F6AEA243_42F1_4255_84C8_95956F4C4561__INCLUDED_)
#define AFX_LOGTEXTERRORMESSAGE_H__F6AEA243_42F1_4255_84C8_95956F4C4561__INCLUDED_


#include "LogErrorMessage.h"

class CLogTextErrorMessage : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogTextErrorMessage(long time, const zString text);

	virtual ~CLogTextErrorMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogTextErrorMessage& rMsg);

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

	CLogTextErrorMessage(const CLogTextErrorMessage& oldMessage);
	CLogTextErrorMessage& operator=(const CLogTextErrorMessage& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Text;	// Text string to write to log file
	
};

#endif // !defined(AFX_LOGTEXTERRORMESSAGE_H__F6AEA243_42F1_4255_84C8_95956F4C4561__INCLUDED_)
