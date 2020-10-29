// pmSendLinearBeads.h: interface for the pmSendLinearBeads class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDLINEARBEADS_H__7EB16E72_4DC5_40F3_85CA_C81367A62F97__INCLUDED_)
#define AFX_PMSENDLINEARBEADS_H__7EB16E72_4DC5_40F3_85CA_C81367A62F97__INCLUDED_


#include "mpmMessage.h"

class pmSendLinearBeads : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendLinearBeads();
	
	pmSendLinearBeads(const pmSendLinearBeads& oldMessage);

	virtual ~pmSendLinearBeads();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendLinearBeads& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendLinearBeads& message);

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
	// so that they may have data read into them given only an pmSendLinearBeads pointer,

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
	bool ReplaceBeadByGhostInPolymer();
	bool IsBeadFirstOrLastInPolymer() const;

    // Function to store the bead coordinates in the message

	void SetMessageData(BeadList lBeads);


    // Accessor functions to the message data

	bool CreateEmptyBeads();
	bool CreateEmptyPolymers();
	BeadList GetRecBeads();
	PolymerList GetRecPolymers();

    inline long GetSentBeadTotal()  const {return m_lBeads.size();}
    inline long GetRecBeadTotal()   const {return m_RecTotal;}
    inline long GetEmptyBeadTotal() const {return m_lEmptyBeads.size();}

    inline long GetVisibleFlags()  const {return m_VisibleFlags;}

	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetMessageType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Function to calculate the bead order for a given polymer.

	long GetBeadOrderFromPolymer(CPolymer* const pPolymer) const;

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

    BeadList    m_lBeads;          // Set of beads being sent to neighbouring processor
    BeadList    m_lRecBeads;       // Set of beads received from neighbouring processor
    BeadList    m_lEmptyBeads;     // Set of empty bead instances to receive incoming transfers

    PolymerList m_lRecPolymers;    // Set of polymers filled up from incoming beads
    PolymerList m_lEmptyPolymers;  // Set of empty polymer instances to receive incoming transfers

    long     m_VisibleFlags; // Integer containing boolean flags for bead visibility
    
    // Arrays used to hold bead data before inserting into message buffer

    long     m_Ids[m_MaxMsgBeads];
	long     m_DisplayIds[m_MaxMsgBeads];
	long     m_Visibility[m_MaxMsgBeads];
    long     m_Types[m_MaxMsgBeads];
	long     m_PolymerIds[m_MaxMsgBeads];   // Id of owning polymer
	long     m_PolymerTypes[m_MaxMsgBeads]; // Type of owning polymer
	long     m_BeadOrder[m_MaxMsgBeads];    // 1 if this is the first or only bead in the polymer to move, >1 otherwise
    double   m_Radius[m_MaxMsgBeads];       // Hardwired to 0.5 for now

    double   m_X[m_MaxMsgBeads];
    double   m_Y[m_MaxMsgBeads];
    double   m_Z[m_MaxMsgBeads];
    double   m_VX[m_MaxMsgBeads];
    double   m_VY[m_MaxMsgBeads];
    double   m_VZ[m_MaxMsgBeads];

    double   m_OldVX[m_MaxMsgBeads];
    double   m_OldVY[m_MaxMsgBeads];
    double   m_OldVZ[m_MaxMsgBeads];
    double   m_OldFX[m_MaxMsgBeads];
    double   m_OldFY[m_MaxMsgBeads];
    double   m_OldFZ[m_MaxMsgBeads];

    double   m_UnPBCX[m_MaxMsgBeads];
    double   m_UnPBCY[m_MaxMsgBeads];
    double   m_UnPBCZ[m_MaxMsgBeads];

    double   m_InitialX[m_MaxMsgBeads];
    double   m_InitialY[m_MaxMsgBeads];
    double   m_InitialZ[m_MaxMsgBeads];

    double   m_dX[m_MaxMsgBeads];
    double   m_dY[m_MaxMsgBeads];
    double   m_dZ[m_MaxMsgBeads];


    // ********************
    // Data used when receiving the message

    long m_RecTotal;  // Number of beads whose coordinates are received

	CAbstractBead* m_pBead;     // Bead pointer for use in filling empty bead instances
    CPolymer*      m_pPolymer;  // Polymer pointer for use in filling empty polymers
};

#endif // !defined(AFX_PMSENDLINEARBEADS_H__7EB16E72_4DC5_40F3_85CA_C81367A62F97__INCLUDED_)
