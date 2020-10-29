// cgtAddCommandToGroupImpl.h: interface for the cgtAddCommandToGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTADDCOMMANDTOGROUPIMPL_H__90B77BB1_5E86_40D1_BA71_EBF2D9FFEE37__INCLUDED_)
#define AFX_CGTADDCOMMANDTOGROUPIMPL_H__90B77BB1_5E86_40D1_BA71_EBF2D9FFEE37__INCLUDED_



#include "ISimBoxCmd.h"

class cgtAddCommandToGroupImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtAddCommandToGroupImpl();

	virtual ~cgtAddCommandToGroupImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddCommandToGroup(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTADDCOMMANDTOGROUPIMPL_H__90B77BB1_5E86_40D1_BA71_EBF2D9FFEE37__INCLUDED_)
