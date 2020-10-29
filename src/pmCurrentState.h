// pmCurrentState.h: interface for the pmCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCURRENTSTATE_H__AC807235_1FC2_4881_A2C8_D9B8F814DBC8__INCLUDED_)
#define AFX_PMCURRENTSTATE_H__AC807235_1FC2_4881_A2C8_D9B8F814DBC8__INCLUDED_


// Constant that specifies the maximum number of beads whose coordinates can 
// be passed in one message instance.

#define  pmCurrentStateMAXBEADS  1000000

// Forward declarations

class CCurrentState;
class CBead;


#include "mpmMessage.h"

class pmCurrentState : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmCurrentState();
	
	pmCurrentState(const pmCurrentState& oldMessage);

	virtual ~pmCurrentState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmCurrentState& message);
	friend zInStream&  operator>>(zInStream& is,        pmCurrentState& message);

	static const zString GetType();	       // Return the type of message

private:

	static const zString m_Type;	       // Identifier for the message


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmCurrentState pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void SendP0();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to pass a pointer to the current state instance
    // so that P0 can copy bead data to its

    void SetCurrentStatePointer(CCurrentState* const pcState);

    // Accessor functions to the message data

	void AddBead(const CAbstractBead* const pBead);

	// Beads with no radius parameter

	void AddBead(long id, long type, double x, double y, double z);
	void GetBead(long* pid, long* pType, double* pX, double* pY, double* pZ);

	// Beads with radius parameter
	void AddBead(long id, long type, double radius, double x, double y, double z);
	void GetBead(long* pid, long* pType, double* pRadius, double* pX, double* pY, double* pZ);

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

    CCurrentState*  m_pcState;  // CurrentState instance used by P0
    long m_Counter;             // Internal counter showing how many beads are stored

    long m_Id[pmCurrentStateMAXBEADS];        // Set of bead display ids - sets bead colour
    long m_BeadType[pmCurrentStateMAXBEADS];  // Set of bead types

    double m_Radius[pmCurrentStateMAXBEADS];  // Hardwired to 0.5 for now
    double m_X[pmCurrentStateMAXBEADS];
    double m_Y[pmCurrentStateMAXBEADS];       // Set of bead position coordinates
    double m_Z[pmCurrentStateMAXBEADS];


};

#endif // !defined(AFX_PMCURRENTSTATE_H__AC807235_1FC2_4881_A2C8_D9B8F814DBC8__INCLUDED_)
