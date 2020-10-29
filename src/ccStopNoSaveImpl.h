// ccStopNoSaveImpl.h: interface for the ccStopNoSaveImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSTOPNOSAVEIMPL_H__9574BB27_E9CF_456A_B497_48544FD29189__INCLUDED_)
#define AFX_CCSTOPNOSAVEIMPL_H__9574BB27_E9CF_456A_B497_48544FD29189__INCLUDED_


#include "ISimBoxCmd.h"

class ccStopNoSaveImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccStopNoSaveImpl();

	virtual ~ccStopNoSaveImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void StopNoSave(const xxCommand* const pCommand);


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
#endif // !defined(AFX_CCSTOPNOSAVEIMPL_H__9574BB27_E9CF_456A_B497_48544FD29189__INCLUDED_)
