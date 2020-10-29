// ctRgOfPolymerTargetImpl.h: interface for the ctRgOfPolymerTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRGOFPOLYMERTARGETIMPL_H__1ef039d9_8bb4_4401_9ba1_675787ba9350__INCLUDED_)
#define      AFX_CTRGOFPOLYMERTARGETIMPL_H__1ef039d9_8bb4_4401_9ba1_675787ba9350__INCLUDED_


#include "ISimBoxCmd.h"

class ctRgOfPolymerTargetImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctRgOfPolymerTargetImpl();

	virtual ~ctRgOfPolymerTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RgPolymerTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTRGOFPOLYMERTARGETIMPL_H__1ef039d9_8bb4_4401_9ba1_675787ba9350__INCLUDED_)
