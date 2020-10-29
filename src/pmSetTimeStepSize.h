// pmSetTimeStepSize.h: interface for the pmSetTimeStepSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETTIMESTEPSIZE_H__520DC6F1_E0CD_4872_A89E_4FBD37E13B9A__INCLUDED_)
#define AFX_PMSETTIMESTEPSIZE_H__520DC6F1_E0CD_4872_A89E_4FBD37E13B9A__INCLUDED_


// Forward declarations

class ccSetTimeStepSize;


#include "mpmCommandWrapper.h"

class pmSetTimeStepSize : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetTimeStepSize();
	
	pmSetTimeStepSize(const pmSetTimeStepSize& oldMessage);

	virtual ~pmSetTimeStepSize();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetTimeStepSize& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetTimeStepSize& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetTimeStepSize pointer,

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

	inline double GetTimeStep()	const {return m_dt;}

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

	double m_dt;	// New integration step size to be used
};

#endif // !defined(AFX_PMSETTIMESTEPSIZE_H__520DC6F1_E0CD_4872_A89E_4FBD37E13B9A__INCLUDED_)
