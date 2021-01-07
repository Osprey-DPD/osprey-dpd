// LogWarningTextMessage.h: interface for the CLogWarningTextMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGWARNINGTEXTMESSAGE_H__D099893F_0823_4C6C_8A12_34BEA9F9FFC9__INCLUDED_)
#define AFX_LOGWARNINGTEXTMESSAGE_H__D099893F_0823_4C6C_8A12_34BEA9F9FFC9__INCLUDED_



#include "LogInfoMessage.h"

class CLogWarningTextMessage : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogWarningTextMessage(long time, const zString text);

	virtual ~CLogWarningTextMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogWarningTextMessage& rMsg);

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

	CLogWarningTextMessage(const CLogWarningTextMessage& oldMessage);
	CLogWarningTextMessage& operator=(const CLogWarningTextMessage& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Text;	// Warning text string to write to log file
	
};

#endif // !defined(AFX_LOGWARNINGTEXTMESSAGE_H__D099893F_0823_4C6C_8A12_34BEA9F9FFC9__INCLUDED_)
