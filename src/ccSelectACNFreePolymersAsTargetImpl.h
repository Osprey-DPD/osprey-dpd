// ccSelectACNFreePolymersAsTargetImpl.h: interface for the ccSelectACNFreePolymersAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNFREEPOLYMERSASTARGETIMPL_H__4CC16E0B_DB92_44ED_8F98_55B1655BA362__INCLUDED_)
#define AFX_CCSELECTACNFREEPOLYMERSASTARGETIMPL_H__4CC16E0B_DB92_44ED_8F98_55B1655BA362__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectACNFreePolymersAsTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectACNFreePolymersAsTargetImpl();

	virtual ~ccSelectACNFreePolymersAsTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectACNFreePolymersAsTarget(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCSELECTACNFREEPOLYMERSASTARGETIMPL_H__4CC16E0B_DB92_44ED_8F98_55B1655BA362__INCLUDED_)
