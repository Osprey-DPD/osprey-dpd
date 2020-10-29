// ctSerialisePolymerHeadCoordinatesImpl.h: interface for the ctSerialisePolymerHeadCoordinatesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSERIALISEPOLYMERHEADCOORDINATESIMPL_H__8F57CF33_6E9E_46D4_AB39_B3FF8B20076F__INCLUDED_)
#define AFX_CTSERIALISEPOLYMERHEADCOORDINATESIMPL_H__8F57CF33_6E9E_46D4_AB39_B3FF8B20076F__INCLUDED_


#include "ISimBoxCmd.h"

class ctSerialisePolymerHeadCoordinatesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSerialisePolymerHeadCoordinatesImpl();

	virtual ~ctSerialisePolymerHeadCoordinatesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SerialisePolymerHeadCoordinates(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSERIALISEPOLYMERHEADCOORDINATESIMPL_H__8F57CF33_6E9E_46D4_AB39_B3FF8B20076F__INCLUDED_)
