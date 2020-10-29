// ccSelectPolymerTypeHeadInPentagonImpl.h: interface for the ccSelectPolymerTypeHeadInPentagonImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINPENTAGONIMPL_H__E8432850_6614_496A_9E6E_C34F52A41870__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINPENTAGONIMPL_H__E8432850_6614_496A_9E6E_C34F52A41870__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectPolymerTypeHeadInPentagonImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectPolymerTypeHeadInPentagonImpl();

	virtual ~ccSelectPolymerTypeHeadInPentagonImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectPolymerTypeHeadInPentagon(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINPENTAGONIMPL_H__E8432850_6614_496A_9E6E_C34F52A41870__INCLUDED_)
