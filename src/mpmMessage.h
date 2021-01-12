// mpmMessage.h: interface for the mpmMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPMMESSAGE_H__D543BB25_23D7_42A1_8A49_09DFEEA42265__INCLUDED_)
#define AFX_MPMMESSAGE_H__D543BB25_23D7_42A1_8A49_09DFEEA42265__INCLUDED_


// Forward declarations


// Include files that contain flags to control conditional compilation
// of the derived classes, and XML-related flags included here to avoid
// every derived class having to explicitly include it.

#include "SimMPSFlags.h"
#include "SimXMLFlags.h"

#if EnableParallelSimBox == SimulationEnabled
    #include "mpi.h"
#endif

// Include file for the Message Factory object

#include "acfParallelMessageFactory.h"


#include "IGlobalSimBox.h"

class mpmMessage : public IGlobalSimBox  
{
public:

#if EnableParallelSimBox == SimulationEnabled
    typedef MPI_Datatype* MsgDataTypePtr;
    typedef MPI_Request*  MsgRequestPtr;
#else
    typedef long* MsgDataTypePtr;
    typedef long* MsgRequestPtr;
#endif


	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~mpmMessage();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const mpmMessage& message);
	friend zInStream&  operator>>(zInStream& is,        mpmMessage& message);

	static long GetMessageTotal();
	static void ZeroMessageTotal();

	static long m_MessageTotal;	// Number of messages created

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an mpmMessage pointer,

	virtual zOutStream& put(zOutStream& os) const = 0;
	virtual zInStream&  get(zInStream& is)        = 0;

	virtual const zString GetMessageType() const  = 0;

	virtual const mpmMessage* GetMessage()  const = 0;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden when the derived class wants to call the 
    // corresponding MPI function.

    virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType) = 0;
    virtual bool Validate()   = 0;
    
	virtual void Send(long procId);
	virtual void SendP0();
	virtual void SendAll();
	virtual void SendAllP();
	virtual void SendSelf();
	virtual void Broadcast();

    // Ooverloaded functions to receive a message
	virtual void Receive();
    virtual void Receive(long procId);

    // Non-blocking functions

    virtual void ISend(long procId, MsgRequestPtr pRequest);

	virtual void IReceive(long procId, MsgRequestPtr pRequest);

	virtual bool Wait(MsgRequestPtr pRequest);  // Function to complete a non-blocking ISend/IReceive

	virtual bool Test(MsgRequestPtr pRequest);  // Function to complete a non-blocking ISend/IReceive

	// ****************************************
	// Public access functions
public:

	inline long GetId()	       const {return m_id;}
	inline long GetTag()       const {return m_Tag;}
    inline long GetSenderPid() const {return m_SenderPid;}

	void SetTag(long tag);
	void SetSenderPid(long pid);

    // Function to copy the data into the message instance. This function
    // is implemented by each derived class and given an argument that allows
    // the message class to obtain the data to be propagated to other processors.
    // Alternatively, several SetXXXData() functions may be provided to set
    // different pieces of the data.

// e.g.,  void SetMessageData(const XXX* const pData);


    // Accessor functions must be provided to allow the receiving objects to 
    // extract their data from the message class.

// e.g.,  inline long GetXXX() const;

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	mpmMessage();
	
	mpmMessage(const mpmMessage& oldMessage);

	// ****************************************
	// Implementation

	// XML output
	zOutStream& putXMLStartTags(zOutStream& os) const;
	zOutStream& putXMLEndTags(zOutStream& os) const;

	// ASCII output
	zOutStream& putASCIIStartTags(zOutStream& os) const;
	zOutStream& putASCIIEndTags(zOutStream& os) const;

protected:

    // Derived classes create and store these objects

	long  m_Tag;         // Identifying tag for each use of a message instance
	long  m_SenderPid;   // Pid of the sending processor
	long  m_ErrorCode;   // Error code used by derived classes to check message passing

    // ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const long m_id;    // Unique identifier for the message


};

#endif // !defined(AFX_MPMMESSAGE_H__D543BB25_23D7_42A1_8A49_09DFEEA42265__INCLUDED_)
