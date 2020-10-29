// cgtToggleAllCommandExecutionInGroupImpl.h: interface for the cgtToggleAllCommandExecutionInGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTTOGGLEALLCOMMANDEXECUTIONINGROUPIMPL_H__2AFAF43E_D035_4F47_A9AD_BA06BA6B5F75__INCLUDED_)
#define AFX_CGTTOGGLEALLCOMMANDEXECUTIONINGROUPIMPL_H__2AFAF43E_D035_4F47_A9AD_BA06BA6B5F75__INCLUDED_



#include "ISimBoxCmd.h"

class cgtToggleAllCommandExecutionInGroupImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtToggleAllCommandExecutionInGroupImpl();

	virtual ~cgtToggleAllCommandExecutionInGroupImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleAllCommandExecutionInGroup(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTTOGGLEALLCOMMANDEXECUTIONINGROUPIMPL_H__2AFAF43E_D035_4F47_A9AD_BA06BA6B5F75__INCLUDED_)
