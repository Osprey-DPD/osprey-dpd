// mcPanCurrentStateCamera.h: interface for the mcPanCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCPANCURRENTSTATECAMERA_H__9CC5C26E_92FC_4C01_B4B6_1D7B8D3450D4__INCLUDED_)
#define AFX_MCPANCURRENTSTATECAMERA_H__9CC5C26E_92FC_4C01_B4B6_1D7B8D3450D4__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcPanCurrentStateCamera : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction: 
public:

	mcPanCurrentStateCamera(long executionTime);
	mcPanCurrentStateCamera(const mcPanCurrentStateCamera& oldCommand);
	
	// Constructor used when creating the command internally
	
	mcPanCurrentStateCamera(long executionTime, bool bLog, bool bInitial, bool bFinal, long duration, long frameRate, long stepsPerFrame, double panAngle,
	                        long normal[3], double camera[3], double target[3]);
	
	virtual ~mcPanCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables

	// ****************************************
	// Public access functions
public:

	inline bool     GetJumpToInitialFlag()			const {return m_bInitial;}
	inline bool     GetStayAtFinalFlag()			const {return m_bFinal;}
	inline long		GetDuration()					const {return m_Duration;}
	inline long     GetFrameRate()					const {return m_FrameRate;}
	inline long     GetStepsPerFrame()				const {return m_StepsPerFrame;}
	inline double   GetPanAngle()					const {return m_PanAngle;}

	inline long		GetNormal(long i)			    const {return m_Normal[i];}
	inline double	GetCamera(long i)			    const {return m_Camera[i];}
	inline double	GetTarget(long i)			    const {return m_Target[i];}

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

	bool	m_bInitial;				// Flag for "jump to initial coords"
	bool	m_bFinal;				// Flag for "stay at final coords"
	long	m_Duration;				// Duration of pan in seconds
	long	m_FrameRate;			// No of snapshots per second
	long	m_StepsPerFrame;		// Simulation time between frames
	double  m_PanAngle;				// Length of pan (degrees)

	long    m_Normal[3];			// Pan normal 
	double	m_Camera[3];			// Initial camera coordinates as multiple of SimBox size
	double	m_Target[3];			// Target (look at) coordinates as multiple of SimBox size
	
	
};

#endif // !defined(AFX_MCPANCURRENTSTATECAMERA_H__9CC5C26E_92FC_4C01_B4B6_1D7B8D3450D4__INCLUDED_)
