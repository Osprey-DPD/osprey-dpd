// pmSaveAmiraCurrentState.h: interface for the pmSaveAmiraCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSAVEAMIRACURRENTSTATE_H__9827067c_8f5d_4a2e_a3a1_5fbf7ca5db2e__INCLUDED_)
#define AFX_PMSAVEAMIRACURRENTSTATE_H__9827067c_8f5d_4a2e_a3a1_5fbf7ca5db2e__INCLUDED_


// Forward declarations

class mcSaveAmiraCurrentState;


#include "mpmCommandWrapper.h"

class pmSaveAmiraCurrentState : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSaveAmiraCurrentState();
	
	pmSaveAmiraCurrentState(const pmSaveAmiraCurrentState& oldMessage);

	virtual ~pmSaveAmiraCurrentState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSaveAmiraCurrentState& message);
	friend zInStream&  operator>>(zInStream& is,        pmSaveAmiraCurrentState& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSaveAmiraCurrentState pointer,

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

    // No data as the message's existence is sufficient.
	
};


#endif // !defined(AFX_PMSAVEAMIRACURRENTSTATE_H__9827067c-8f5d-4a2e-a3a1-5fbf7ca5db2e__INCLUDED_)
