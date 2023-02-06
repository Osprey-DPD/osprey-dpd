// mcSavePairCorrelationFunctionImpl.h: interface for the mcSavePairCorrelationFunctionImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPAIRCORRELATIONFUNCTIONIMPL_H__58cc8062_f529_4ddb_a075_70ced24b4a97__INCLUDED_)
#define AFX_MCSAVEPAIRCORRELATIONFUNCTIONIMPL_H__58cc8062_f529_4ddb_a075_70ced24b4a97__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSavePairCorrelationFunctionImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSavePairCorrelationFunctionImpl();

	virtual ~mcSavePairCorrelationFunctionImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SavePairCorrelationFunction(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEPAIRCORRELATIONFUNCTIONIMPL_H__58cc8062_f529_4ddb_a075_70ced24b4a97__INCLUDED_)
