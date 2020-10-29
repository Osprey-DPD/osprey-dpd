// tguCommands.h: interface for the tguCommands class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUCOMMANDS_H__18DE073D_AE06_47C3_B585_5C3FD4493D3C__INCLUDED_)
#define AFX_TGUCOMMANDS_H__18DE073D_AE06_47C3_B585_5C3FD4493D3C__INCLUDED_


// Foward declarations

class ISimState;


#include "xxBase.h"

class tguCommands : public xxBase
{
	// ****************************************
	// Construction/Destruction
public:

	tguCommands();

	virtual ~tguCommands();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:

    // Function to add a single command to the local store

	void AddCommand(const xxCommand* const pCommand);

    // Function to pass the payload to the ISimState for execution,
    // as the result of a currently-executing command

	void ExecuteCommands(ISimState* const pISimState);

    // Function to pass the payload to the ISimState for immediate execution 
    // as the result of a timed execution

	void ExecuteCommandsImmediately(ISimState* const pISimState);


	// ****************************************
	// Protected local functions
protected:

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:




	// ****************************************
	// Data members
private:

    CommandSequence  m_Commands;    // Set of concrete command instances

};

#endif // !defined(AFX_TGUCOMMANDS_H__18DE073D_AE06_47C3_B585_5C3FD4493D3C__INCLUDED_)
