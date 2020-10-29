// LogcgcSetArgumentToIntegerSequence.h: interface for the CLogcgcSetArgumentToIntegerSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOINTEGERSEQUENCE_H__9F2BBABC_82A0_4F07_9724_4BDDC7BEEB00__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOINTEGERSEQUENCE_H__9F2BBABC_82A0_4F07_9724_4BDDC7BEEB00__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToIntegerSequence : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToIntegerSequence(long time, const zString groupName, const zString cmdName, 
                                        long cmdNo, const zString argName, long initialValue, long increment);

	virtual ~CLogcgcSetArgumentToIntegerSequence();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToIntegerSequence& rMsg);

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

	CLogcgcSetArgumentToIntegerSequence(const CLogcgcSetArgumentToIntegerSequence& oldMessage);
	CLogcgcSetArgumentToIntegerSequence& operator=(const CLogcgcSetArgumentToIntegerSequence& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const long     m_CommandNo;     // Index to command in the group's command container
    const long     m_InitialValue;  // Initial value of argument
    const long     m_Increment;     // Increment

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOINTEGERSEQUENCE_H__9F2BBABC_82A0_4F07_9724_4BDDC7BEEB00__INCLUDED_)
