// acfTargetFactory.h: interface for the acfTargetFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACFTARGETFACTORY_H__6C4C028E_EAB0_486D_9638_26ED10EBF1A1__INCLUDED_)
#define AFX_ACFTARGETFACTORY_H__6C4C028E_EAB0_486D_9638_26ED10EBF1A1__INCLUDED_


#include "xxBase.h"

class acfTargetFactory : public xxBase
{
    // Typedef for the factory class that creates xxProtocol objects.
    // Unlike previous factory classes we explicitly include the word
    // "Callback" in the map's name to distringiush it from the
    // definition in xxBase used by composite command targets to
    // identify their contained targets.

typedef CCommandTargetNode*	(*CreateTargetCallback)(const xxBasestring label);

typedef xxBasemap<zString, CreateTargetCallback>					      StringTargetCallbackMap;
typedef xxBasemap<zString, CreateTargetCallback>::iterator	         StringTargetCallbackIterator;
typedef xxBasemap<zString, CreateTargetCallback>::const_iterator	cStringTargetCallbackIterator;

	// ****************************************
	// Construction/Destruction: private constructor/destructor declared below
public:

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function, a private static member and a 
	// protected constructor ensure that only a single acfTargetFactory
	// factory object can be created. 

	static acfTargetFactory* Instance();

private:

	static acfTargetFactory* m_pInstance;

		// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// No classes should inherit from this class: it is a singleton

	// ****************************************
	// Public access functions
public:

	bool Register(const zString id, CreateTargetCallback createFun);
	bool UnRegister(const zString id);
	CCommandTargetNode* Create(const zString id, const zString label);

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

	acfTargetFactory();
	~acfTargetFactory();

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	acfTargetFactory(const acfTargetFactory& oldFactory);
	acfTargetFactory& operator=(const acfTargetFactory& rhs);


	// ****************************************
	// Data members
private:

	// The map holds pairs consisting of a string identifier and
	// a pointer to a function that returns a pointer to an object
	// of a class derived from CCommandTargetNode.

	StringTargetCallbackMap  m_mCallbacks;	// Map of (string, CCommandTargetNode) pairs

};

#endif // !defined(AFX_ACFTARGETFACTORY_H__6C4C028E_EAB0_486D_9638_26ED10EBF1A1__INCLUDED_)
