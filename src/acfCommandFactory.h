// acfCommandFactory.h: interface for the acfCommandFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFCOMMANDFACTORY_H__B2427D1F_D4BF_4979_B964_EA7D9479EFAD__INCLUDED_)
#define AFX_ACFCOMMANDFACTORY_H__B2427D1F_D4BF_4979_B964_EA7D9479EFAD__INCLUDED_


#include "xxBase.h"

class acfCommandFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: protected constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfCommandFactory
	// factory object can be created. 

	static acfCommandFactory* Instance();

private:

	static acfCommandFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateCommandCallback createFun);
	bool Register(const zString id, long argTotal, CreateCommandCallback createFun);
	bool UnRegister(const zString id);
	xxCommand* Create(const zString id, long executionTime);
	long GetArgumentTotal(const zString id);

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

	acfCommandFactory();
	~acfCommandFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfCommandFactory(const acfCommandFactory& oldFactory);
	acfCommandFactory& operator=(const acfCommandFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from xxCommand.

	StringCommandMap  m_mCallbacks;	// Map of (zString,xxCommand) pairs

    // This map holds the number of arguments required by an instance of
    // a command class. It is optional whether any command class uses it.

    StringLongMap     m_mArguments; // Map of (zString,long) pairs

};

#endif // !defined(AFX_ACFCOMMANDFACTORY_H__B2427D1F_D4BF_4979_B964_EA7D9479EFAD__INCLUDED_)
