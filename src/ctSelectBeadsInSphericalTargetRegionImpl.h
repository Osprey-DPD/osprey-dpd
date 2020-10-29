// ctSelectBeadsInSphericalTargetRegionImpl.h: interface for the ctSelectBeadsInSphericalTargetRegionImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSELECTBEADSINSPHERICALTARGETREGIONIMPL_H__2E57997A_714A_4DE7_A2C1_F3743209AA10__INCLUDED_)
#define AFX_CTSELECTBEADSINSPHERICALTARGETREGIONIMPL_H__2E57997A_714A_4DE7_A2C1_F3743209AA10__INCLUDED_


#include "ISimBoxCmd.h"

class ctSelectBeadsInSphericalTargetRegionImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSelectBeadsInSphericalTargetRegionImpl();

	virtual ~ctSelectBeadsInSphericalTargetRegionImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectBeadsInSphericalTargetRegion(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSELECTBEADSINSPHERICALTARGETREGIONIMPL_H__2E57997A_714A_4DE7_A2C1_F3743209AA10__INCLUDED_)
