// mcSaveBeadVelocityScalarProduct1dProfileImpl.h: interface for the mcSaveBeadVelocityScalarProduct1dProfileImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILEIMPL_H__71A5E8C9_78E7_46A9_B011_9F019D79CB75__INCLUDED_)
#define AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILEIMPL_H__71A5E8C9_78E7_46A9_B011_9F019D79CB75__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveBeadVelocityScalarProduct1dProfileImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveBeadVelocityScalarProduct1dProfileImpl();

	virtual ~mcSaveBeadVelocityScalarProduct1dProfileImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveBeadVelocityScalarProduct1dProfile(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEBEADVELOCITYSCALARPRODUCT1DPROFILEIMPL_H__71A5E8C9_78E7_46A9_B011_9F019D79CB75__INCLUDED_)
