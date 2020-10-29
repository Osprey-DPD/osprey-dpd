// ctMSDOfPolymerTargetImpl.h: interface for the ctMSDOfPolymerTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTMSDOFPOLYMERTARGETIMPL_H__9BDE3E1D_6BD3_47D1_BF49_A530FE2BD5FF__INCLUDED_)
#define AFX_CTMSDOFPOLYMERTARGETIMPL_H__9BDE3E1D_6BD3_47D1_BF49_A530FE2BD5FF__INCLUDED_


#include "ISimBoxCmd.h"

class ctMSDOfPolymerTargetImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctMSDOfPolymerTargetImpl();

	virtual ~ctMSDOfPolymerTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void MSDPolymerTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTMSDOFPOLYMERTARGETIMPL_H__9BDE3E1D_6BD3_47D1_BF49_A530FE2BD5FF__INCLUDED_)
