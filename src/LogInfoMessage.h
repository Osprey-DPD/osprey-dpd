// LogInfoMessage.h: interface for the CLogInfoMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINFOMESSAGE_H__9C689EF7_E003_4FAF_8936_70ED0225CAFF__INCLUDED_)
#define AFX_LOGINFOMESSAGE_H__9C689EF7_E003_4FAF_8936_70ED0225CAFF__INCLUDED_


// Include the generic "command failed" message header so all info messages
// have access to it

#include "LogCommandFailed.h"


#include "LogMessage.h"

class CLogInfoMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogInfoMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogInfoMessage& rMsg);

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

	CLogInfoMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogInfoMessage(const CLogInfoMessage& oldMessage);
	CLogInfoMessage& operator=(const CLogInfoMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGINFOMESSAGE_H__9C689EF7_E003_4FAF_8936_70ED0225CAFF__INCLUDED_)
