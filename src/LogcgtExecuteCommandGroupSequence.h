// LogcgtExecuteCommandGroupSequence.h: interface for the CLogcgtExecuteCommandGroupSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTEXECUTECOMMANDGROUPSEQUENCE_H__BBF2D26D_8408_4F38_ABF2_01CB2DD3AF39__INCLUDED_)
#define AFX_LOGCGTEXECUTECOMMANDGROUPSEQUENCE_H__BBF2D26D_8408_4F38_ABF2_01CB2DD3AF39__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtExecuteCommandGroupSequence : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtExecuteCommandGroupSequence(long time, const zString groupName,
                                       long total, long period);

	virtual ~CLogcgtExecuteCommandGroupSequence();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtExecuteCommandGroupSequence& rMsg);

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

	CLogcgtExecuteCommandGroupSequence(const CLogcgtExecuteCommandGroupSequence& oldMessage);
	CLogcgtExecuteCommandGroupSequence& operator=(const CLogcgtExecuteCommandGroupSequence& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const long     m_Total;         // Number of times to execute the group
    const long     m_Period;        // Number of time steps between executions of the group

};

#endif // !defined(AFX_LOGCGTEXECUTECOMMANDGROUPSEQUENCE_H__BBF2D26D_8408_4F38_ABF2_01CB2DD3AF39__INCLUDED_)
