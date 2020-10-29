// IActiveBondReleasesPi.h: interface for the IActiveBondReleasesPi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEBONDRELEASESPI_H__FA7560BE_08B5_4B78_AA85_59F97FC55EEC__INCLUDED_)
#define AFX_IACTIVEBONDRELEASESPI_H__FA7560BE_08B5_4B78_AA85_59F97FC55EEC__INCLUDED_


#include "IActiveEventSource.h"

class IActiveBondReleasesPi : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActiveBondReleasesPi(aeActiveSimBox* pShadow);

	virtual ~IActiveBondReleasesPi();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: 
private:

};

#endif // !defined(AFX_IACTIVEBONDRELEASESPI_H__FA7560BE_08B5_4B78_AA85_59F97FC55EEC__INCLUDED_)
