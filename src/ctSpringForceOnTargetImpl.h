// ctSpringForceOnTargetImpl.h: interface for the ctSpringForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSPRINGFORCEONTARGETIMPL_H__be901acf_1fca_4921_958a_ca87d560aebe__INCLUDED_)
#define AFX_CTSPRINGFORCEONTARGETIMPL_H__be901acf_1fca_4921_958a_ca87d560aebe__INCLUDED_


#include "ISimBoxCmd.h"

class ctSpringForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSpringForceOnTargetImpl();

	virtual ~ctSpringForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SpringForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSPRINGFORCEONTARGETIMPL_H__be901acf_1fca_4921_958a_ca87d560aebe__INCLUDED_)
