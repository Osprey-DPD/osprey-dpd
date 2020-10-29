// LogCommandForbiddenInSerialRun.h: interface for the CLogCommandForbiddenInSerialRun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDFORBIDDENINSERIALRUN_H__91472E17_9805_41BC_B252_0FF92A8A77F9__INCLUDED_)
#define AFX_LOGCOMMANDFORBIDDENINSERIALRUN_H__91472E17_9805_41BC_B252_0FF92A8A77F9__INCLUDED_


#include "LogWarningMessage.h"

class CLogCommandForbiddenInSerialRun : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandForbiddenInSerialRun(long time, const xxCommand* const pCommand);

	virtual ~CLogCommandForbiddenInSerialRun();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandForbiddenInSerialRun& rMsg);

	// ****************************************
	// Public access functions
public:

	// Function to get access to the command object. It is needed when 
	// serialising the command parameters.

	inline const xxCommand*  GetCommand() const {return m_pCommand;}

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

	CLogCommandForbiddenInSerialRun(const CLogCommandForbiddenInSerialRun& oldMessage);
	CLogCommandForbiddenInSerialRun& operator=(const CLogCommandForbiddenInSerialRun& rhs);

	// ****************************************
	// Data members
private:

	// We store the (failed) command object to write out its parameters using 
	// its put() function.

	const xxCommand* const m_pCommand;

};

#endif // !defined(AFX_LOGCOMMANDFORBIDDENINSERIALRUN_H__91472E17_9805_41BC_B252_0FF92A8A77F9__INCLUDED_)
