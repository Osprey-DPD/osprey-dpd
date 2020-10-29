// ccSelectACNAsTargetImpl.h: interface for the ccSelectACNAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNASTARGETIMPL_H__54C47FA0_F981_4A7F_B315_CA2CF67DFE46__INCLUDED_)
#define AFX_CCSELECTACNASTARGETIMPL_H__54C47FA0_F981_4A7F_B315_CA2CF67DFE46__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectACNAsTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectACNAsTargetImpl();

	virtual ~ccSelectACNAsTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectACNAsTarget(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCSELECTACNASTARGETIMPL_H__54C47FA0_F981_4A7F_B315_CA2CF67DFE46__INCLUDED_)
