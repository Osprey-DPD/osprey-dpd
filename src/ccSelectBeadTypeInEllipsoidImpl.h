// ccSelectBeadTypeInEllipsoidImpl.h: interface for the ccSelectBeadTypeInEllipsoidImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINELLIPSOIDIMPL_H__A3D87D4B_97B7_4A58_AB77_B0A824020898__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINELLIPSOIDIMPL_H__A3D87D4B_97B7_4A58_AB77_B0A824020898__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInEllipsoidImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInEllipsoidImpl();

	virtual ~ccSelectBeadTypeInEllipsoidImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInEllipsoid(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTBEADTYPEINELLIPSOIDIMPL_H__A3D87D4B_97B7_4A58_AB77_B0A824020898__INCLUDED_)
