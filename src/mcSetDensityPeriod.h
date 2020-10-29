// mcSetDensityPeriod.h: interface for the mcSetDensityPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDENSITYPERIOD_H__AECA6671_167E_421E_B84A_549A7DD7C481__INCLUDED_)
#define AFX_MCSETDENSITYPERIOD_H__AECA6671_167E_421E_B84A_549A7DD7C481__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetDensityPeriod : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetDensityPeriod(long executionTime);
	mcSetDensityPeriod(const mcSetDensityPeriod& oldCommand);

	// Constructor for use when executing the command internally

	mcSetDensityPeriod(long executionTime, bool bLog, long period);

	virtual ~mcSetDensityPeriod();
	
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

#endif // !defined(AFX_MCSETDENSITYPERIOD_H__AECA6671_167E_421E_B84A_549A7DD7C481__INCLUDED_)
