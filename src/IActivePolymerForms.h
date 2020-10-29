// IActivePolymerForms.h: interface for the IActivePolymerForms class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IACTIVEPOLYMERFORMS_H__D42E6363_6388_434D_8FD9_589F1DE8B837__INCLUDED_)
#define AFX_IACTIVEPOLYMERFORMS_H__D42E6363_6388_434D_8FD9_589F1DE8B837__INCLUDED_


#include "IActiveEventSource.h"

class IActivePolymerForms : public IActiveEventSource
{
	// ****************************************
	// Construction/Destruction: interface class that wraps the aeActiveSimBox instance
public:

	IActivePolymerForms(aeActiveSimBox* pShadow);

	virtual ~IActivePolymerForms();


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

#endif // !defined(AFX_IACTIVEPOLYMERFORMS_H__D42E6363_6388_434D_8FD9_589F1DE8B837__INCLUDED_)
