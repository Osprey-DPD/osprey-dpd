// IActiveBondHydrolysesATP.h: interface for the IActiveBondHydrolysesATP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEBONDHYDROLYSESATP_H__2F96F94D_49D2_4FCE_B1AC_E99944996DA6__INCLUDED_)
#define AFX_IACTIVEBONDHYDROLYSESATP_H__2F96F94D_49D2_4FCE_B1AC_E99944996DA6__INCLUDED_


#include "IActiveEventSource.h"

class IActiveBondHydrolysesATP : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActiveBondHydrolysesATP(aeActiveSimBox* pShadow);

	virtual ~IActiveBondHydrolysesATP();


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

#endif // !defined(AFX_IACTIVEBONDHYDROLYSESATP_H__2F96F94D_49D2_4FCE_B1AC_E99944996DA6__INCLUDED_)
