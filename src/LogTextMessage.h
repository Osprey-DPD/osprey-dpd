// LogTextMessage.h: interface for the CLogTextMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTEXTMESSAGE_H__5B26E6F6_85E8_4BDE_997D_6B61AF4483AF__INCLUDED_)
#define AFX_LOGTEXTMESSAGE_H__5B26E6F6_85E8_4BDE_997D_6B61AF4483AF__INCLUDED_


#include "LogInfoMessage.h"

class CLogTextMessage : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogTextMessage(long time, const zString text);

	virtual ~CLogTextMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogTextMessage& rMsg);

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

	CLogTextMessage(const CLogTextMessage& oldMessage);
	CLogTextMessage& operator=(const CLogTextMessage& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Text;	// Text string to write to log file
	
};

#endif // !defined(AFX_LOGTEXTMESSAGE_H__5B26E6F6_85E8_4BDE_997D_6B61AF4483AF__INCLUDED_)
