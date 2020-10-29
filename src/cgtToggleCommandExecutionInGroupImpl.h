// cgtToggleCommandExecutionInGroupImpl.h: interface for the cgtToggleCommandExecutionInGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTTOGGLECOMMANDEXECUTIONINGROUPIMPL_H__7818E06A_FC87_4BBB_AF10_F6B1FA04DF9C__INCLUDED_)
#define AFX_CGTTOGGLECOMMANDEXECUTIONINGROUPIMPL_H__7818E06A_FC87_4BBB_AF10_F6B1FA04DF9C__INCLUDED_



#include "ISimBoxCmd.h"

class cgtToggleCommandExecutionInGroupImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtToggleCommandExecutionInGroupImpl();

	virtual ~cgtToggleCommandExecutionInGroupImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleCommandExecutionInGroup(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTTOGGLECOMMANDEXECUTIONINGROUPIMPL_H__7818E06A_FC87_4BBB_AF10_F6B1FA04DF9C__INCLUDED_)
