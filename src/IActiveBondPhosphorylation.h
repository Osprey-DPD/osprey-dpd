// IActiveBondPhosphorylation.h: interface for the IActiveBondPhosphorylation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEBONDPHOSPHORYLATION_H__0237A27B_6E54_4356_B400_C00E566F7A22__INCLUDED_)
#define AFX_IACTIVEBONDPHOSPHORYLATION_H__0237A27B_6E54_4356_B400_C00E566F7A22__INCLUDED_


#include "IActiveEventSource.h"

class IActiveBondPhosphorylation : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActiveBondPhosphorylation(aeActiveSimBox* pShadow);

	virtual ~IActiveBondPhosphorylation();


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

#endif // !defined(AFX_IACTIVEBONDPHOSPHORYLATION_H__0237A27B_6E54_4356_B400_C00E566F7A22__INCLUDED_)
