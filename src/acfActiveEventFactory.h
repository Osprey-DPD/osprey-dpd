// acfActiveEventFactory.h: interface for the acfActiveEventFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFACTIVEEVENTFACTORY_H__9A886AF4_E43F_4137_BD4E_CE48F208AF5E__INCLUDED_)
#define AFX_ACFACTIVEEVENTFACTORY_H__9A886AF4_E43F_4137_BD4E_CE48F208AF5E__INCLUDED_


#include "xxBase.h"

class acfActiveEventFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfActiveEventFactory
	// factory object can be created. 

	static acfActiveEventFactory* Instance();

private:

	static acfActiveEventFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateActiveEventCallback createFun);
	bool UnRegister(const zString id);
	aevActiveEvent* Create(const zString id);

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

	acfActiveEventFactory();
	~acfActiveEventFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfActiveEventFactory(const acfActiveEventFactory& oldFactory);
	acfActiveEventFactory& operator=(const acfActiveEventFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from aevActiveEvent.

	StringActiveEventMap  m_mCallbacks;	// Map of (string, aevActiveEvent) pairs

};

#endif // !defined(AFX_ACFACTIVEEVENTFACTORY_H__9A886AF4_E43F_4137_BD4E_CE48F208AF5E__INCLUDED_)
