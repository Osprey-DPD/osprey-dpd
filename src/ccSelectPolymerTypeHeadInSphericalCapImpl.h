// ccSelectPolymerTypeHeadInSphericalCapImpl.h: interface for the ccSelectPolymerTypeHeadInSphericalCapImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINSPHERICALCAPIMPL_H__720ced25_9b05_425a_921c_7f123baecb42__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINSPHERICALCAPIMPL_H__720ced25_9b05_425a_921c_7f123baecb42__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInSphericalCapImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInSphericalCapImpl();

	virtual ~ccSelectPolymerTypeHeadInSphericalCapImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInSphericalCap(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINSPHERICALCAPIMPL_H__720ced25_9b05_425a_921c_7f123baecb42__INCLUDED_)
