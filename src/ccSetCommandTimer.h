// ccSetCommandTimer.h: interface for the ccSetCommandTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETCOMMANDTIMER_H__01743757_0FC7_453A_859D_BA1099653128__INCLUDED_)
#define AFX_CCSETCOMMANDTIMER_H__01743757_0FC7_453A_859D_BA1099653128__INCLUDED_


#include "xxCommand.h"

class ccSetCommandTimer : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetCommandTimer(long executionTime);
	ccSetCommandTimer(const ccSetCommandTimer& oldCommand);

	virtual ~ccSetCommandTimer();
	
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
	inline long			   GetDelay()		 const {return m_Delay;}
	inline long			   GetCommandTotal() const {return m_Commands.size();}
	inline CommandSequence GetCommands()	 const {return m_Commands;}

	void SetDelay(long delay);
	void AddCommand(const xxCommand* const pCommand);

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

	long			m_Delay;		// No of time steps to delay before executing commands
	long			m_LinesToRead;	// No of commands to read from CDF
	CommandSequence	m_Commands;		// Commands to execute after delay
};

#endif // !defined(AFX_CCSETCOMMANDTIMER_H__01743757_0FC7_453A_859D_BA1099653128__INCLUDED_)
