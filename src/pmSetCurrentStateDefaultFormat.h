// pmSetCurrentStateDefaultFormat.h: interface for the pmSetCurrentStateDefaultFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETCURRENTSTATEDEFAULTFORMAT_H__4808BF7C_4210_468B_A1FE_9EF994DE6992__INCLUDED_)
#define AFX_PMSETCURRENTSTATEDEFAULTFORMAT_H__4808BF7C_4210_468B_A1FE_9EF994DE6992__INCLUDED_


// Forward declarations

class mcToggleBeadDisplay;


#include "mpmCommandWrapper.h"

class pmSetCurrentStateDefaultFormat : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetCurrentStateDefaultFormat();
	
	pmSetCurrentStateDefaultFormat(const pmSetCurrentStateDefaultFormat& oldMessage);

	virtual ~pmSetCurrentStateDefaultFormat();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetCurrentStateDefaultFormat& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetCurrentStateDefaultFormat& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetCurrentStateDefaultFormat pointer,

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

    inline const zString GetFormat()  const {return m_Format;}

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

    zString m_Format;  // String identifier for the current state format
};

#endif // !defined(AFX_PMSETCURRENTSTATEDEFAULTFORMAT_H__4808BF7C_4210_468B_A1FE_9EF994DE6992__INCLUDED_)
