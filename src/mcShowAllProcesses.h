// mcShowAllProcesses.h: interface for the mcShowAllProcesses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSHOWALLPROCESSES_H__2641C839_2CCF_4E37_8C64_DAB54F6E29EE__INCLUDED_)
#define AFX_MCSHOWALLPROCESSES_H__2641C839_2CCF_4E37_8C64_DAB54F6E29EE__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcShowAllProcesses : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcShowAllProcesses(long executionTime);
	mcShowAllProcesses(const mcShowAllProcesses& oldCommand);

	virtual ~mcShowAllProcesses();
	
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

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;


	// ****************************************
	// Public access functions
public:

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

#endif // !defined(AFX_MCSHOWALLPROCESSES_H__2641C839_2CCF_4E37_8C64_DAB54F6E29EE__INCLUDED_)
