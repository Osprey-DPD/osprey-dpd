// LogAnalysisMessage.h: interface for the CLogAnalysisMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGANALYSISMESSAGE_H__58619CBD_B097_4E10_9CDE_211DA3C9EE36__INCLUDED_)
#define AFX_LOGANALYSISMESSAGE_H__58619CBD_B097_4E10_9CDE_211DA3C9EE36__INCLUDED_


// Include the generic "command failed" message header so all info messages 
// have access to it

#include "LogCommandFailed.h"


#include "LogMessage.h"

class CLogAnalysisMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogAnalysisMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogAnalysisMessage& rMsg);

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

	CLogAnalysisMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogAnalysisMessage(const CLogAnalysisMessage& oldMessage);
	CLogAnalysisMessage& operator=(const CLogAnalysisMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGANALYSISMESSAGE_H__58619CBD_B097_4E10_9CDE_211DA3C9EE36__INCLUDED_)
