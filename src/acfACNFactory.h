// acfACNFactory.h: interface for the acfACNFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFACNFACTORY_H__14423033_72EB_4D87_A4C0_3ABE7C92113A__INCLUDED_)
#define AFX_ACFACNFACTORY_H__14423033_72EB_4D87_A4C0_3ABE7C92113A__INCLUDED_


#include "xxBase.h"

class acfACNFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfACNFactory
	// factory object can be created. 

	static acfACNFactory* Instance();

private:

	static acfACNFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateACNCallback createFun);
	bool UnRegister(const zString id);
	aeActiveCellNetwork* Create(const zString id, IActiveSimBox* const pShadow, 
								long xn, long yn, long zn, double xw, double yw, double zw);

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

	acfACNFactory();
	~acfACNFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfACNFactory(const acfACNFactory& oldFactory);
	acfACNFactory& operator=(const acfACNFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from aeActiveCellNetwork.

	StringACNMap  m_mCallbacks;	// Map of (string, aeActiveCellNetwork) pairs

};

#endif // !defined(AFX_ACFACNFACTORY_H__14423033_72EB_4D87_A4C0_3ABE7C92113A__INCLUDED_)
