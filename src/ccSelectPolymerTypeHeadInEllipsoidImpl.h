// ccSelectPolymerTypeHeadInEllipsoidImpl.h: interface for the ccSelectPolymerTypeHeadInEllipsoidImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOIDIMPL_H__D84DA5BB_21DC_437C_8591_37C16A89EEFF__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOIDIMPL_H__D84DA5BB_21DC_437C_8591_37C16A89EEFF__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInEllipsoidImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInEllipsoidImpl();

	virtual ~ccSelectPolymerTypeHeadInEllipsoidImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInEllipsoid(const xxCommand* const pCommand);


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void CalculateRotationMatrix(double theta, double phi);

	// ****************************************
	// Data members
private:

	double m_RNormal[3][3];		// Rotation matrix for ellipsoid's normal 

};

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOIDIMPL_H__D84DA5BB_21DC_437C_8591_37C16A89EEFF__INCLUDED_)
