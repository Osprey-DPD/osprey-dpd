// LogPanCurrentStateCamera.h: interface for the CLogPanCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPANCURRENTSTATECAMERA_H__B4AD6F3E_FE0F_47CD_BAE4_214DBCBB1A04__INCLUDED_)
#define AFX_LOGPANCURRENTSTATECAMERA_H__B4AD6F3E_FE0F_47CD_BAE4_214DBCBB1A04__INCLUDED_


#include "LogInfoMessage.h"

class CLogPanCurrentStateCamera : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogPanCurrentStateCamera(long time, const bool bInitial, const bool bFinal,
							  const long duration, const long frames, const long steps,
							  const double angle, const double normal[3],
							  const double camera[3], const double target[3]);

	virtual ~CLogPanCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogPanCurrentStateCamera& rMsg);

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

	CLogPanCurrentStateCamera(const CLogPanCurrentStateCamera& oldMessage);
	CLogPanCurrentStateCamera& operator=(const CLogPanCurrentStateCamera& rhs);


	// ****************************************
	// Data members
private:

	const bool	 m_bInitial;		// Flag for "jump to initial coords"
	const bool	 m_bFinal;			// Flag for "stay at final coords"

	const long	 m_Duration;		// Duration of pan in seconds
	const long	 m_FrameRate;		// No of snapshots per second
	const long	 m_StepsPerFrame;	// Simulation time between frames
	const double m_PanAngle;		// Angle through which to pan the camera (degrees)

	double		 m_Normal[3];		// Normal to plane of pan
	double		 m_Camera[3];		// Camera coordinates (multiple of SimBox side lengths)
	double		 m_Target[3];		// Target (look-at) coordinates      "

};

#endif // !defined(AFX_LOGPANCURRENTSTATECAMERA_H__B4AD6F3E_FE0F_47CD_BAE4_214DBCBB1A04__INCLUDED_)
