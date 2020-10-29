// mcSaveBeadKineticTemperaturesImpl.h: interface for the mcSaveBeadKineticTemperaturesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADKINETICTEMPERATURESIMPL_H__040700E4_7D10_423C_B6DE_8E27906A9C73__INCLUDED_)
#define AFX_MCSAVEBEADKINETICTEMPERATURESIMPL_H__040700E4_7D10_423C_B6DE_8E27906A9C73__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveBeadKineticTemperaturesImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveBeadKineticTemperaturesImpl();

	virtual ~mcSaveBeadKineticTemperaturesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveBeadKineticTemperatures(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEBEADKINETICTEMPERATURESIMPL_H__040700E4_7D10_423C_B6DE_8E27906A9C73__INCLUDED_)
