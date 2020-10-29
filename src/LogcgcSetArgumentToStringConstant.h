// LogcgcSetArgumentToStringConstant.h: interface for the CLogcgcSetArgumentToStringConstant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOSTRINGCONSTANT_H__03956527_A486_42FF_95D0_BF3DE07D46EA__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOSTRINGCONSTANT_H__03956527_A486_42FF_95D0_BF3DE07D46EA__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToStringConstant : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToStringConstant(long time, const zString groupName, const zString cmdName, 
                                     long cmdNo, const zString argName, const zString value);

	virtual ~CLogcgcSetArgumentToStringConstant();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToStringConstant& rMsg);

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

	CLogcgcSetArgumentToStringConstant(const CLogcgcSetArgumentToStringConstant& oldMessage);
	CLogcgcSetArgumentToStringConstant& operator=(const CLogcgcSetArgumentToStringConstant& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const zString  m_ArgName;       // Placeholder name of argument being set
    const zString  m_Value;         // String value of the argument
    const long     m_CommandNo;     // Index to command in the group's command container

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOSTRINGCONSTANT_H__03956527_A486_42FF_95D0_BF3DE07D46EA__INCLUDED_)
