// ccAddfActinBondImpl.h: interface for the ccAddfActinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDFACTINBONDIMPL_H__09FA9DC2_0BBF_44D3_A927_83587A69C9D8__INCLUDED_)
#define AFX_CCADDFACTINBONDIMPL_H__09FA9DC2_0BBF_44D3_A927_83587A69C9D8__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddfActinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddfActinBondImpl();

	virtual ~ccAddfActinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddfActinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCADDFACTINBONDIMPL_H__09FA9DC2_0BBF_44D3_A927_83587A69C9D8__INCLUDED_)
