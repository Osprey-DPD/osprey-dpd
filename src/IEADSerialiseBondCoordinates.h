// IEADSerialiseBondCoordinates.h: interface for the IEADSerialiseBondCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEADSERIALISEBONDCOORDINATES_H__897FA241_6C3C_4981_9FA3_B56364719C63__INCLUDED_)
#define AFX_IEADSERIALISEBONDCOORDINATES_H__897FA241_6C3C_4981_9FA3_B56364719C63__INCLUDED_


#include "IEAD.h"

class IEADSerialiseBondCoordinates : public IEAD
{
	// ****************************************
	// Construction/Destruction
public:

	IEADSerialiseBondCoordinates(taEventSourceDecorator* const pESD);

	virtual ~IEADSerialiseBondCoordinates();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:

	double GetXPos() const;
	double GetYPos() const;
	double GetZPos() const;


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

protected:

private:


};

#endif // !defined(AFX_IEADSERIALISEBONDCOORDINATES_H__897FA241_6C3C_4981_9FA3_B56364719C63__INCLUDED_)
