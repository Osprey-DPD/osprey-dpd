// ccSelectACNCurrentBondsAsTargetImpl.h: interface for the ccSelectACNCurrentBondsAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNCURRENTBONDSASTARGETIMPL_H__C7F33801_B87C_427E_9C08_397ECF609B87__INCLUDED_)
#define AFX_CCSELECTACNCURRENTBONDSASTARGETIMPL_H__C7F33801_B87C_427E_9C08_397ECF609B87__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectACNCurrentBondsAsTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectACNCurrentBondsAsTargetImpl();

	virtual ~ccSelectACNCurrentBondsAsTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectACNCurrentBondsAsTarget(const xxCommand* const pCommand);

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_CCSELECTACNCURRENTBONDSASTARGETIMPL_H__C7F33801_B87C_427E_9C08_397ECF609B87__INCLUDED_)
