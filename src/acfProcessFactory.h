// acfProcessFactory.h: interface for the acfProcessFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFPROCESSFACTORY_H__97ED8D18_DF30_429B_B3E6_23C6D199E523__INCLUDED_)
#define AFX_ACFPROCESSFACTORY_H__97ED8D18_DF30_429B_B3E6_23C6D199E523__INCLUDED_


#include "xxBase.h"

class acfProcessFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfProcessFactory
	// factory object can be created. 

	static acfProcessFactory* Instance();

private:

	static acfProcessFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateProcessCallback createFun);
	bool UnRegister(const zString id);
	xxProcess* Create(const zString id);

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

	acfProcessFactory();
	~acfProcessFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfProcessFactory(const acfProcessFactory& oldFactory);
	acfProcessFactory& operator=(const acfProcessFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from CAnalysis.

	StringProcessMap  m_mCallbacks;	// Map of (string, xxProcess) pairs

};

#endif // !defined(AFX_ACFPROCESSFACTORY_H__97ED8D18_DF30_429B_B3E6_23C6D199E523__INCLUDED_)
