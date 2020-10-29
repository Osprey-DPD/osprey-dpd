// ctPolymerisePolymerTripleInTargetImpl.h: interface for the ctPolymerisePolymerTripleInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGETIMPL_H__3177249C_71B3_48D5_B8B2_6A087C31BC96__INCLUDED_)
#define AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGETIMPL_H__3177249C_71B3_48D5_B8B2_6A087C31BC96__INCLUDED_


#include "ISimBoxCmd.h"

class ctPolymerisePolymerTripleInTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctPolymerisePolymerTripleInTargetImpl();

	virtual ~ctPolymerisePolymerTripleInTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void PolymerisePolymerTripleInTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGETIMPL_H__3177249C_71B3_48D5_B8B2_6A087C31BC96__INCLUDED_)
