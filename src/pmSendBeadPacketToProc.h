// pmSendBeadPacketToProc.h: interface for the pmSendBeadPacketToProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDBEADPACKETTOPROC_H__B2C704C4_6332_4A78_9EE4_CC8F5578C695__INCLUDED_)
#define AFX_PMSENDBEADPACKETTOPROC_H__B2C704C4_6332_4A78_9EE4_CC8F5578C695__INCLUDED_


#include "mpmMessage.h"

class pmSendBeadPacketToProc : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendBeadPacketToProc();
	
	pmSendBeadPacketToProc(const pmSendBeadPacketToProc& oldMessage);

	virtual ~pmSendBeadPacketToProc();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendBeadPacketToProc& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendBeadPacketToProc& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendBeadPacketToProc pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void Send(long procId);
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Accessor functions to the message data for extraction by the input data object

    inline long GetSenderId() const {return m_SenderId;}
	
	void        SetSenderId(long pid);

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

};

#endif // !defined(AFX_PMSENDBEADPACKETTOPROC_H__B2C704C4_6332_4A78_9EE4_CC8F5578C695__INCLUDED_)
