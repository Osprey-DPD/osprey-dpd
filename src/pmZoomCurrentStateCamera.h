// pmZoomCurrentStateCamera.h: interface for the pmZoomCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMZOOMCURRENTSTATECAMERA_H__F96445BE_774F_4B22_A1A7_4B313FD94547__INCLUDED_)
#define AFX_PMZOOMCURRENTSTATECAMERA_H__F96445BE_774F_4B22_A1A7_4B313FD94547__INCLUDED_


// Forward declarations

class mcSetCurrentStateCamera;


#include "mpmCommandWrapper.h"

class pmZoomCurrentStateCamera : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmZoomCurrentStateCamera();
	pmZoomCurrentStateCamera(const pmZoomCurrentStateCamera& oldMessage);

	virtual ~pmZoomCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmZoomCurrentStateCamera& message);
	friend zInStream&  operator>>(zInStream& is,        pmZoomCurrentStateCamera& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmZoomCurrentStateCamera pointer,

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

	inline long		GetDuration()				const {return m_Duration;}
	inline long     GetFrameRate()				const {return m_FrameRate;}
	inline long     GetStepsPerFrame()			const {return m_StepsPerFrame;}
	inline double	GetScaleFactor()			const {return m_ScaleFactor;}

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

	long	m_Duration;				// Duration of pan in seconds
	long	m_FrameRate;			// No of snapshots per second
	long	m_StepsPerFrame;		// Simulation time between frames
	double  m_ScaleFactor;			// Zoom factor per snapshot (0,infinity)
};

#endif // !defined(AFX_PMZOOMCURRENTSTATECAMERA_H__F96445BE_774F_4B22_A1A7_4B313FD94547__INCLUDED_)
