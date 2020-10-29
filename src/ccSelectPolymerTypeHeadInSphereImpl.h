// ccSelectPolymerTypeHeadInSphereImpl.h: interface for the ccSelectPolymerTypeHeadInSphereImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINSPHEREIMPL_H__572300d7_2094_40fc_af08_9e9e9246f193__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINSPHEREIMPL_H__572300d7_2094_40fc_af08_9e9e9246f193__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInSphereImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInSphereImpl();

	virtual ~ccSelectPolymerTypeHeadInSphereImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInSphere(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINSPHEREIMPL_H__572300d7_2094_40fc_af08_9e9e9246f193__INCLUDED_)
