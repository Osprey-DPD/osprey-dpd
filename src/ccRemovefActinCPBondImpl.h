// ccRemovefActinCPBondImpl.h: interface for the ccRemovefActinCPBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEFACTINCPBONDIMPL_H__49247C92_F191_43DB_B773_F8B1E09D512F__INCLUDED_)
#define AFX_CCREMOVEFACTINCPBONDIMPL_H__49247C92_F191_43DB_B773_F8B1E09D512F__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemovefActinCPBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemovefActinCPBondImpl();

	virtual ~ccRemovefActinCPBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemovefActinCPBond(const xxCommand* const pCommand);

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
#endif // !defined(AFX_CCREMOVEFACTINCPBONDIMPL_H__49247C92_F191_43DB_B773_F8B1E09D512F__INCLUDED_)
