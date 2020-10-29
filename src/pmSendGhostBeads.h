// pmSendGhostBeads.h: interface for the pmSendGhostBeads class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDGHOSTBEADS_H__E01475A4_52D5_4091_A798_0BC0F779525F__INCLUDED_)
#define AFX_PMSENDGHOSTBEADS_H__E01475A4_52D5_4091_A798_0BC0F779525F__INCLUDED_


#include "mpmMessage.h"

class pmSendGhostBeads : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendGhostBeads();
	
	pmSendGhostBeads(const pmSendGhostBeads& oldMessage);

	virtual ~pmSendGhostBeads();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendGhostBeads& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendGhostBeads& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

    static const long m_MaxMsgBuffer = 100000;     // Max number of chars in message buffer
    static const long m_MaxMsgBeads  = 10000;       // Max number of beads whose coordinates are sent
    static const long m_InitialEmptyBeads = 1000;   // Initial number of empty bead instances
	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendGhostBeads pointer,

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

    // Accessor functions to the message data

	bool CreateEmptyBeads();
	BeadList GetRecBeads();

    inline long GetSentBeadTotal()  const {return m_lBeads.size();}
    inline long GetRecBeadTotal()   const {return m_RecTotal;}
    inline long GetEmptyBeadTotal() const {return m_lEmptyBeads.size();}

    // Accessor functions to the message data

    inline zLongVector&   GetBeadIds()   {return m_vIds;}
    inline zLongVector&   GetBeadTypes() {return m_vTypes;}
    inline zDoubleVector& GetX()         {return m_vLX;}
    inline zDoubleVector& GetY()         {return m_vLY;}
    inline zDoubleVector& GetZ()         {return m_vLZ;}
    inline zDoubleVector& GetUnPBCX()    {return m_vUnPBCX;}
    inline zDoubleVector& GetUnPBCY()    {return m_vUnPBCY;}
    inline zDoubleVector& GetUnPBCZ()    {return m_vUnPBCZ;}

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

    BeadList m_lBeads;       // Set of beads being sent to neighbouring processor
    BeadList m_lRecBeads;    // Set of beads received from neighbouring processor
    BeadList m_lEmptyBeads;  // Set of empty bead instances to receive incoming transfers

    long     m_VisibleFlags; // Integer containing boolean flags for bead visibility
    
    // Arrays used to hold bead data before inserting into message buffer

    long     m_Ids[m_MaxMsgBeads];
	long     m_DisplayIds[m_MaxMsgBeads];
	long     m_Visibility[m_MaxMsgBeads];
    long     m_Types[m_MaxMsgBeads];
    double   m_Radius[m_MaxMsgBeads];       // Hardwired to 0.5 for now

    double   m_X[m_MaxMsgBeads];
    double   m_Y[m_MaxMsgBeads];
    double   m_Z[m_MaxMsgBeads];
    double   m_UnPBCX[m_MaxMsgBeads];
    double   m_UnPBCY[m_MaxMsgBeads];
    double   m_UnPBCZ[m_MaxMsgBeads];

    // ********************
    // Data used when receiving the message

    long m_RecTotal;  // Number of ghost beads whose coordinates are received

	CAbstractBead* m_pBead;  // Bead pointer for use in filling empty bead instances

    zLongVector    m_vIds;        // Ids of ghost beads whose coordinates are passed
    zLongVector    m_vTypes;      // Types of ghost beads
    zLongVector    m_vPolymerIds; // Ids of owning polymers
    zDoubleVector  m_vLX;
    zDoubleVector  m_vLY;    // Bead coordinate data
    zDoubleVector  m_vLZ;
    zDoubleVector  m_vUnPBCX;
    zDoubleVector  m_vUnPBCY;
    zDoubleVector  m_vUnPBCZ;


};

#endif // !defined(AFX_PMSENDGHOSTBEADS_H__E01475A4_52D5_4091_A798_0BC0F779525F__INCLUDED_)
