// IEADBinSuccessIntervals.h: interface for the IEADBinSuccessIntervals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEADBINSUCCESSINTERVALS_H__19789A6E_E1F1_4224_A71D_29940B83233C__INCLUDED_)
#define AFX_IEADBINSUCCESSINTERVALS_H__19789A6E_E1F1_4224_A71D_29940B83233C__INCLUDED_


#include "IEAD.h"

class IEADBinSuccessIntervals : public IEAD
{
	// ****************************************
	// Construction/Destruction
public:

	IEADBinSuccessIntervals(taEventSourceDecorator* const pESD);

	virtual ~IEADBinSuccessIntervals();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:

	long GetTime() const;


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

#endif // !defined(AFX_IEADBINSUCCESSINTERVALS_H__19789A6E_E1F1_4224_A71D_29940B83233C__INCLUDED_)
