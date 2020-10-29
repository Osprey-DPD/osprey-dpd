// ccRemovefActinBondImpl.h: interface for the ccRemovefActinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEFACTINBONDIMPL_H__DA42855B_8751_43F6_ACD3_31A56F6EA11A__INCLUDED_)
#define AFX_CCREMOVEFACTINBONDIMPL_H__DA42855B_8751_43F6_ACD3_31A56F6EA11A__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemovefActinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemovefActinBondImpl();

	virtual ~ccRemovefActinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemovefActinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCREMOVEFACTINBONDIMPL_H__DA42855B_8751_43F6_ACD3_31A56F6EA11A__INCLUDED_)
