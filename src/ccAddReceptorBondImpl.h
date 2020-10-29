// ccAddReceptorBondImpl.h: interface for the ccAddReceptorBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDRECEPTORBONDIMPL_H__AE1D2531_88F9_4CCD_BD4B_E70F6855794F__INCLUDED_)
#define AFX_CCADDRECEPTORBONDIMPL_H__AE1D2531_88F9_4CCD_BD4B_E70F6855794F__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddReceptorBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddReceptorBondImpl();

	virtual ~ccAddReceptorBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddReceptorBond(const xxCommand* const pCommand);

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


#endif // !defined(AFX_CCADDRECEPTORBONDIMPL_H__AE1D2531_88F9_4CCD_BD4B_E70F6855794F__INCLUDED_)
