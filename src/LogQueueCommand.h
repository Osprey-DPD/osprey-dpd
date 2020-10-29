// LogQueueCommand.h: interface for the CLogQueueCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGQUEUECOMMAND_H__6607C407_44B7_41C5_861E_26DDCDB0993A__INCLUDED_)
#define AFX_LOGQUEUECOMMAND_H__6607C407_44B7_41C5_861E_26DDCDB0993A__INCLUDED_


#include "LogInfoMessage.h"

class CLogQueueCommand : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogQueueCommand(long time);

	virtual ~CLogQueueCommand();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogQueueCommand& rMsg);

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

	CLogQueueCommand(const CLogQueueCommand& oldMessage);
	CLogQueueCommand& operator=(const CLogQueueCommand& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGQUEUECOMMAND_H__6607C407_44B7_41C5_861E_26DDCDB0993A__INCLUDED_)
