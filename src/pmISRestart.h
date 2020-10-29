// pmISRestart.h: interface for the pmISRestart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMISRESTART_H__64B8EE4B_747C_42B0_951C_A11BBB2097DE__INCLUDED_)
#define AFX_PMISRESTART_H__64B8EE4B_747C_42B0_951C_A11BBB2097DE__INCLUDED_


// Forward declarations

class CInitialStateData;


#include "mpmMessage.h"

class pmISRestart : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmISRestart();
	
	pmISRestart(const pmISRestart& oldMessage);

	virtual ~pmISRestart();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmISRestart& message);
	friend zInStream&  operator>>(zInStream& is,        pmISRestart& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmISRestart pointer,

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

    // Function to copy the data from the initial state object to the message instance

	void SetMessageData(const CInitialStateData* const pISD);

    // Accessor functions to the message data for extraction by the input data object

    inline const zString GetRunId()   const {return m_RunId;}
    inline long          GetStateId() const {return m_StateId;}

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

    zString m_RunId;    // RunId for the run to be restarted
    long    m_StateId;  // Timestep at which the restart state was saved
};

#endif // !defined(AFX_PMISRESTART_H__64B8EE4B_747C_42B0_951C_A11BBB2097DE__INCLUDED_)
