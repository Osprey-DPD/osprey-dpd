// ccAddArp23BondImpl.h: interface for the ccAddArp23BondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDARP23BONDIMPL_H__B9BB7260_30C0_463D_B150_77391FBC5F27__INCLUDED_)
#define AFX_CCADDARP23BONDIMPL_H__B9BB7260_30C0_463D_B150_77391FBC5F27__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddArp23BondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddArp23BondImpl();

	virtual ~ccAddArp23BondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddArp23Bond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCADDARP23BONDIMPL_H__B9BB7260_30C0_463D_B150_77391FBC5F27__INCLUDED_)
