// mcZoomCurrentStateCameraImpl.h: interface for the mcZoomCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCZOOMCURRENTSTATECAMERAIMPL_H__1FC9E806_414E_40C7_9489_E049F6DCFA81__INCLUDED_)
#define AFX_MCZOOMCURRENTSTATECAMERAIMPL_H__1FC9E806_414E_40C7_9489_E049F6DCFA81__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcZoomCurrentStateCameraImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcZoomCurrentStateCameraImpl();

	virtual ~mcZoomCurrentStateCameraImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ZoomCurrentStateCamera(const xxCommand* const pCommand);


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

	double m_OriginalCamera[3];		// Pre-command position of camera (abs units)
	double m_OriginalTarget[3];		// Pre-command position of target (abs units)
	double m_FinalCamera[3];		// Post-command position of camera (abs units)

	double m_OriginalSeparation;	// Pre-command camera/target separation
	double m_FinalSeparation;		// Final camera/target separation

};

#endif // !defined(AFX_MCZOOMCURRENTSTATECAMERAIMPL_H__1FC9E806_414E_40C7_9489_E049F6DCFA81__INCLUDED_)
