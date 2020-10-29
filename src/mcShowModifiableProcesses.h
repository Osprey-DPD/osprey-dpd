// mcShowModifiableProcesses.h: interface for the mcShowModifiableProcesses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSHOWMODIFIABLEPROCESSES_H__F69502ED_F911_4CB1_9D84_6A4930FE4B67__INCLUDED_)
#define AFX_MCSHOWMODIFIABLEPROCESSES_H__F69502ED_F911_4CB1_9D84_6A4930FE4B67__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcShowModifiableProcesses : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcShowModifiableProcesses(long executionTime);
	mcShowModifiableProcesses(const mcShowModifiableProcesses& oldCommand);

	virtual ~mcShowModifiableProcesses();
	
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

#endif // !defined(AFX_MCSHOWMODIFIABLEPROCESSES_H__F69502ED_F911_4CB1_9D84_6A4930FE4B67__INCLUDED_)
