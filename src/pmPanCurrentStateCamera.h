// pmPanCurrentStateCamera.h: interface for the pmPanCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPANCURRENTSTATECAMERA_H__CD839828_8C85_4D74_8C47_889368BD45C5__INCLUDED_)
#define AFX_PMPANCURRENTSTATECAMERA_H__CD839828_8C85_4D74_8C47_889368BD45C5__INCLUDED_


// Forward declarations

class mcSetCurrentStateCamera;


#include "mpmCommandWrapper.h"

class pmPanCurrentStateCamera : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmPanCurrentStateCamera();
		
	pmPanCurrentStateCamera(const pmPanCurrentStateCamera& oldMessage);

	virtual ~pmPanCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmPanCurrentStateCamera& message);
	friend zInStream&  operator>>(zInStream& is,        pmPanCurrentStateCamera& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

    static const long m_MaxMsgBuffer = 1000;   // Max number of chars in message buffer

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmPanCurrentStateCamera pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void SendAllP();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to copy the data from the xxCommand-derived object to the message

	void SetMessageData(const xxCommand* const pCommand);

    // Accessor functions to the message data

	inline bool     GetJumpToInitialFlag()		const {return m_bInitialFlag;}
	inline bool     GetStayAtFinalFlag()		const {return m_bFinalFlag;}
	inline long		GetDuration()				const {return m_Duration;}
	inline long     GetFrameRate()				const {return m_FrameRate;}
	inline long     GetStepsPerFrame()			const {return m_StepsPerFrame;}
	inline double   GetPanAngle()				const {return m_PanAngle;}

	inline long		GetNormal(long i)		    const {return m_Normal[i];}
	inline double	GetCamera(long i)           const {return m_Camera[i];}
	inline double	GetTarget(long i)           const {return m_Target[i];}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetMessageType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    // MPI-related data
	
    char    m_Buffer[m_MaxMsgBuffer];  // Buffer used to send/receive message

	bool	m_bInitialFlag;			// Flag for "jump to initial coords" (1 = true)
	bool	m_bFinalFlag;			// Flag for "stay at final coords"   (1 = true)
	long	m_Duration;				// Duration of pan in seconds
	long	m_FrameRate;			// No of snapshots per second
	long	m_StepsPerFrame;		// Simulation time between frames

	double  m_PanAngle;				// Length of pan (degrees)

	long    m_Normal[3];			// Pan normal 
	double	m_Camera[3];			// Camera coordinates as multiple of SimBox size
	double	m_Target[3];			// Target (look at) coordinates        "
};

#endif // !defined(AFX_PMPANCURRENTSTATECAMERA_H__CD839828_8C85_4D74_8C47_889368BD45C5__INCLUDED_)
