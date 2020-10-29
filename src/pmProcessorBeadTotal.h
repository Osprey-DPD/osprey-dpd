// pmProcessorBeadTotal.h: interface for the pmProcessorBeadTotal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPROCESSORBEADTOTAL_H__B74F42EA_9CB5_4769_95A1_5E52B37DAD4C__INCLUDED_)
#define AFX_PMPROCESSORBEADTOTAL_H__B74F42EA_9CB5_4769_95A1_5E52B37DAD4C__INCLUDED_


#include "mpmMessage.h"

class pmProcessorBeadTotal : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmProcessorBeadTotal();
	
	pmProcessorBeadTotal(const pmProcessorBeadTotal& oldMessage);

	virtual ~pmProcessorBeadTotal();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmProcessorBeadTotal& message);
	friend zInStream&  operator>>(zInStream& is,        pmProcessorBeadTotal& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmProcessorBeadTotal pointer,

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

    // Function to set the single data value

	void SetMessageData(long beadTotal);

    // Accessor functions to the message data for extraction by the input data object

    long GetBeadTotal(long pn) const;
	
	// Function to return the data accumulated for all processors

	long GetWorldBeadTotal() const;

	void ZeroBeadTotal();

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

    long  m_BeadTotal;       // Number of beads in each processor's Space

	zLongVector m_vBeadTotal;  // Total beads in the processor World
};

#endif // !defined(AFX_PMPROCESSORBEADTOTAL_H__B74F42EA_9CB5_4769_95A1_5E52B37DAD4C__INCLUDED_)
