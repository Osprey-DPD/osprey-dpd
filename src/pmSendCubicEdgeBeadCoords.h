// pmSendCubicEdgeBeadCoords.h: interface for the pmSendCubicEdgeBeadCoords class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDCUBICEDGEBEADCOORDS_H__07F67839_94CF_45C2_8CB6_690C501303B6__INCLUDED_)
#define AFX_PMSENDCUBICEDGEBEADCOORDS_H__07F67839_94CF_45C2_8CB6_690C501303B6__INCLUDED_


#include "mpmMessage.h"

class pmSendCubicEdgeBeadCoords : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendCubicEdgeBeadCoords();
	
	pmSendCubicEdgeBeadCoords(const pmSendCubicEdgeBeadCoords& oldMessage);

	virtual ~pmSendCubicEdgeBeadCoords();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendCubicEdgeBeadCoords& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendCubicEdgeBeadCoords& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

    static const long m_MaxMsgBuffer = 100000; // Max number of chars in message buffer
    static const long m_MaxMsgBeads  = 10000;   // Max number of beads whose coordinates are sent

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendCubicEdgeBeadCoords pointer,

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

    // Function to store the bead coordinates in the message

	void SetMessageData(BeadList lBeads);

    inline long GetSentBeadTotal() const {return m_lBeads.size();}

    // Accessor functions to the message data

    inline long GetRecBeadTotal()  const {return m_RecTotal;}

    inline zLongVector&   GetBeadIds()   {return m_vIds;}
    inline zLongVector&   GetBeadTypes() {return m_vTypes;}
    inline zDoubleVector& GetX()         {return m_vLX;}
    inline zDoubleVector& GetY()         {return m_vLY;}
    inline zDoubleVector& GetZ()         {return m_vLZ;}
    inline zDoubleVector& GetVX()        {return m_vVX;}
    inline zDoubleVector& GetVY()        {return m_vVY;}
    inline zDoubleVector& GetVZ()        {return m_vVZ;}

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

    char m_SendBuffer[m_MaxMsgBuffer]; // Buffer used in non-blocking send operation
    char m_RecBuffer[m_MaxMsgBuffer];  // Buffer used in non-blocking receive operation

    // ********************
    // Data used when sending the message

    BeadList m_lBeads;    // Set of beads whose coordinates are to be sent

    // Arrays used to hold bead data before inserting into message buffer

    long     m_Ids[m_MaxMsgBeads];
    long     m_Types[m_MaxMsgBeads];
    double   m_X[m_MaxMsgBeads];
    double   m_Y[m_MaxMsgBeads];
    double   m_Z[m_MaxMsgBeads];
    double   m_VX[m_MaxMsgBeads];
    double   m_VY[m_MaxMsgBeads];
    double   m_VZ[m_MaxMsgBeads];

    // ********************
    // Data used when receiving the message

    long m_RecTotal;  // Number of beads whose coordinates are received

    zLongVector    m_vIds;
    zLongVector    m_vTypes;
    zDoubleVector  m_vLX;
    zDoubleVector  m_vLY;    // Bead coordinate data
    zDoubleVector  m_vLZ;
    zDoubleVector  m_vVX;
    zDoubleVector  m_vVY;
    zDoubleVector  m_vVZ;

};

#endif // !defined(AFX_PMSENDCUBICEDGEBEADCOORDS_H__07F67839_94CF_45C2_8CB6_690C501303B6__INCLUDED_)
