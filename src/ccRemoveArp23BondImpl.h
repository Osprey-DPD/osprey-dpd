// ccRemoveArp23BondImpl.h: interface for the ccRemoveArp23BondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEARP23BONDIMPL_H__3F8AD85B_AADE_4957_B6DE_6ABF4C3B991B__INCLUDED_)
#define AFX_CCREMOVEARP23BONDIMPL_H__3F8AD85B_AADE_4957_B6DE_6ABF4C3B991B__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemoveArp23BondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemoveArp23BondImpl();

	virtual ~ccRemoveArp23BondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemoveArp23Bond(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCREMOVEARP23BONDIMPL_H__3F8AD85B_AADE_4957_B6DE_6ABF4C3B991B__INCLUDED_)
