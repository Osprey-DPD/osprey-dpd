// LogcgcSetArgumentToStringSequence.h: interface for the CLogcgcSetArgumentToStringSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOSTRINGSEQUENCE_H__EECBC919_93FC_4A0E_A59F_9FEDA722B3AA__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOSTRINGSEQUENCE_H__EECBC919_93FC_4A0E_A59F_9FEDA722B3AA__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToStringSequence : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToStringSequence(long time,  const zString groupName, const zString cmdName, 
                                       long cmdNo, const zString argName,   const zString root);

	virtual ~CLogcgcSetArgumentToStringSequence();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToStringSequence& rMsg);

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

	CLogcgcSetArgumentToStringSequence(const CLogcgcSetArgumentToStringSequence& oldMessage);
	CLogcgcSetArgumentToStringSequence& operator=(const CLogcgcSetArgumentToStringSequence& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const zString  m_Root;          // Prefix of argument to which counter (1,2,3,...) is appended

    const long     m_CommandNo;     // Index to command in the group's command container

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOSTRINGSEQUENCE_H__EECBC919_93FC_4A0E_A59F_9FEDA722B3AA__INCLUDED_)
