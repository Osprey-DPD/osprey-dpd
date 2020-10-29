// mpmCommandWrapper.h: interface for the mpmCommandWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPMCOMMANDWRAPPER_H__94BB955C_02C7_4CC7_8EDB_9F0F05571F1F__INCLUDED_)
#define AFX_MPMCOMMANDWRAPPER_H__94BB955C_02C7_4CC7_8EDB_9F0F05571F1F__INCLUDED_


// Forward declarations

class xxCommand;


#include "mpmMessage.h"

class mpmCommandWrapper : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~mpmCommandWrapper();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden when the derived class wants to call the 
    // corresponding MPI function.

   
    // ****************************************
	// Public access functions
public:

    // Function to copy the data from the xxCommand-derived object to the message:
    // this must be implemented by all derived classes to pack their
    // command-specific data into the message.

	virtual void SetMessageData(const xxCommand* const pCommand) = 0;

    // Function used by ISimState to set the execution time of the command.

 	void SetExecutionTime(long simTime);

    // Accessor functions to the message data

	xxCommand* GetCommand();

	// ****************************************
	// Protected local functions
protected:

	mpmCommandWrapper();
	
	mpmCommandWrapper(const mpmCommandWrapper& oldMessage);


	// ****************************************
	// Implementation

    long       m_SimTime;    // Simulation time when wrapped command is to be executed

    xxCommand* m_pCommand;   // Pointer to the command whose data are wrapped by the derived class

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    bool m_bDeleteCommand;  // Flag showing if the contained command should be deleted here

};
#endif // !defined(AFX_MPMCOMMANDWRAPPER_H__94BB955C_02C7_4CC7_8EDB_9F0F05571F1F__INCLUDED_)
