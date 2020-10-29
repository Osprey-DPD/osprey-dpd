// pmSetDensityPeriod.h: interface for the pmSetDensityPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETDENSITYPERIOD_H__95D13693_3120_4AF3_B47C_F832020D168D__INCLUDED_)
#define AFX_PMSETDENSITYPERIOD_H__95D13693_3120_4AF3_B47C_F832020D168D__INCLUDED_


// Forward declarations

class mcSetDensityPeriod;


#include "mpmCommandWrapper.h"

class pmSetDensityPeriod : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetDensityPeriod();
	
	pmSetDensityPeriod(const pmSetDensityPeriod& oldMessage);

	virtual ~pmSetDensityPeriod();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetDensityPeriod& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetDensityPeriod& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetDensityPeriod pointer,

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

	inline long GetPeriod()	const {return m_Period;}

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

	long m_Period;	// No of timesteps between saving density states
};

#endif // !defined(AFX_PMSETDENSITYPERIOD_H__95D13693_3120_4AF3_B47C_F832020D168D__INCLUDED_)
