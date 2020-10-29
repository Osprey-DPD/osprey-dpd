// ctSpringForceBetweenTargetsImpl.h: interface for the ctSpringForceBetweenTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSPRINGFORCEBETWEENTARGETSIMPL_H__c6bb7994_71fe_453e_b72b_a9aed47ec366__INCLUDED_)
#define AFX_CTSPRINGFORCEBETWEENTARGETSIMPL_H__c6bb7994_71fe_453e_b72b_a9aed47ec366__INCLUDED_


#include "ISimBoxCmd.h"

class ctSpringForceBetweenTargetsImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctSpringForceBetweenTargetsImpl();

	virtual ~ctSpringForceBetweenTargetsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SpringForceBetweenTargets(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSPRINGFORCEBETWEENTARGETSIMPL_H__c6bb7994_71fe_453e_b72b_a9aed47ec366__INCLUDED_)
