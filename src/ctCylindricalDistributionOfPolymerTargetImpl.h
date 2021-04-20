// ctCylindricalDistributionOfPolymerTargetImpl.h: interface for the ctCylindricalDistributionOfPolymerTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGETIMPL_H__69caf0a9_93c6_4788_8e36_f41d76175c0f__INCLUDED_)
#define AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGETIMPL_H__69caf0a9_93c6_4788_8e36_f41d76175c0f__INCLUDED_


#include "ISimBoxCmd.h"

class ctCylindricalDistributionOfPolymerTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctCylindricalDistributionOfPolymerTargetImpl();

	virtual ~ctCylindricalDistributionOfPolymerTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:



	// ****************************************
	// Public access functions
public:

	void CylindricalDistributionOfPolymerTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCYLINDRICALDISTRIBUTIONOFPOLYMERTARGETIMPL_H__69caf0a9_93c6_4788_8e36_f41d76175c0f__INCLUDED_)
