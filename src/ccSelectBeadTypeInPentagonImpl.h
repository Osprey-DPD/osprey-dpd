// ccSelectBeadTypeInPentagonImpl.h: interface for the ccSelectBeadTypeInPentagonImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINPENTAGONIMPL_H__DC825A48_2CA2_40FC_AEFE_A478EAE84A0C__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINPENTAGONIMPL_H__DC825A48_2CA2_40FC_AEFE_A478EAE84A0C__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectBeadTypeInPentagonImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectBeadTypeInPentagonImpl();

	virtual ~ccSelectBeadTypeInPentagonImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadTypeInPentagon(const xxCommand* const pCommand);


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

	double m_RNormal[3][3];		// Rotation matrix for pentagon's normal 

};

#endif // !defined(AFX_CCSELECTBEADTYPEINPENTAGONIMPL_H__DC825A48_2CA2_40FC_AEFE_A478EAE84A0C__INCLUDED_)
