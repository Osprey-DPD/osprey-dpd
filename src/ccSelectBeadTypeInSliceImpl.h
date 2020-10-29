// ccSelectBeadTypeInSliceImpl.h: interface for the ccSelectBeadTypeInSliceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSLICEIMPL_H__233a7392_6bca_4581_a337_7bb9bb8e8d42__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSLICEIMPL_H__233a7392_6bca_4581_a337_7bb9bb8e8d42__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInSliceImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInSliceImpl();

	virtual ~ccSelectBeadTypeInSliceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInSlice(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINSLICEIMPL_H__233a7392_6bca_4581_a337_7bb9bb8e8d42__INCLUDED_)
