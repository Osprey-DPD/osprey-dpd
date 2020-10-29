// ctSerialisePolymerTailCoordinatesImpl.h: interface for the ctSerialisePolymerTailCoordinatesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSERIALISEPOLYMERTAILCOORDINATESIMPL_H__F29D747C_6F82_410D_BCD0_46F66FBF70B9__INCLUDED_)
#define AFX_CTSERIALISEPOLYMERTAILCOORDINATESIMPL_H__F29D747C_6F82_410D_BCD0_46F66FBF70B9__INCLUDED_


#include "ISimBoxCmd.h"

class ctSerialisePolymerTailCoordinatesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSerialisePolymerTailCoordinatesImpl();

	virtual ~ctSerialisePolymerTailCoordinatesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SerialisePolymerTailCoordinates(const xxCommand* const pCommand);


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

};

#endif // !defined(AFX_CTSERIALISEPOLYMERTAILCOORDINATESIMPL_H__F29D747C_6F82_410D_BCD0_46F66FBF70B9__INCLUDED_)
