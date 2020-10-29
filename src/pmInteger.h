// pmInteger.h: interface for the pmInteger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMINTEGER_H__113F8013_8A86_41E5_BF04_92CA70AF7744__INCLUDED_)
#define AFX_PMINTEGER_H__113F8013_8A86_41E5_BF04_92CA70AF7744__INCLUDED_


#include "mpmMessage.h"

class pmInteger : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmInteger();
	
	pmInteger(const pmInteger& oldMessage);

	virtual ~pmInteger();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmInteger& message);
	friend zInStream&  operator>>(zInStream& is,        pmInteger& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmInteger pointer,

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
    virtual void SendAllP();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to set the single data value for the message

	void SetMessageData(long value);

    // Accessor functions to the message data

    inline long GetValue() const {return m_Value;}

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

    long  m_Value;
};

#endif // !defined(AFX_PMINTEGER_H__113F8013_8A86_41E5_BF04_92CA70AF7744__INCLUDED_)
