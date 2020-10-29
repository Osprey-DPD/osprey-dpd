// ISimCmd.h: interface for the ISimCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMCMD_H__AD573873_E5FD_11D3_BF23_004095086186__INCLUDED_)
#define AFX_ISIMCMD_H__AD573873_E5FD_11D3_BF23_004095086186__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxBase.h"
#include "ISimBoxCmd.h"

class ISimCmd : public ISimBoxBase, public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction: Protected constructor declared below
public:

	virtual ~ISimCmd();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	ISimCmd(const ISimBox* const pISimBox);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

	// Interface class to allow ccXXX and mcXXX command classes to route their
	// commands to the CSimBox and CMonitor. Commands destined for the CSimBox
	// are specified in the ISimBoxCmd interface, whereas those destined for the
	// CMonitor are specified in the IMonitorCmd class but are routed through the
	// ISimBoxBase class.

};

#endif // !defined(AFX_ISIMCMD_H__AD573873_E5FD_11D3_BF23_004095086186__INCLUDED_)
