// ccSelectBeadTypeInSimBoxImpl.h: interface for the ccSelectBeadTypeInSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSIMBOXIMPL_H__19b09b59_dac8_4368_915e_0c2324d174e5)
#define AFX_CCSELECTBEADTYPEINSIMBOXIMPL_H__19b09b59_dac8_4368_915e_0c2324d174e5


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInSimBoxImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInSimBoxImpl();

	virtual ~ccSelectBeadTypeInSimBoxImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInSimBox(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINSIMBOXIMPL_H__19b09b59_dac8_4368_915e_0c2324d174e5)
