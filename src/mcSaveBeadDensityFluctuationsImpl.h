// mcSaveBeadDensityFluctuationsImpl.h: interface for the mcSaveBeadDensityFluctuationsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSIMPL_H__E9DB5510_3BA0_41F3_BFD8_64D47E8E7BC1__INCLUDED_)
#define AFX_MCSAVEBEADDENSITYFLUCTUATIONSIMPL_H__E9DB5510_3BA0_41F3_BFD8_64D47E8E7BC1__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveBeadDensityFluctuationsImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveBeadDensityFluctuationsImpl();

	virtual ~mcSaveBeadDensityFluctuationsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveBeadDensityFluctuations(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSIMPL_H__E9DB5510_3BA0_41F3_BFD8_64D47E8E7BC1__INCLUDED_)
