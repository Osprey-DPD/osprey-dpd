// ccSelectBeadTypeInSphereImpl.h: interface for the ccSelectBeadTypeInSphereImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSPHEREIMPL_H__38f250ef_6798_4ad0_b804_ba0a2d67556e__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSPHEREIMPL_H__38f250ef_6798_4ad0_b804_ba0a2d67556e__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInSphereImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInSphereImpl();

	virtual ~ccSelectBeadTypeInSphereImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInSphere(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINSPHEREIMPL_H__38f250ef_6798_4ad0_b804_ba0a2d67556e__INCLUDED_)
