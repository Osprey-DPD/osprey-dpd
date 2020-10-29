// LogcgcSetArgumentToIntegerConstant.h: interface for the CLogcgcSetArgumentToIntegerConstant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOINTEGERCONSTANT_H__6FC9C1F2_FD70_4A8E_82D4_F5DD9D937C82__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOINTEGERCONSTANT_H__6FC9C1F2_FD70_4A8E_82D4_F5DD9D937C82__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToIntegerConstant : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToIntegerConstant(long time, const zString groupName, const zString cmdName, 
                                        long cmdNo, const zString argName, long value);

	virtual ~CLogcgcSetArgumentToIntegerConstant();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToIntegerConstant& rMsg);

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

	CLogcgcSetArgumentToIntegerConstant(const CLogcgcSetArgumentToIntegerConstant& oldMessage);
	CLogcgcSetArgumentToIntegerConstant& operator=(const CLogcgcSetArgumentToIntegerConstant& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const long     m_CommandNo;     // Index to command in the group's command container
    const long     m_Value;         // Integer value of the argument

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOINTEGERCONSTANT_H__6FC9C1F2_FD70_4A8E_82D4_F5DD9D937C82__INCLUDED_)
