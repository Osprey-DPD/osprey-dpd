// mcSaveProtocolFileImpl.h: interface for the mcSaveProtocolFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPROTOCOLFILEIMPL_H__9A9F92CD_8769_4B82_B3BB_2EE4D27694EC__INCLUDED_)
#define AFX_MCSAVEPROTOCOLFILEIMPL_H__9A9F92CD_8769_4B82_B3BB_2EE4D27694EC__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveProtocolFileImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveProtocolFileImpl();

	virtual ~mcSaveProtocolFileImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	void SaveProtocolFile(const xxCommand* const pCommand);

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


#endif // !defined(AFX_MCSAVEPROTOCOLFILEIMPL_H__9A9F92CD_8769_4B82_B3BB_2EE4D27694EC__INCLUDED_)
