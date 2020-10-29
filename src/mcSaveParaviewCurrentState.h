// mcSaveParaviewCurrentState.h: interface for the mcSaveParaviewCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPARAVIEWCURRENTSTATE_H__C6042454_18F4_40BD_A607_B4673D803564__INCLUDED_)
#define AFX_MCSAVEPARAVIEWCURRENTSTATE_H__C6042454_18F4_40BD_A607_B4673D803564__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSaveParaviewCurrentState : public xxCommand  
{
	// ****************************************
	// Construction/Destruction
public:

	mcSaveParaviewCurrentState(long executionTime);
	mcSaveParaviewCurrentState(const mcSaveParaviewCurrentState& oldCommand);

	// Constructor for use when executing the command internally

	mcSaveParaviewCurrentState(long executionTime, bool bLog);

	virtual ~mcSaveParaviewCurrentState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;


	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

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


};

#endif // !defined(AFX_MCSAVEPARAVIEWCURRENTSTATE_H__C6042454_18F4_40BD_A607_B4673D803564__INCLUDED_)
