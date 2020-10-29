// pmSimBoxNeighbourCall.h: interface for the pmSimBoxNeighbourCall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSIMBOXNEIGHBOURCALL_H__59F9FC4D_54C8_4986_B41E_5B32782C3F46__INCLUDED_)
#define AFX_PMSIMBOXNEIGHBOURCALL_H__59F9FC4D_54C8_4986_B41E_5B32782C3F46__INCLUDED_


#include "mpmMessage.h"

class pmSimBoxNeighbourCall : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSimBoxNeighbourCall();
	
	pmSimBoxNeighbourCall(const pmSimBoxNeighbourCall& oldMessage);

	virtual ~pmSimBoxNeighbourCall();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSimBoxNeighbourCall& message);
	friend zInStream&  operator>>(zInStream& is,        pmSimBoxNeighbourCall& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSimBoxNeighbourCall pointer,

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

    // Accessor functions to the message data for extraction by the input data object

    inline long GetSenderId() const {return m_SenderId;}
	long        GetNNProcId(long i) const;
	
	void        SetSenderId(long pid);
	void        SetNNProcId(long i, long pid);

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

    long m_SenderId;      // Sending processor's id (should be the rank of P0)
    long m_NNProcId[27];  // Processor ids for processor PN id plus its nearest neighbours

};


#endif // !defined(AFX_PMSIMBOXNEIGHBOURCALL_H__59F9FC4D_54C8_4986_B41E_5B32782C3F46__INCLUDED_)
