// LogcgtCreateCommandGroup.h: interface for the CLogcgtCreateCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTCREATECOMMANDGROUP_H__00B06D76_EC74_43CA_9A4A_C425074AB467__INCLUDED_)
#define AFX_LOGCGTCREATECOMMANDGROUP_H__00B06D76_EC74_43CA_9A4A_C425074AB467__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtCreateCommandGroup : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtCreateCommandGroup(long time, const zString name);

	virtual ~CLogcgtCreateCommandGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtCreateCommandGroup& rMsg);

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

	CLogcgtCreateCommandGroup(const CLogcgtCreateCommandGroup& oldMessage);
	CLogcgtCreateCommandGroup& operator=(const CLogcgtCreateCommandGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_Name;   // Name of command group

};

#endif // !defined(AFX_LOGCGTCREATECOMMANDGROUP_H__00B06D76_EC74_43CA_9A4A_C425074AB467__INCLUDED_)
