// pmExtendedPolymerProcessorCount.h: interface for the pmExtendedPolymerProcessorCount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMEXTENDEDPOLYMERPROCESSORCOUNT_H__96C512B0_FA96_4701_B168_12FF6341DB0E__INCLUDED_)
#define AFX_PMEXTENDEDPOLYMERPROCESSORCOUNT_H__96C512B0_FA96_4701_B168_12FF6341DB0E__INCLUDED_


#include "mpmMessage.h"

class pmExtendedPolymerProcessorCount : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmExtendedPolymerProcessorCount();
	
	pmExtendedPolymerProcessorCount(const pmExtendedPolymerProcessorCount& oldMessage);

	virtual ~pmExtendedPolymerProcessorCount();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmExtendedPolymerProcessorCount& message);
	friend zInStream&  operator>>(zInStream& is,        pmExtendedPolymerProcessorCount& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmExtendedPolymerProcessorCount pointer,

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

    // Function to store the extended polymer ids in the message

	void SetMessageData(LongExtendedPolymerMap& mExtPolymers);

    // Accessor functions to the message data

    inline long        GetPolymerTotal() const {return m_vPolymerIds.size();}
    inline zLongVector GetPolymerIds()   const {return m_vPolymerIds;}

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

    zLongVector  m_vPolymerIds;      // Ids of extended polymers owned by the sending processor
};

#endif // !defined(AFX_PMEXTENDEDPOLYMERPROCESSORCOUNT_H__96C512B0_FA96_4701_B168_12FF6341DB0E__INCLUDED_)
