// mcIssueLogMessage.h: interface for the mcIssueLogMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCISSUELOGMESSAGE_H__5F19C4FA_E1AB_443C_8F34_2EDFD587DB9D__INCLUDED_)
#define AFX_MCISSUELOGMESSAGE_H__5F19C4FA_E1AB_443C_8F34_2EDFD587DB9D__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcIssueLogMessage : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcIssueLogMessage(long executionTime);

	mcIssueLogMessage(long executionTime, const zString message, long time);

	mcIssueLogMessage(const mcIssueLogMessage& oldCommand);

	virtual ~mcIssueLogMessage();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

	
private:

	static const zString m_Type;	// Identifier used in control data file for command


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;


	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;


	// ****************************************
	// Public access functions
public:

	inline const zString GetMessageName()	const {return m_Message;}
	inline long GetMessageTime()			const {return m_Time;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	zString	m_Message;		// Name of message to be issued
	long	m_Time;			// Simulation time at which to issue message
};

#endif // !defined(AFX_MCISSUELOGMESSAGE_H__5F19C4FA_E1AB_443C_8F34_2EDFD587DB9D__INCLUDED_)
