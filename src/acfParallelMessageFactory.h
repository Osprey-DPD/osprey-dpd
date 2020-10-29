// acfParallelMessageFactory.h: interface for the acfParallelMessageFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFPARALLELMESSAGEFACTORY_H__5CD6C5A8_A507_46F0_A47C_310392E93670__INCLUDED_)
#define AFX_ACFPARALLELMESSAGEFACTORY_H__5CD6C5A8_A507_46F0_A47C_310392E93670__INCLUDED_


#include "xxBase.h"

class acfParallelMessageFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: protected constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfParallelMessageFactory
	// factory object can be created. 

	static acfParallelMessageFactory* Instance();

private:

	static acfParallelMessageFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateParallelMessageCallback createFun);
	bool UnRegister(const zString id);
	mpmMessage* Create(const zString id);

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

	acfParallelMessageFactory();
	~acfParallelMessageFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfParallelMessageFactory(const acfParallelMessageFactory& oldFactory);
	acfParallelMessageFactory& operator=(const acfParallelMessageFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from xxCommand.

	StringParallelMessageMap  m_mCallbacks;	// Map of (string, mpmMessage) pairs

};

#endif // !defined(AFX_ACFPARALLELMESSAGEFACTORY_H__5CD6C5A8_A507_46F0_A47C_310392E93670__INCLUDED_)
