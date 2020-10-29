// pmSimBoxAssembly.h: interface for the pmSimBoxAssembly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSIMBOXASSEMBLY_H__F7535406_7416_425B_A558_7BE80E0A05DB__INCLUDED_)
#define AFX_PMSIMBOXASSEMBLY_H__F7535406_7416_425B_A558_7BE80E0A05DB__INCLUDED_


#include "mpmMessage.h"

class pmSimBoxAssembly : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSimBoxAssembly();
	
	pmSimBoxAssembly(const pmSimBoxAssembly& oldMessage);

	virtual ~pmSimBoxAssembly();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSimBoxAssembly& message);
	friend zInStream&  operator>>(zInStream& is,        pmSimBoxAssembly& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSimBoxAssembly pointer,

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
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Accessor functions to the message data for extraction by the input data object

    inline bool GetAssemblyFailed() const {return m_bFailed;}
	
	void SetAssemblyFailed(bool bFailed);

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

    bool m_bFailed;       // Flag showing if the SimBox assembly failed or not
};

#endif // !defined(AFX_PMSIMBOXASSEMBLY_H__F7535406_7416_425B_A558_7BE80E0A05DB__INCLUDED_)
