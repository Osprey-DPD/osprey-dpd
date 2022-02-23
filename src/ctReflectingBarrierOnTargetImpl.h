// ctReflectingBarrierOnTargetImpl.h: interface for the ctReflectingBarrierOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTREFLECTINGBARRIERONTARGETIMPL_H__35c5d90c_b804_4616_b923_f4d9ef85136b__INCLUDED_)
#define AFX_CTREFLECTINGBARRIERONTARGETIMPL_H__35c5d90c_b804_4616_b923_f4d9ef85136b__INCLUDED_


#include "ISimBoxCmd.h"

class ctReflectingBarrierOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctReflectingBarrierOnTargetImpl();

	virtual ~ctReflectingBarrierOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ReflectingBarrierOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTREFLECTINGBARRIERONTARGETIMPL_H__35c5d90c_b804_4616_b923_f4d9ef85136b__INCLUDED_)
