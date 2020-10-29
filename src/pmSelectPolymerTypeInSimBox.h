// pmSelectPolymerTypeInSimBox.h: interface for the pmSelectPolymerTypeInSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSELECTPOLYMERTYPEINSIMBOX_H__c3bed93e_993c_4cfe_a401_f973dbe58c73__INCLUDED_)
#define AFX_PMSELECTPOLYMERTYPEINSIMBOX_H__c3bed93e_993c_4cfe_a401_f973dbe58c73__INCLUDED_


// Forward declarations

class ccSelectPolymerTypeInSimBox;


#include "mpmCommandWrapper.h"

class pmSelectPolymerTypeInSimBox : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSelectPolymerTypeInSimBox();
	
	pmSelectPolymerTypeInSimBox(const pmSelectPolymerTypeInSimBox& oldMessage);

	virtual ~pmSelectPolymerTypeInSimBox();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSelectPolymerTypeInSimBox& message);
	friend zInStream&  operator>>(zInStream& is,        pmSelectPolymerTypeInSimBox& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSelectPolymerTypeInSimBox pointer,

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

    inline const zString GetTargetLabel()  const {return m_TargetLabel;}
    inline const zString GetPolymerName()  const {return m_PolymerName;}

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

	zString		m_TargetLabel;	// Command target's name
	zString		m_PolymerName; 	// Polymer name

};

#endif // !defined(AFX_PMSELECTPOLYMERTYPEINSIMBOX_H__c3bed93e_993c_4cfe_a401_f973dbe58c73__INCLUDED_)
