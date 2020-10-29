// mcSavePovrayCurrentState.h: interface for the mcSavePovrayCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPOVRAYCURRENTSTATE_H__C5227886_35A7_4034_8087_2B2AC7A4F701__INCLUDED_)
#define AFX_MCSAVEPOVRAYCURRENTSTATE_H__C5227886_35A7_4034_8087_2B2AC7A4F701__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSavePovrayCurrentState : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSavePovrayCurrentState(long executionTime);
	mcSavePovrayCurrentState(const mcSavePovrayCurrentState& oldCommand);

	// Constructor for use when executing the command internally

	mcSavePovrayCurrentState(long executionTime, bool bLog);
	
	virtual ~mcSavePovrayCurrentState();

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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions


	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

};

#endif // !defined(AFX_MCSAVEPOVRAYCURRENTSTATE_H__C5227886_35A7_4034_8087_2B2AC7A4F701__INCLUDED_)
