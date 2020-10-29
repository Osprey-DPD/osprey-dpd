// mcSetDisplayPeriod.h: interface for the mcSetDisplayPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDISPLAYPERIOD_H__7B07A134_785E_484D_ADF6_20D541661CAC__INCLUDED_)
#define AFX_MCSETDISPLAYPERIOD_H__7B07A134_785E_484D_ADF6_20D541661CAC__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetDisplayPeriod : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetDisplayPeriod(long executionTime);
	mcSetDisplayPeriod(const mcSetDisplayPeriod& oldCommand);

	// Constructor for use when executing the command internally

	mcSetDisplayPeriod(long executionTime, bool bLog, long period);

	virtual ~mcSetDisplayPeriod();
	
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

	inline long GetPeriod()	const {return m_Period;}

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

	long m_Period;	// No of timesteps between saving current states
};

#endif // !defined(AFX_MCSETDISPLAYPERIOD_H__7B07A134_785E_484D_ADF6_20D541661CAC__INCLUDED_)


