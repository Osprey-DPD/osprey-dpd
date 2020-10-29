// pmString.h: interface for the pmString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSTRING_H__BFB2C7D7_CC4F_4D90_860F_EAA2D1D2060E__INCLUDED_)
#define AFX_PMSTRING_H__BFB2C7D7_CC4F_4D90_860F_EAA2D1D2060E__INCLUDED_


#include "mpmMessage.h"

class pmString : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmString();
	
	pmString(const pmString& oldMessage);

	virtual ~pmString();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmString& message);
	friend zInStream&  operator>>(zInStream& is,        pmString& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmString pointer,

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

    // Function to set the single string value

	void SetMessageData(const zString value);

    // Accessor functions to the message data for extraction by the input data object

    inline  const zString GetValue()  const {return m_Value;}


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

    zString m_Value;    // String identifier


};

#endif // !defined(AFX_PMSTRING_H__BFB2C7D7_CC4F_4D90_860F_EAA2D1D2060E__INCLUDED_)
