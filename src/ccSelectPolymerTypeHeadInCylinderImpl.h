// ccSelectPolymerTypeHeadInCylinderImpl.h: interface for the ccSelectPolymerTypeHeadInCylinderImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINCYLINDERIMPL_H__3609268b_73bd_423a_bf38_2c110ae2d784__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINCYLINDERIMPL_H__3609268b_73bd_423a_bf38_2c110ae2d784__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInCylinderImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInCylinderImpl();

	virtual ~ccSelectPolymerTypeHeadInCylinderImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInCylinder(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINCYLINDERIMPL_H__3609268b_73bd_423a_bf38_2c110ae2d784__INCLUDED_)
