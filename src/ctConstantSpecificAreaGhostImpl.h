// ctConstantSpecificAreaGhostImpl.h: interface for the ctConstantSpecificAreaGhostImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCONSTANTSPECIFICAREAGHOSTIMPL_H_d8e19dd6_d816_465c_9e00_844f071341e3_INCLUDED_)
#define AFX_CTCONSTANTSPECIFICAREAGHOSTIMPL_H_d8e19dd6_d816_465c_9e00_844f071341e3_INCLUDED_


#include "ISimBoxCmd.h"

class ctConstantSpecificAreaGhostImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctConstantSpecificAreaGhostImpl();

	virtual ~ctConstantSpecificAreaGhostImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ConstantSpecificArea(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCONSTANTSPECIFICAREAGHOSTIMPL_H_d8e19dd6_d816_465c_9e00_844f071341e3_INCLUDED_)
