// pmSetCurrentStateCamera.h: interface for the pmSetCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETCURRENTSTATECAMERA_H__B77DD2BF_86C5_492D_B6A3_BF664522FA1F__INCLUDED_)
#define AFX_PMSETCURRENTSTATECAMERA_H__B77DD2BF_86C5_492D_B6A3_BF664522FA1F__INCLUDED_


// Forward declarations

class mcSetCurrentStateCamera;


#include "mpmCommandWrapper.h"

class pmSetCurrentStateCamera : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetCurrentStateCamera();
	
	pmSetCurrentStateCamera(const pmSetCurrentStateCamera& oldMessage);

	virtual ~pmSetCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetCurrentStateCamera& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetCurrentStateCamera& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetCurrentStateCamera pointer,

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

	inline double	GetCamera(short int i) const {return m_Camera[i];}
	inline double	GetTarget(short int i) const {return m_Target[i];}

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

	double	m_Camera[3];			// Camera coordinates as multiple of SimBox size
	double	m_Target[3];			// Target (look at) coordinates        "
};

#endif // !defined(AFX_PMSETCURRENTSTATECAMERA_H__B77DD2BF_86C5_492D_B6A3_BF664522FA1F__INCLUDED_)
