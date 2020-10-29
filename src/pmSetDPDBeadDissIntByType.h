// pmSetDPDBeadDissIntByType.h: interface for the pmSetDPDBeadDissIntByType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETDPDBEADDISSINTBYTYPE_H__DF8096C7_0499_40F9_B3F2_CD1525941771__INCLUDED_)
#define AFX_PMSETDPDBEADDISSINTBYTYPE_H__DF8096C7_0499_40F9_B3F2_CD1525941771__INCLUDED_


// Forward declarations

class ccSetDPDBeadDissIntByType;



#include "mpmCommandWrapper.h"

class pmSetDPDBeadDissIntByType : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetDPDBeadDissIntByType();
	
	pmSetDPDBeadDissIntByType(const pmSetDPDBeadDissIntByType& oldMessage);

	virtual ~pmSetDPDBeadDissIntByType();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetDPDBeadDissIntByType& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetDPDBeadDissIntByType& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetDPDBeadDissIntByType pointer,

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

	inline long   GetFirstType()  const {return m_FirstType;}
	inline long   GetSecondType() const {return m_SecondType;}
	inline double GetDissInt()	  const {return m_DissInt;}

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

	long    m_FirstType;   // Numeric identifier for first bead type
	long    m_SecondType;  // Numeric identifier for second bead type
	double  m_DissInt;	   // New value for dissipative interaction parameter
};

#endif // !defined(AFX_PMSETDPDBEADDISSINTBYTYPE_H__DF8096C7_0499_40F9_B3F2_CD1525941771__INCLUDED_)
