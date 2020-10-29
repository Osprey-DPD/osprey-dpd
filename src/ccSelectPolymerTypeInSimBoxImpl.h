// ccSelectPolymerTypeInSimBoxImpl.h: interface for the ccSelectPolymerTypeInSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEINSIMBOXIMPL_H__512FD785_AE7E_40C4_B751_C8AF69B4CE44__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEINSIMBOXIMPL_H__512FD785_AE7E_40C4_B751_C8AF69B4CE44__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeInSimBoxImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeInSimBoxImpl();

	virtual ~ccSelectPolymerTypeInSimBoxImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeInSimBox(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEINSIMBOXIMPL_H__512FD785_AE7E_40C4_B751_C8AF69B4CE44__INCLUDED_)
