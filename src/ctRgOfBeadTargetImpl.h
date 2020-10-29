// ctRgOfBeadTargetImpl.h: interface for the ctRgOfBeadTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRGOFBEADTARGETIMPL_H__04ec17da_d60e_44c4_9634_5cbc2fb36ca1__INCLUDED_)
#define      AFX_CTRGOFBEADTARGETIMPL_H__04ec17da_d60e_44c4_9634_5cbc2fb36ca1__INCLUDED_


#include "ISimBoxCmd.h"

class ctRgOfBeadTargetImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctRgOfBeadTargetImpl();

	virtual ~ctRgOfBeadTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RgBeadTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTRGOFBEADTARGETIMPL_H__04ec17da_d60e_44c4_9634_5cbc2fb36ca1__INCLUDED_)
