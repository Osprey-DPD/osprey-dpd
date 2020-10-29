// acfProtocolFactory.h: interface for the acfProtocolFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFPROTOCOLFACTORY_H__C8197B91_635D_4268_B085_46244D0B695C__INCLUDED_)
#define AFX_ACFPROTOCOLFACTORY_H__C8197B91_635D_4268_B085_46244D0B695C__INCLUDED_


// Forward declarations

class xxProtocol;


#include "xxBase.h"

class acfProtocolFactory : public xxBase
{
// Typedef for the factory class that creates xxProtocol objects

typedef xxProtocol*	(*CreateProtocolCallback)(const xxBasestring runId);

typedef xxBasemap<zString, CreateProtocolCallback>					StringProtocolMap;
typedef xxBasemap<zString, CreateProtocolCallback>::iterator	    StringProtocolIterator;
typedef xxBasemap<zString, CreateProtocolCallback>::const_iterator	cStringProtocolIterator;

	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfProtocolFactory
	// factory object can be created. 

	static acfProtocolFactory* Instance();

private:

	static acfProtocolFactory* m_pInstance;

		// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateProtocolCallback createFun);
	bool UnRegister(const zString id);
	xxProtocol* Create(const zString id, const zString runId);

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

	acfProtocolFactory();
	~acfProtocolFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfProtocolFactory(const acfProtocolFactory& oldFactory);
	acfProtocolFactory& operator=(const acfProtocolFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from xxProtocol.

	StringProtocolMap  m_mCallbacks;	// Map of (string, xxProtocol) pairs

};

#endif // !defined(AFX_ACFPROTOCOLFACTORY_H__C8197B91_635D_4268_B085_46244D0B695C__INCLUDED_)
