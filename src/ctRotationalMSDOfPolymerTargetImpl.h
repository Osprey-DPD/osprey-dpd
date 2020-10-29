// ctRotationalMSDOfPolymerTargetImpl.h: interface for the ctRotationalMSDOfPolymerTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTROTATIONALMSDOFPOLYMERTARGETIMPL_H__49EB4DF8_A3F6_4ECA_8187_97A859978043__INCLUDED_)
#define AFX_CTROTATIONALMSDOFPOLYMERTARGETIMPL_H__49EB4DF8_A3F6_4ECA_8187_97A859978043__INCLUDED_


#include "ISimBoxCmd.h"

class ctRotationalMSDOfPolymerTargetImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctRotationalMSDOfPolymerTargetImpl();

	virtual ~ctRotationalMSDOfPolymerTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RotationalMSDPolymerTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTROTATIONALMSDOFPOLYMERTARGETIMPL_H__49EB4DF8_A3F6_4ECA_8187_97A859978043__INCLUDED_)
