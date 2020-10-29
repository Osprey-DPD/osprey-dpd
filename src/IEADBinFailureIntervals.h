// IEADBinFailureIntervals.h: interface for the IEADBinFailureIntervals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEADBINFAILUREINTERVALS_H__9119E95D_F858_4784_8D0A_2EE83FCF5E9F__INCLUDED_)
#define AFX_IEADBINFAILUREINTERVALS_H__9119E95D_F858_4784_8D0A_2EE83FCF5E9F__INCLUDED_


#include "IEAD.h"

class IEADBinFailureIntervals : public IEAD
{
	// ****************************************
	// Construction/Destruction
public:

	IEADBinFailureIntervals(taEventSourceDecorator* const pESD);

	virtual ~IEADBinFailureIntervals();


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

#endif // !defined(AFX_IEADBINFAILUREINTERVALS_H__9119E95D_F858_4784_8D0A_2EE83FCF5E9F__INCLUDED_)
