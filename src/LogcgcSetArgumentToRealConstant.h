// LogcgcSetArgumentToRealConstant.h: interface for the CLogcgcSetArgumentToRealConstant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOREALCONSTANT_H__1465CE00_E31E_4364_8760_A817F19338B8__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOREALCONSTANT_H__1465CE00_E31E_4364_8760_A817F19338B8__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToRealConstant : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToRealConstant(long time, const zString groupName, const zString cmdName, 
                                     long cmdNo, const zString argName, double value);

	virtual ~CLogcgcSetArgumentToRealConstant();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToRealConstant& rMsg);

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

	CLogcgcSetArgumentToRealConstant(const CLogcgcSetArgumentToRealConstant& oldMessage);
	CLogcgcSetArgumentToRealConstant& operator=(const CLogcgcSetArgumentToRealConstant& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const long     m_CommandNo;     // Index to command in the group's command container
    const double   m_Value;         // Real value of the argument

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOREALCONSTANT_H__1465CE00_E31E_4364_8760_A817F19338B8__INCLUDED_)
