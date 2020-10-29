// pmSimBoxNeighbourResponse.h: interface for the pmSimBoxNeighbourResponse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSIMBOXNEIGHBOURRESPONSE_H__8FAD6614_8458_488B_96CB_C99993F85EDF__INCLUDED_)
#define AFX_PMSIMBOXNEIGHBOURRESPONSE_H__8FAD6614_8458_488B_96CB_C99993F85EDF__INCLUDED_


#include "mpmMessage.h"

class pmSimBoxNeighbourResponse : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSimBoxNeighbourResponse();
	
	pmSimBoxNeighbourResponse(const pmSimBoxNeighbourResponse& oldMessage);

	virtual ~pmSimBoxNeighbourResponse();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSimBoxNeighbourResponse& message);
	friend zInStream&  operator>>(zInStream& is,        pmSimBoxNeighbourResponse& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSimBoxNeighbourResponse pointer,

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

    // Function to copy the data from the input data object to the message instance

	void SetMessageData(long senderId);

    // Accessor functions to the message data for extraction by the input data object


    inline long GetSenderId() const {return m_SenderId;}
	long        GetNNProcId(long i) const;

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

    long m_SenderId;      // Sending processor's id (should be the rank of PN)
    long m_NNProcId[27];  // Processor ids for nearest neighbours of PN

};

#endif // !defined(AFX_PMSIMBOXNEIGHBOURRESPONSE_H__8FAD6614_8458_488B_96CB_C99993F85EDF__INCLUDED_)
