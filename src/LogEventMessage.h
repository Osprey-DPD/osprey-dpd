// LogEventMessage.h: interface for the CLogEventMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGEVENTMESSAGE_H__FD125824_4913_472F_B102_875631949401__INCLUDED_)
#define AFX_LOGEVENTMESSAGE_H__FD125824_4913_472F_B102_875631949401__INCLUDED_


#include "LogMessage.h"

class CLogEventMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogEventMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogEventMessage& rMsg);

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

	CLogEventMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogEventMessage(const CLogEventMessage& oldMessage);
	CLogEventMessage& operator=(const CLogEventMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGEVENTMESSAGE_H__FD125824_4913_472F_B102_875631949401__INCLUDED_)
