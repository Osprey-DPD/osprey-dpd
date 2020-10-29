// IModifySimState.h: interface for the IModifySimState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYSIMSTATE_H__9654378E_D31C_4CDA_AA51_6771E12AF206__INCLUDED_)
#define AFX_IMODIFYSIMSTATE_H__9654378E_D31C_4CDA_AA51_6771E12AF206__INCLUDED_


#include "SimState.h"

class IModifySimState  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~IModifySimState();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor allows only derived classes to be instantiated

	IModifySimState(CSimState& simState);

	// ****************************************
	// Implementation
protected:

	// Reference to the enclosed CSimState object that is used by 
	// the derived classes to change the CSimState in a controlled
	// manner.

	CSimState&	    m_rSimState;	

	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IModifySimState(const IModifySimState& oldIModify);
	IModifySimState& operator=(const IModifySimState& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IMODIFYSIMSTATE_H__9654378E_D31C_4CDA_AA51_6771E12AF206__INCLUDED_)
