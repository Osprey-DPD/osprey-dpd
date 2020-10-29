// ccSetTimeStepSize.h: interface for the ccSetTimeStepSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSETTIMESTEPSIZE_H__65120C02_3CDB_4E07_A23D_42B5A02FF1DD__INCLUDED_)
#define AFX_CCSETTIMESTEPSIZE_H__65120C02_3CDB_4E07_A23D_42B5A02FF1DD__INCLUDED_


#include "xxCommand.h"

class ccSetTimeStepSize : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSetTimeStepSize(long executionTime);
	ccSetTimeStepSize(const ccSetTimeStepSize& oldCommand);

	ccSetTimeStepSize(long executionTime, bool bLog, double newStep);

	virtual ~ccSetTimeStepSize();
	
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

	inline double GetTimeStep() const {return m_dt;}

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

	double  m_dt;					// New value of integration time step
};

#endif // !defined(AFX_CCSETTIMESTEPSIZE_H__65120C02_3CDB_4E07_A23D_42B5A02FF1DD__INCLUDED_)
