// LogcgtDeleteExecuteCommandGroupSequence.h: interface for the CLogcgtDeleteExecuteCommandGroupSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTDELETEEXECUTECOMMANDGROUPSEQUENCE_H__16EE02C9_CDCC_46B2_A94B_7C80E9A0A737__INCLUDED_)
#define AFX_LOGCGTDELETEEXECUTECOMMANDGROUPSEQUENCE_H__16EE02C9_CDCC_46B2_A94B_7C80E9A0A737__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtDeleteExecuteCommandGroupSequence : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtDeleteExecuteCommandGroupSequence(long time, const zString groupName);

	virtual ~CLogcgtDeleteExecuteCommandGroupSequence();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtDeleteExecuteCommandGroupSequence& rMsg);

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

	CLogcgtDeleteExecuteCommandGroupSequence(const CLogcgtDeleteExecuteCommandGroupSequence& oldMessage);
	CLogcgtDeleteExecuteCommandGroupSequence& operator=(const CLogcgtDeleteExecuteCommandGroupSequence& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group

};

#endif // !defined(AFX_LOGCGTDELETEEXECUTECOMMANDGROUPSEQUENCE_H__16EE02C9_CDCC_46B2_A94B_7C80E9A0A737__INCLUDED_)
