// LogcgcSetArgumentToRealSequence.h: interface for the CLogcgcSetArgumentToRealSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOREALSEQUENCE_H__EAF45639_9C7E_4F1E_AE2C_EEB47B3ABA0C__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOREALSEQUENCE_H__EAF45639_9C7E_4F1E_AE2C_EEB47B3ABA0C__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToRealSequence : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToRealSequence(long time, const zString groupName, const zString cmdName, 
                                        long cmdNo, const zString argName, double initialValue, double increment);

	virtual ~CLogcgcSetArgumentToRealSequence();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToRealSequence& rMsg);

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

	CLogcgcSetArgumentToRealSequence(const CLogcgcSetArgumentToRealSequence& oldMessage);
	CLogcgcSetArgumentToRealSequence& operator=(const CLogcgcSetArgumentToRealSequence& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const long     m_CommandNo;     // Index to command in the group's command container
    const double   m_InitialValue;  // Initial value of argument
    const double   m_Increment;     // Increment

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOREALSEQUENCE_H__EAF45639_9C7E_4F1E_AE2C_EEB47B3ABA0C__INCLUDED_)
