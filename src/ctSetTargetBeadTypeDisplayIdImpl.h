// ctSetTargetBeadTypeDisplayIdImpl.h: interface for the ctSetTargetBeadTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETBEADTYPEDISPLAYIDIMPL_H__2C618720_A19F_4E6D_B3A2_9E4082A7946C__INCLUDED_)
#define AFX_CTSETTARGETBEADTYPEDISPLAYIDIMPL_H__2C618720_A19F_4E6D_B3A2_9E4082A7946C__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctSetTargetBeadTypeDisplayIdImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSetTargetBeadTypeDisplayIdImpl();

	virtual ~ctSetTargetBeadTypeDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetTargetBeadTypeDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSETTARGETBEADTYPEDISPLAYIDIMPL_H__2C618720_A19F_4E6D_B3A2_9E4082A7946C__INCLUDED_)
