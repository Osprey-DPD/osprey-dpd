// pmISRandom.h: interface for the pmISRandom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMISRANDOM_H__0C148E53_FA05_47AB_A62B_36964E3FE6AD__INCLUDED_)
#define AFX_PMISRANDOM_H__0C148E53_FA05_47AB_A62B_36964E3FE6AD__INCLUDED_



// Forward declarations

class CInitialStateData;


#include "mpmInitialState.h"

class pmISRandom : public mpmInitialState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmISRandom();
	
	pmISRandom(const pmISRandom& oldMessage);

	virtual ~pmISRandom();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmISRandom& message);
	friend zInStream&  operator>>(zInStream& is,        pmISRandom& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmISRandom pointer,

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

	virtual void SetMessageData(const CInitialStateData* const pISD);


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


};

#endif // !defined(AFX_PMISRANDOM_H__0C148E53_FA05_47AB_A62B_36964E3FE6AD__INCLUDED_)
