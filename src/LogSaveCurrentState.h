// LogSaveCurrentState.h: interface for the CLogSaveCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVECURRENTSTATE_H__3074A08F_7A8D_4E75_81B9_ACD20F5EDB32__INCLUDED_)
#define AFX_LOGSAVECURRENTSTATE_H__3074A08F_7A8D_4E75_81B9_ACD20F5EDB32__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveCurrentState : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveCurrentState(long time);

	virtual ~CLogSaveCurrentState();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveCurrentState& rMsg);

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

	CLogSaveCurrentState(const CLogSaveCurrentState& oldMessage);
	CLogSaveCurrentState& operator=(const CLogSaveCurrentState& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSAVECURRENTSTATE_H__3074A08F_7A8D_4E75_81B9_ACD20F5EDB32__INCLUDED_)
