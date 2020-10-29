// ctListCommandTargetActivitiesImpl.h: interface for the ctListCommandTargetActivitiesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLISTCOMMANDTARGETACTIVITIESIMPL_H__FAAC99EB_FDD5_49BB_9E23_00CCEC6976E0__INCLUDED_)
#define AFX_CTLISTCOMMANDTARGETACTIVITIESIMPL_H__FAAC99EB_FDD5_49BB_9E23_00CCEC6976E0__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctListCommandTargetActivitiesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctListCommandTargetActivitiesImpl();

	virtual ~ctListCommandTargetActivitiesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ListCommandTargetActivities(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTLISTCOMMANDTARGETACTIVITIESIMPL_H__FAAC99EB_FDD5_49BB_9E23_00CCEC6976E0__INCLUDED_)
