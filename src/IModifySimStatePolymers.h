// IModifySimStatePolymers.h: interface for the IModifySimStatePolymers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYSIMSTATEPOLYMERS_H__D381DC92_D667_4E12_956B_08478F264DA6__INCLUDED_)
#define AFX_IMODIFYSIMSTATEPOLYMERS_H__D381DC92_D667_4E12_956B_08478F264DA6__INCLUDED_


#include "IModifySimState.h"

class IModifySimStatePolymers : public IModifySimState
{
	// ****************************************
	// Construction/Destruction: 
public:

	IModifySimStatePolymers(CSimState& simState);
    
    virtual ~IModifySimStatePolymers();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	const PolymerVector& GetPolymers() const;

	// ****************************************
	// Protected local functions
protected:

	

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IModifySimStatePolymers(const IModifySimStatePolymers& oldIModify);
	IModifySimStatePolymers& operator=(const IModifySimStatePolymers& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IMODIFYSIMSTATEPOLYMERS_H__D381DC92_D667_4E12_956B_08478F264DA6__INCLUDED_)
