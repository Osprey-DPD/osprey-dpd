// pmSetCurrentStateLight.h: interface for the pmSetCurrentStateLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETCURRENTSTATELIGHT_H__2e9c5f98_71db_4130_8c3b_f660bc21d1b9__INCLUDED_)
#define AFX_PMSETCURRENTSTATELIGHT_H__2e9c5f98_71db_4130_8c3b_f660bc21d1b9__INCLUDED_


// Forward declarations

class mcSetCurrentStateCamera;


#include "mpmCommandWrapper.h"

class pmSetCurrentStateLight : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetCurrentStateLight();
	
	pmSetCurrentStateLight(const pmSetCurrentStateLight& oldMessage);

	virtual ~pmSetCurrentStateLight();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetCurrentStateLight& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetCurrentStateLight& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetCurrentStateLight pointer,

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

	inline double	GetLight(short int i) const {return m_Light[i];}

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

	double	m_Light[3];			// Light coordinates as multiple of SimBox size
};

#endif // !defined(AFX_PMSETCURRENTSTATELIGHT_H__2e9c5f98_71db_4130_8c3b_f660bc21d1b9__INCLUDED_)
