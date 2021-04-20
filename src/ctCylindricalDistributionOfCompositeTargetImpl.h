// ctCylindricalDistributionOfCompositeTargetImpl.h: interface for the ctCylindricalDistributionOfCompositeTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFCOMPOSITETARGETIMPL_H__f020edc0_4f5a_435b_8aa6_a5146300110a__INCLUDED_)
#define AFX_CTCYLINDRICALDISTRIBUTIONOFCOMPOSITETARGETIMPL_H__f020edc0_4f5a_435b_8aa6_a5146300110a__INCLUDED_


#include "ISimBoxCmd.h"

class ctCylindricalDistributionOfCompositeTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctCylindricalDistributionOfCompositeTargetImpl();

	virtual ~ctCylindricalDistributionOfCompositeTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:



	// ****************************************
	// Public access functions
public:

	void CylindricalDistributionOfCompositeTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFCOMPOSITETARGETIMPL_H__f020edc0_4f5a_435b_8aa6_a5146300110a__INCLUDED_)
