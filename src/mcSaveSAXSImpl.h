// mcSaveSAXSImpl.h: interface for the mcSaveSAXSImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVESAXSIMPL_H__16d107ec_821d_41e4_9906_4f5272dd854b__INCLUDED_)
#define AFX_MCSAVESAXSIMPL_H__16d107ec_821d_41e4_9906_4f5272dd854b__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveSAXSImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveSAXSImpl();

	virtual ~mcSaveSAXSImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveSAXS(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVESAXSIMPL_H__16d107ec_821d_41e4_9906_4f5272dd854b__INCLUDED_)
