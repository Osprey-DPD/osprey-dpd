// mcSetAnalysisPeriod.h: interface for the mcSetAnalysisPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETANALYSISPERIOD_H__1A386963_CC6D_45EA_B28B_23EA8CCAB39C__INCLUDED_)
#define AFX_MCSETANALYSISPERIOD_H__1A386963_CC6D_45EA_B28B_23EA8CCAB39C__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetAnalysisPeriod : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetAnalysisPeriod(long executionTime);
	mcSetAnalysisPeriod(const mcSetAnalysisPeriod& oldCommand);

	// Constructor for use when executing the command internally

	mcSetAnalysisPeriod(long executionTime, bool bLog, long period);

	virtual ~mcSetAnalysisPeriod();
	
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

	long m_Period;	// No of timesteps between time-averaged data analysis
};

#endif // !defined(AFX_MCSETANALYSISPERIOD_H__1A386963_CC6D_45EA_B28B_23EA8CCAB39C__INCLUDED_)
