// mcToggleRestartWarningMessages.h: interface for the mcToggleRestartWarningMessages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLERESTARTWARNINGMESSAGES_H__27DFB9C7_6ADF_4BED_A4C9_A8FAA8191BAB__INCLUDED_)
#define AFX_MCTOGGLERESTARTWARNINGMESSAGES_H__27DFB9C7_6ADF_4BED_A4C9_A8FAA8191BAB__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcToggleRestartWarningMessages : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:
	mcToggleRestartWarningMessages(long executionTime);
	mcToggleRestartWarningMessages(const mcToggleRestartWarningMessages& oldCommand);

	virtual ~mcToggleRestartWarningMessages();

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

#endif // !defined(AFX_MCTOGGLERESTARTWARNINGMESSAGES_H__27DFB9C7_6ADF_4BED_A4C9_A8FAA8191BAB__INCLUDED_)
