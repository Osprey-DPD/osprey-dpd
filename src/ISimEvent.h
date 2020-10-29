// ISimEvent.h: interface for the ISimEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMEVENT_H__57692760_65C3_498E_9E61_6D2B6F6A581F__INCLUDED_)
#define AFX_ISIMEVENT_H__57692760_65C3_498E_9E61_6D2B6F6A581F__INCLUDED_


#include "ISimCmd.h"

class ISimEvent : public ISimCmd
{
	// ****************************************
	// Construction/Destruction: Protected constructor declared below
public:

	virtual ~ISimEvent();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function to refresh the CSimBox's set of events when the CSimState changes

	virtual	void  RefreshEvents() = 0;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	ISimEvent(const ISimBox* const pISimBox);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

};

#endif // !defined(AFX_ISIMEVENT_H__57692760_65C3_498E_9E61_6D2B6F6A581F__INCLUDED_)
