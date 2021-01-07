// LogZoomCurrentStateCamera.h: interface for the CLogZoomCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGZOOMCURRENTSTATECAMERA_H__75A7F3D4_A60E_43FE_84FC_25B68725314B__INCLUDED_)
#define AFX_LOGZOOMCURRENTSTATECAMERA_H__75A7F3D4_A60E_43FE_84FC_25B68725314B__INCLUDED_


#include "LogInfoMessage.h"

class CLogZoomCurrentStateCamera : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogZoomCurrentStateCamera(long time, const long duration, const long frames, const long steps,
							  const double scaleFactor, const double camera[3], const double target[3]);

	virtual ~CLogZoomCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogZoomCurrentStateCamera& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogZoomCurrentStateCamera(const CLogZoomCurrentStateCamera& oldMessage);
	CLogZoomCurrentStateCamera& operator=(const CLogZoomCurrentStateCamera& rhs);


	// ****************************************
	// Data members
private:

	const long	 m_Duration;		// Duration of zoom in seconds
	const long	 m_FrameRate;		// No of snapshots per second
	const long	 m_StepsPerFrame;	// Simulation time between frames
	const double m_ScaleFactor;		// Zoom scale factor per shapshot

	double		 m_Camera[3];		// Camera coordinates (multiple of SimBox side lengths)
	double		 m_Target[3];		// Target (look-at) coordinates      "

};
#endif // !defined(AFX_LOGZOOMCURRENTSTATECAMERA_H__75A7F3D4_A60E_43FE_84FC_25B68725314B__INCLUDED_)
