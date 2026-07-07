// ctDynamicPolymerisePolymersInTargetImpl.h: interface for the ctDynamicPolymerisePolymersInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGETIMPL_H__47896986_72c2_47bf_b1d0_fd303e679704__INCLUDED_)
#define AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGETIMPL_H__47896986_72c2_47bf_b1d0_fd303e679704__INCLUDED_


#include "ISimBoxCmd.h"

class ctDynamicPolymerisePolymersInTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

    ctDynamicPolymerisePolymersInTargetImpl();

	virtual ~ctDynamicPolymerisePolymersInTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void DynamicPolymerisePolymersInTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTDYNAMICPOLYMERISEPOLYMERSINTARGETIMPL_H__47896986_72c2_47bf_b1d0_fd303e679704__INCLUDED_)
