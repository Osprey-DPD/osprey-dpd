// ccSelectPolymerTypeHeadInSliceImpl.h: interface for the ccSelectPolymerTypeHeadInSliceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINSLICEIMPL_H__A1F4B5F7_04A8_4159_AEAE_0296F2F59C53__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINSLICEIMPL_H__A1F4B5F7_04A8_4159_AEAE_0296F2F59C53__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInSliceImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInSliceImpl();

	virtual ~ccSelectPolymerTypeHeadInSliceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInSlice(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINSLICEIMPL_H__A1F4B5F7_04A8_4159_AEAE_0296F2F59C53__INCLUDED_)
