// ctPolymerisePolymersInTargetImpl.h: interface for the ctPolymerisePolymersInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTPOLYMERISEPOLYMERSINTARGETIMPL_H__B1019AAA_8245_4415_8944_31ED19F100B1__INCLUDED_)
#define AFX_CTPOLYMERISEPOLYMERSINTARGETIMPL_H__B1019AAA_8245_4415_8944_31ED19F100B1__INCLUDED_


#include "ISimBoxCmd.h"

class ctPolymerisePolymersInTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctPolymerisePolymersInTargetImpl();

	virtual ~ctPolymerisePolymersInTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void PolymerisePolymersInTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTPOLYMERISEPOLYMERSINTARGETIMPL_H__B1019AAA_8245_4415_8944_31ED19F100B1__INCLUDED_)
