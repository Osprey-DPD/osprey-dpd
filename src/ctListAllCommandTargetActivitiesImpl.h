// ctListAllCommandTargetActivitiesImpl.h: interface for the ctListAllCommandTargetActivitiesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLISTALLCOMMANDTARGETACTIVITIESIMPL_H__3CA144ED_33C7_412A_9803_BEFB0B637BE7__INCLUDED_)
#define AFX_CTLISTALLCOMMANDTARGETACTIVITIESIMPL_H__3CA144ED_33C7_412A_9803_BEFB0B637BE7__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctListAllCommandTargetActivitiesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctListAllCommandTargetActivitiesImpl();

	virtual ~ctListAllCommandTargetActivitiesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ListAllCommandTargetActivities(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTLISTALLCOMMANDTARGETACTIVITIESIMPL_H__3CA144ED_33C7_412A_9803_BEFB0B637BE7__INCLUDED_)
