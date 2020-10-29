// acfEventFactory.h: interface for the acfEventFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFEVENTFACTORY_H__3EDDCFC5_520D_4C8A_8573_C6C8E42ABD62__INCLUDED_)
#define AFX_ACFEVENTFACTORY_H__3EDDCFC5_520D_4C8A_8573_C6C8E42ABD62__INCLUDED_


#include "xxBase.h"

class acfEventFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfEventFactory
	// factory object can be created. 

	static acfEventFactory* Instance();

private:

	static acfEventFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateEventCallback createFun);
	bool UnRegister(const zString id);
	xxEvent* Create(const zString id);

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Private constructor and destructor prevent instantiation of the 
	// factory class even by derived classes, and accidental deletion of the
	// factory given a pointer to it.

	acfEventFactory();
	~acfEventFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfEventFactory(const acfEventFactory& oldFactory);
	acfEventFactory& operator=(const acfEventFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from xxEvent.

	StringEventMap  m_mCallbacks;	// Map of (string, xxEvent) pairs

};

#endif // !defined(AFX_ACFEVENTFACTORY_H__3EDDCFC5_520D_4C8A_8573_C6C8E42ABD62__INCLUDED_)
