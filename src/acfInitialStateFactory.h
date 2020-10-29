// acfInitialStateFactory.h: interface for the acfInitialStateFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFINITIALSTATEFACTORY_H__0CAEFA9D_6EDE_4240_AA1E_93F0CAF78606__INCLUDED_)
#define AFX_ACFINITIALSTATEFACTORY_H__0CAEFA9D_6EDE_4240_AA1E_93F0CAF78606__INCLUDED_


#include "xxBase.h"

class acfInitialStateFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfInitialStateFactory
	// factory object can be created. 

	static acfInitialStateFactory* Instance();

private:

	static acfInitialStateFactory* m_pInstance;

		// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateInitialStateCallback createFun);
	bool UnRegister(const zString id);
	CInitialStateData* Create(const zString id);

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

	acfInitialStateFactory();
	~acfInitialStateFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfInitialStateFactory(const acfInitialStateFactory& oldFactory);
	acfInitialStateFactory& operator=(const acfInitialStateFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from CAnalysis.

	StringInitialStateMap  m_mCallbacks;	// Map of (string, CInitialStateData) pairs

};

#endif // !defined(AFX_ACFINITIALSTATEFACTORY_H__0CAEFA9D_6EDE_4240_AA1E_93F0CAF78606__INCLUDED_)
