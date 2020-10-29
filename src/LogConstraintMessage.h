// LogConstraintMessage.h: interface for the CLogConstraintMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCONSTRAINTMESSAGE_H__BB83B32F_FB52_41D0_9578_9F8BF50F5C78__INCLUDED_)
#define AFX_LOGCONSTRAINTMESSAGE_H__BB83B32F_FB52_41D0_9578_9F8BF50F5C78__INCLUDED_


// Include the generic "command failed" message header so all constraint
// messages have access to it

#include "LogCommandFailed.h"


#include "LogMessage.h"

class CLogConstraintMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogConstraintMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogConstraintMessage& rMsg);

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

	CLogConstraintMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogConstraintMessage(const CLogConstraintMessage& oldMessage);
	CLogConstraintMessage& operator=(const CLogConstraintMessage& rhs);


	// ****************************************
	// Data members
private:


};


#endif // !defined(AFX_LOGCONSTRAINTMESSAGE_H__BB83B32F_FB52_41D0_9578_9F8BF50F5C78__INCLUDED_)
