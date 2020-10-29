// LogSaveRestartState.h: interface for the CLogSaveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVERESTARTSTATE_H__53C58C10_42C8_4EBB_87E3_D2DF79E7F5F9__INCLUDED_)
#define AFX_LOGSAVERESTARTSTATE_H__53C58C10_42C8_4EBB_87E3_D2DF79E7F5F9__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveRestartState : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveRestartState(long time);

	virtual ~CLogSaveRestartState();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveRestartState& rMsg);

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

	CLogSaveRestartState(const CLogSaveRestartState& oldMessage);
	CLogSaveRestartState& operator=(const CLogSaveRestartState& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSAVERESTARTSTATE_H__53C58C10_42C8_4EBB_87E3_D2DF79E7F5F9__INCLUDED_)
