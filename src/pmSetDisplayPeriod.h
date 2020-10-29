// pmSetDisplayPeriod.h: interface for the pmSetDisplayPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETDISPLAYPERIOD_H__E5BC4237_1BEA_4423_B75E_A74FFC5DE855__INCLUDED_)
#define AFX_PMSETDISPLAYPERIOD_H__E5BC4237_1BEA_4423_B75E_A74FFC5DE855__INCLUDED_


// Forward declarations

class mcSetDisplayPeriod;


#include "mpmCommandWrapper.h"

class pmSetDisplayPeriod : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetDisplayPeriod();
	
	pmSetDisplayPeriod(const pmSetDisplayPeriod& oldMessage);

	virtual ~pmSetDisplayPeriod();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetDisplayPeriod& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetDisplayPeriod& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetDisplayPeriod pointer,

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

    // Accessor functions to the message data

	inline long GetPeriod()	const {return m_Period;}

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

	long m_Period;	// No of timesteps between saving current states
};

#endif // !defined(AFX_PMSETDISPLAYPERIOD_H__E5BC4237_1BEA_4423_B75E_A74FFC5DE855__INCLUDED_)
