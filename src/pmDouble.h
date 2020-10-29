// pmDouble.h: interface for the pmDouble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMDOUBLE_H__D53B6D74_58F8_4F4F_BEE9_809118491E6F__INCLUDED_)
#define AFX_PMDOUBLE_H__D53B6D74_58F8_4F4F_BEE9_809118491E6F__INCLUDED_


#include "mpmMessage.h"

class pmDouble : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmDouble();
	
	pmDouble(const pmDouble& oldMessage);

	virtual ~pmDouble();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmDouble& message);
	friend zInStream&  operator>>(zInStream& is,        pmDouble& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmDouble pointer,

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

	void SetMessageData(double value);

    // Accessor functions to the message data

    inline double GetValue() const {return m_Value;}

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

    double  m_Value;
};

#endif // !defined(AFX_PMDOUBLE_H__D53B6D74_58F8_4F4F_BEE9_809118491E6F__INCLUDED_)
