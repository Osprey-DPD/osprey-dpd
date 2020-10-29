// pmToggleBeadDisplay.h: interface for the pmToggleBeadDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMTOGGLEBEADDISPLAY_H__E3C2AAA0_4F24_40E7_B484_CC9D0D4F77A1__INCLUDED_)
#define AFX_PMTOGGLEBEADDISPLAY_H__E3C2AAA0_4F24_40E7_B484_CC9D0D4F77A1__INCLUDED_


// Forward declarations

class mcToggleBeadDisplay;


#include "mpmCommandWrapper.h"

class pmToggleBeadDisplay : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmToggleBeadDisplay();
	
	pmToggleBeadDisplay(const pmToggleBeadDisplay& oldMessage);

	virtual ~pmToggleBeadDisplay();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmToggleBeadDisplay& message);
	friend zInStream&  operator>>(zInStream& is,        pmToggleBeadDisplay& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmToggleBeadDisplay pointer,

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

    inline const zString GetBeadName()  const {return m_Name;}

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

    zString m_Name;  // String identifier for bead type
};

#endif // !defined(AFX_PMTOGGLEBEADDISPLAY_H__E3C2AAA0_4F24_40E7_B484_CC9D0D4F77A1__INCLUDED_)
