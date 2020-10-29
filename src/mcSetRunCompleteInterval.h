// mcSetRunCompleteInterval.h: interface for the mcSetRunCompleteInterval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRUNCOMPLETEINTERVAL_H__197C17C1_76DD_461D_9238_5A11BE8AE5D3__INCLUDED_)
#define AFX_MCSETRUNCOMPLETEINTERVAL_H__197C17C1_76DD_461D_9238_5A11BE8AE5D3__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetRunCompleteInterval : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetRunCompleteInterval(long executionTime);

	mcSetRunCompleteInterval(long executionTime, long interval);

	mcSetRunCompleteInterval(const mcSetRunCompleteInterval& oldCommand);

	virtual ~mcSetRunCompleteInterval();
	
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

	inline long GetInterval()	const {return m_Interval;}

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

	long m_Interval;	// Percentage of total run time between log messages
};

#endif // !defined(AFX_MCSETRUNCOMPLETEINTERVAL_H__197C17C1_76DD_461D_9238_5A11BE8AE5D3__INCLUDED_)
