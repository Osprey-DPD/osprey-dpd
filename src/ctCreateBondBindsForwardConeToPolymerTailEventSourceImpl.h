// ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl.h: interface for the ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERTAILEVENTSOURCEIMPL_H__8423592C_29F2_42C0_803B_DBA2874BD9BB__INCLUDED_)
#define AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERTAILEVENTSOURCEIMPL_H__8423592C_29F2_42C0_803B_DBA2874BD9BB__INCLUDED_


#include "ISimBoxCmd.h"

class ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl();

	virtual ~ctCreateBondBindsForwardConeToPolymerTailEventSourceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void CreateBondBindsForwardConeToPolymerTailEventSource(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCREATEBONDBINDSFORWARDCONETOPOLYMERTAILEVENTSOURCEIMPL_H__8423592C_29F2_42C0_803B_DBA2874BD9BB__INCLUDED_)
