// ctExtractBeadTypesIntoCompositeTargetImpl.h: interface for the ctExtractBeadTypesIntoCompositeTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGETIMPL_H__D26A09B6_4424_465E_9DC6_2D5E7BBC84D0__INCLUDED_)
#define AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGETIMPL_H__D26A09B6_4424_465E_9DC6_2D5E7BBC84D0__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctExtractBeadTypesIntoCompositeTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctExtractBeadTypesIntoCompositeTargetImpl();

	virtual ~ctExtractBeadTypesIntoCompositeTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ExtractBeadTypesIntoCompositeTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTEXTRACTBEADTYPESINTOCOMPOSITETARGETIMPL_H__D26A09B6_4424_465E_9DC6_2D5E7BBC84D0__INCLUDED_)
