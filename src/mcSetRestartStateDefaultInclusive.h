// mcSetRestartStateDefaultInclusive.h: interface for the mcSetRestartStateDefaultInclusive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVE_H__06854ED3_56EE_49E0_9255_7C0A831B292F__INCLUDED_)
#define AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVE_H__06854ED3_56EE_49E0_9255_7C0A831B292F__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetRestartStateDefaultInclusive : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction
public:
	mcSetRestartStateDefaultInclusive(long executionTime);
	mcSetRestartStateDefaultInclusive(const mcSetRestartStateDefaultInclusive& oldCommand);

	virtual ~mcSetRestartStateDefaultInclusive();

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

#endif // !defined(AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVE_H__06854ED3_56EE_49E0_9255_7C0A831B292F__INCLUDED_)
