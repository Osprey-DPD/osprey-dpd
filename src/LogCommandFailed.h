// LogCommandFailed.h: interface for the CLogCommandFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDFAILED_H__5DD8C817_CBF8_47DC_9231_ABE0DC3DB0DA__INCLUDED_)
#define AFX_LOGCOMMANDFAILED_H__5DD8C817_CBF8_47DC_9231_ABE0DC3DB0DA__INCLUDED_


#include "LogWarningMessage.h"

class CLogCommandFailed : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandFailed(long time, const xxCommand* const pCommand);

	virtual ~CLogCommandFailed();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandFailed& rMsg);

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

	CLogCommandFailed(const CLogCommandFailed& oldMessage);
	CLogCommandFailed& operator=(const CLogCommandFailed& rhs);

	// ****************************************
	// Data members
private:

	// We store the (failed) command object to write out its parameters using 
	// its put() function.

	const xxCommand* const m_pCommand;

};

#endif // !defined(AFX_LOGCOMMANDFAILED_H__5DD8C817_CBF8_47DC_9231_ABE0DC3DB0DA__INCLUDED_)
