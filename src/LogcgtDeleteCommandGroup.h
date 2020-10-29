// LogcgtDeleteCommandGroup.h: interface for the CLogcgtDeleteCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTDELETECOMMANDGROUP_H__C1FA6B15_4086_4133_B850_18D57C789976__INCLUDED_)
#define AFX_LOGCGTDELETECOMMANDGROUP_H__C1FA6B15_4086_4133_B850_18D57C789976__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtDeleteCommandGroup : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtDeleteCommandGroup(long time, const zString name);

	virtual ~CLogcgtDeleteCommandGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtDeleteCommandGroup& rMsg);

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

	CLogcgtDeleteCommandGroup(const CLogcgtDeleteCommandGroup& oldMessage);
	CLogcgtDeleteCommandGroup& operator=(const CLogcgtDeleteCommandGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_Name;   // Name of command group

};

#endif // !defined(AFX_LOGCGTDELETECOMMANDGROUP_H__C1FA6B15_4086_4133_B850_18D57C789976__INCLUDED_)
