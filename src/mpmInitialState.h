// mpmInitialState.h: interface for the mpmInitialState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPMINITIALSTATE_H__C4FCB7A8_0E09_490B_89A3_A9B68700B489__INCLUDED_)
#define AFX_MPMINITIALSTATE_H__C4FCB7A8_0E09_490B_89A3_A9B68700B489__INCLUDED_


// Forward declarations

class CInitialStateData;

// Include files that contain flags to control conditional compilation
// of the derived classes, and XML-related flags included here to avoid
// every derived class having to explicitly include it.



#include "mpmMessage.h"

class mpmInitialState : public mpmMessage  
{
public:

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~mpmInitialState();

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

	virtual void SetMessageData(const CInitialStateData* const pISD) = 0;

    // Function to return the IS string identifier

    const zString GetISType() const {return m_ISType;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	mpmInitialState(const zString type);
	
	mpmInitialState(const mpmInitialState& oldMessage);

	// ****************************************
	// Implementation


protected:

	void SetISType(zString type);


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

     zString m_ISType;  // String identifier for the initial state

};

#endif // !defined(AFX_MPMINITIALSTATE_H__C4FCB7A8_0E09_490B_89A3_A9B68700B489__INCLUDED_)
