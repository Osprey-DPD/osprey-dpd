// ccSelectBeadTypeInSphericalCapImpl.h: interface for the ccSelectBeadTypeInSphericalCapImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSPHERICALCAPIMPL_H__fdb58ee9_f17d_43ca_a922_376ce54c5398__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSPHERICALCAPIMPL_H__fdb58ee9_f17d_43ca_a922_376ce54c5398__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInSphericalCapImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInSphericalCapImpl();

	virtual ~ccSelectBeadTypeInSphericalCapImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInSphericalCap(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINSPHERICALCAPIMPL_H__fdb58ee9_f17d_43ca_a922_376ce54c5398__INCLUDED_)
