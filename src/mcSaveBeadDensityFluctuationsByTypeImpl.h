// mcSaveBeadDensityFluctuationsByTypeImpl.h: interface for the mcSaveBeadDensityFluctuationsByTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPEIMPL_H__B92066F5_64C8_4719_BDDB_96A55D16746C__INCLUDED_)
#define AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPEIMPL_H__B92066F5_64C8_4719_BDDB_96A55D16746C__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveBeadDensityFluctuationsByTypeImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveBeadDensityFluctuationsByTypeImpl();

	virtual ~mcSaveBeadDensityFluctuationsByTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveBeadDensityFluctuationsByType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEBEADDENSITYFLUCTUATIONSBYTYPEIMPL_H__B92066F5_64C8_4719_BDDB_96A55D16746C__INCLUDED_)
