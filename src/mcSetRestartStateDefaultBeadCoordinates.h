// mcSetRestartStateDefaultBeadCoordinates.h: interface for the mcSetRestartStateDefaultBeadCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATES_H__341F2C05_A88E_4F62_924B_D90053BB26C5__INCLUDED_)
#define AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATES_H__341F2C05_A88E_4F62_924B_D90053BB26C5__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetRestartStateDefaultBeadCoordinates : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction
public:
	mcSetRestartStateDefaultBeadCoordinates(long executionTime);
	mcSetRestartStateDefaultBeadCoordinates(const mcSetRestartStateDefaultBeadCoordinates& oldCommand);

	virtual ~mcSetRestartStateDefaultBeadCoordinates();

	// ****************************************
	// Global functions, static member functions and variables


	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	static const zString m_Type;	// Identifier used in control data file for command
};

#endif // !defined(AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATES_H__341F2C05_A88E_4F62_924B_D90053BB26C5__INCLUDED_)
