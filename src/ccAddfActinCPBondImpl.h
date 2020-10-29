// ccAddfActinCPBondImpl.h: interface for the ccAddfActinCPBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDFACTINCPBONDIMPL_H__3639C2A8_3BF8_4946_BC7F_ACE0084C6E20__INCLUDED_)
#define AFX_CCADDFACTINCPBONDIMPL_H__3639C2A8_3BF8_4946_BC7F_ACE0084C6E20__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddfActinCPBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddfActinCPBondImpl();

	virtual ~ccAddfActinCPBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddfActinCPBond(const xxCommand* const pCommand);

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
#endif // !defined(AFX_CCADDFACTINCPBONDIMPL_H__3639C2A8_3BF8_4946_BC7F_ACE0084C6E20__INCLUDED_)
