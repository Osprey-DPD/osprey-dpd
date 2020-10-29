// ccSelectACNCurrentPolymersAsTargetImpl.h: interface for the ccSelectACNCurrentPolymersAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNCURRENTPOLYMERSASTARGETIMPL_H__19D7629F_34FC_4B62_971E_FF239AD68846__INCLUDED_)
#define AFX_CCSELECTACNCURRENTPOLYMERSASTARGETIMPL_H__19D7629F_34FC_4B62_971E_FF239AD68846__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectACNCurrentPolymersAsTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectACNCurrentPolymersAsTargetImpl();

	virtual ~ccSelectACNCurrentPolymersAsTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectACNCurrentPolymersAsTarget(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCSELECTACNCURRENTPOLYMERSASTARGETIMPL_H__19D7629F_34FC_4B62_971E_FF239AD68846__INCLUDED_)
