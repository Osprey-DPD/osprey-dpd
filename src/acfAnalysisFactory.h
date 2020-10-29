// acfAnalysisFactory.h: interface for the acfAnalysisFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFANALYSISFACTORY_H__BBFA3109_313A_4E80_A152_DBF70CACF5B6__INCLUDED_)
#define AFX_ACFANALYSISFACTORY_H__BBFA3109_313A_4E80_A152_DBF70CACF5B6__INCLUDED_


#include "xxBase.h"

class acfAnalysisFactory : public xxBase
{
	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfAnalysisFactory
	// factory object can be created. 

	static acfAnalysisFactory* Instance();

private:

	static acfAnalysisFactory* m_pInstance;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateAnalysisCallback createFun);
	bool UnRegister(const zString id);
	CAnalysisData* Create(const zString id);

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

	acfAnalysisFactory();
	~acfAnalysisFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfAnalysisFactory(const acfAnalysisFactory& oldFactory);
	acfAnalysisFactory& operator=(const acfAnalysisFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from CAnalysis.

	StringAnalysisMap  m_mCallbacks;	// Map of (string, CAnalysis) pairs

};

#endif // !defined(AFX_ACFANALYSISFACTORY_H__BBFA3109_313A_4E80_A152_DBF70CACF5B6__INCLUDED_)
