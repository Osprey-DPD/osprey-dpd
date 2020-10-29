// pmCommandNames.h: interface for the pmCommandNames class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCOMMANDNAMES_H__3583F836_699C_4BF3_830D_B9E141AC4EDF__INCLUDED_)
#define AFX_PMCOMMANDNAMES_H__3583F836_699C_4BF3_830D_B9E141AC4EDF__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmCommandNames : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmCommandNames();
	
	pmCommandNames(const pmCommandNames& oldMessage);

	virtual ~pmCommandNames();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmCommandNames& message);
	friend zInStream&  operator>>(zInStream& is,        pmCommandNames& message);

	static const zString GetType();	// Return the type of message

    static long GetTypeTotal();     // Return the number of types created

private:

	static const zString m_Type;	// Identifier for the message

    static long  m_TypeTotal;      // Number of types created so far

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmCommandNames pointer,

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

    // Function to copy the data from the input data object to the message instance

	void SetMessageData(const CInputData* const pData);

    // Accessor functions to the message data

    inline  StringSequence GetCommandNames()  const {return m_Names;}


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

    StringSequence m_Names;       // Set of command names scheduled for execution
};

#endif // !defined(AFX_PMCOMMANDNAMES_H__3583F836_699C_4BF3_830D_B9E141AC4EDF__INCLUDED_)
