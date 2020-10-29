// pmSaveParaviewCurrentState.h: interface for the pmSaveParaviewCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSAVEPARAVIEWCURRENTSTATE_H__959839A9_A88D_444F_BDDF_D7198139071F__INCLUDED_)
#define AFX_PMSAVEPARAVIEWCURRENTSTATE_H__959839A9_A88D_444F_BDDF_D7198139071F__INCLUDED_


// Forward declarations

class mcSaveParaviewCurrentState;


#include "mpmCommandWrapper.h"

class pmSaveParaviewCurrentState : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSaveParaviewCurrentState();
	
	pmSaveParaviewCurrentState(const pmSaveParaviewCurrentState& oldMessage);

	virtual ~pmSaveParaviewCurrentState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSaveParaviewCurrentState& message);
	friend zInStream&  operator>>(zInStream& is,        pmSaveParaviewCurrentState& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSaveParaviewCurrentState pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void SendAllP();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to copy the data from the xxCommand-derived object to the message

	void SetMessageData(const xxCommand* const pCommand);

    // Accessor functions to the message data - no data for this command


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

	long m_Value;  // Redundant message payload
};

#endif // !defined(AFX_PMSAVEPARAVIEWCURRENTSTATE_H__959839A9_A88D_444F_BDDF_D7198139071F__INCLUDED_)
