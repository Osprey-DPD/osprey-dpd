// ctDistanceMovedByTargetImpl.h: interface for the ctDistanceMovedByTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTDISTANCEMOVEDBYTARGETIMPL_H__9E7CFBCB_326E_42B8_ABA1_75DEA091001D__INCLUDED_)
#define AFX_CTDISTANCEMOVEDBYTARGETIMPL_H__9E7CFBCB_326E_42B8_ABA1_75DEA091001D__INCLUDED_


#include "ISimBoxCmd.h"

class ctDistanceMovedByTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctDistanceMovedByTargetImpl();

	virtual ~ctDistanceMovedByTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void DistanceMovedByTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTDISTANCEMOVEDBYTARGETIMPL_H__9E7CFBCB_326E_42B8_ABA1_75DEA091001D__INCLUDED_)
