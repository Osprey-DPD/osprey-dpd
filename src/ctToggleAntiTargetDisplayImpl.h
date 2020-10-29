// ctToggleAntiTargetDisplayImpl.h: interface for the ctToggleAntiTargetDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTOGGLEANTITARGETDISPLAYIMPL_H__8D824A90_BE5D_49FC_9860_D749200A3AEA__INCLUDED_)
#define AFX_CTTOGGLEANTITARGETDISPLAYIMPL_H__8D824A90_BE5D_49FC_9860_D749200A3AEA__INCLUDED_


#include "ISimBoxCmd.h"

class ctToggleAntiTargetDisplayImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctToggleAntiTargetDisplayImpl();

	virtual ~ctToggleAntiTargetDisplayImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleAntiTargetDisplay(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTTOGGLEANTITARGETDISPLAYIMPL_H__8D824A90_BE5D_49FC_9860_D749200A3AEA__INCLUDED_)
