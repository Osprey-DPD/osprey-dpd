// pmSendPlanarEdgeBeadForces.h: interface for the pmSendPlanarEdgeBeadForces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDPLANAREDGEBEADFORCES_H__B916BE2F_AD97_43A9_902D_F4007739D59C__INCLUDED_)
#define AFX_PMSENDPLANAREDGEBEADFORCES_H__B916BE2F_AD97_43A9_902D_F4007739D59C__INCLUDED_


#include "mpmMessage.h"

class pmSendPlanarEdgeBeadForces : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendPlanarEdgeBeadForces();
	
	pmSendPlanarEdgeBeadForces(const pmSendPlanarEdgeBeadForces& oldMessage);

	virtual ~pmSendPlanarEdgeBeadForces();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendPlanarEdgeBeadForces& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendPlanarEdgeBeadForces& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

    static const long m_MaxMsgBuffer = 100000; // Max number of chars in message buffer
    static const long m_MaxMsgBeads  = 1000;   // Max number of beads whose forces are sent

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendPlanarEdgeBeadForces pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

	virtual void ISend(long procId, MsgRequestPtr pRequest);

	virtual void IReceive(long procId, MsgRequestPtr pRequest);

	virtual bool Wait(MsgRequestPtr pRequest);

	// ****************************************
	// Public access functions
public:

    // Function to store the bead forces in the message: we actually pass in the 
    // set of external CNT cells that contain the forces to minimise the arguments.

	void SetMessageData(const CNTCellVector& vExtCells);

    inline long GetSentForceTotal() const {return m_SentForceTotal;}

    // Accessor functions to the message data

    inline long GetRecForceTotal()  const {return m_RecTotal;}

    inline zLongVector&   GetBeadIds()   {return m_vIds;}
    inline zLongVector&   GetForceCounter()   {return m_vForceCounter;}
    inline zDoubleVector& GetFX()        {return m_vFX;}
    inline zDoubleVector& GetFY()        {return m_vFY;}
    inline zDoubleVector& GetFZ()        {return m_vFZ;}

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

    MsgRequestPtr   m_pSendRequest;  // Request token for the send operation
    MsgRequestPtr   m_pRecRequest;   // Request token for the receive operation

    char m_RecBuffer[m_MaxMsgBuffer];   // Buffer used in non-blocking receive operation
    char m_SendBuffer[m_MaxMsgBuffer];  // Buffer used in non-blocking send operation

    // ********************
    // Data used when sending the message

    long m_SentForceTotal;    // Number of force terms being sent

    // Arrays used to hold bead data before inserting into message buffer

    long     m_Ids[m_MaxMsgBeads];
    long     m_ForceCounter[m_MaxMsgBeads];
    double   m_FX[m_MaxMsgBeads];
    double   m_FY[m_MaxMsgBeads];
    double   m_FZ[m_MaxMsgBeads];

    // ********************
    // Data used when receiving the message

    long m_RecTotal;  // Number of beads whose forces are received

    zLongVector    m_vIds;
    zLongVector    m_vForceCounter;
    zDoubleVector  m_vFX;
    zDoubleVector  m_vFY;    // Bead force data
    zDoubleVector  m_vFZ;

};

#endif // !defined(AFX_PMSENDPLANAREDGEBEADFORCES_H__B916BE2F_AD97_43A9_902D_F4007739D59C__INCLUDED_)
