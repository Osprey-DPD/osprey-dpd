// pmSetSamplePeriod.h: interface for the pmSetSamplePeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETSAMPLEPERIOD_H__03287EA5_5874_4690_967E_A54CC7EF2047__INCLUDED_)
#define AFX_PMSETSAMPLEPERIOD_H__03287EA5_5874_4690_967E_A54CC7EF2047__INCLUDED_


// Forward declarations

class mcSetSamplePeriod;


#include "mpmCommandWrapper.h"

class pmSetSamplePeriod : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetSamplePeriod();
	
	pmSetSamplePeriod(const pmSetSamplePeriod& oldMessage);

	virtual ~pmSetSamplePeriod();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetSamplePeriod& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetSamplePeriod& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetSamplePeriod pointer,

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

	long m_Period;	// No of timesteps between sampling observables for analysis
};

#endif // !defined(AFX_PMSETSAMPLEPERIOD_H__03287EA5_5874_4690_967E_A54CC7EF2047__INCLUDED_)
