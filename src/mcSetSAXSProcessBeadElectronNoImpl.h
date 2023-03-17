// mcSetSAXSProcessBeadElectronNoImpl.h: interface for the mcSetSAXSProcessBeadElectronNoImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__54ea42d2_3f24_46b8_9a8a_f0c8939f4261__INCLUDED_)
#define AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__54ea42d2_3f24_46b8_9a8a_f0c8939f4261__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetSAXSProcessBeadElectronNoImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetSAXSProcessBeadElectronNoImpl();

	virtual ~mcSetSAXSProcessBeadElectronNoImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetSAXSProcessBeadElectronNo(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETSAXSPROCESSBEADELECTRONNO_H__54ea42d2_3f24_46b8_9a8a_f0c8939f4261__INCLUDED_)
