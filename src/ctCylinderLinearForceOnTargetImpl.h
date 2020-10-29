// ctCylinderLinearForceOnTargetImpl.h: interface for the ctCylinderLinearForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCYLINDERLINEARFORCEONTARGETIMPL_H__8DF8A53C_138B_4DDE_9607_124594D2181F__INCLUDED_)
#define AFX_CTCYLINDERLINEARFORCEONTARGETIMPL_H__8DF8A53C_138B_4DDE_9607_124594D2181F__INCLUDED_


#include "ISimBoxCmd.h"

class ctCylinderLinearForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctCylinderLinearForceOnTargetImpl();

	virtual ~ctCylinderLinearForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CylinderLinearForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCYLINDERLINEARFORCEONTARGETIMPL_H__8DF8A53C_138B_4DDE_9607_124594D2181F__INCLUDED_)
