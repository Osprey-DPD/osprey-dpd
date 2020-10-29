// LogCommandForbiddenInParallelRun.h: interface for the CLogCommandForbiddenInParallelRun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDFORBIDDENINPARALLELRUN_H__71D5B5E2_A480_4E72_94E9_34CE54E5A91A__INCLUDED_)
#define AFX_LOGCOMMANDFORBIDDENINPARALLELRUN_H__71D5B5E2_A480_4E72_94E9_34CE54E5A91A__INCLUDED_


#include "LogWarningMessage.h"

class CLogCommandForbiddenInParallelRun : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandForbiddenInParallelRun(long time, const xxCommand* const pCommand);

	virtual ~CLogCommandForbiddenInParallelRun();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandForbiddenInParallelRun& rMsg);

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

	CLogCommandForbiddenInParallelRun(const CLogCommandForbiddenInParallelRun& oldMessage);
	CLogCommandForbiddenInParallelRun& operator=(const CLogCommandForbiddenInParallelRun& rhs);

	// ****************************************
	// Data members
private:

	// We store the (failed) command object to write out its parameters using 
	// its put() function.

	const xxCommand* const m_pCommand;

};

#endif // !defined(AFX_LOGCOMMANDFORBIDDENINPARALLELRUN_H__71D5B5E2_A480_4E72_94E9_34CE54E5A91A__INCLUDED_)
