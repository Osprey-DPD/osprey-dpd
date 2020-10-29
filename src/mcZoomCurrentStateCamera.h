// mcZoomCurrentStateCamera.h: interface for the mcZoomCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCZOOMCURRENTSTATECAMERA_H__5537A0EA_B4D4_44D4_8994_874865C6418B__INCLUDED_)
#define AFX_MCZOOMCURRENTSTATECAMERA_H__5537A0EA_B4D4_44D4_8994_874865C6418B__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcZoomCurrentStateCamera : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	mcZoomCurrentStateCamera(long executionTime);
	mcZoomCurrentStateCamera(const mcZoomCurrentStateCamera& oldCommand);

	// Constructor used when creating the command internally
	
	mcZoomCurrentStateCamera(long executionTime, bool bLog, long duration, long frameRate, long stepsPerFrame, double scale);

	virtual ~mcZoomCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	inline long		GetDuration()				const {return m_Duration;}
	inline long     GetFrameRate()				const {return m_FrameRate;}
	inline long     GetStepsPerFrame()			const {return m_StepsPerFrame;}

	inline double	GetScaleFactor()			const {return m_ScaleFactor;}


	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	static const zString m_Type;	// Identifier used in control data file for command

	long	m_Duration;				// Duration of pan in seconds
	long	m_FrameRate;			// No of snapshots per second
	long	m_StepsPerFrame;		// Simulation time between frames

	double  m_ScaleFactor;			// Zoom factor per snapshot (0,infinity)
	
};

#endif // !defined(AFX_MCZOOMCURRENTSTATECAMERA_H__5537A0EA_B4D4_44D4_8994_874865C6418B__INCLUDED_)
