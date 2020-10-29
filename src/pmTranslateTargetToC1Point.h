// pmTranslateTargetToC1Point.h: interface for the pmTranslateTargetToC1Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMTRANSLATETARGETTOC1POINT_H__ed3fed71_5c09_4a62_ab07_b1d7838ca0b9__INCLUDED_)
#define AFX_PMTRANSLATETARGETTOC1POINT_H__ed3fed71_5c09_4a62_ab07_b1d7838ca0b9__INCLUDED_


// Forward declarations

class ccSetTimeStepSize;


#include "mpmCommandWrapper.h"

class pmTranslateTargetToC1Point : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmTranslateTargetToC1Point();
		
	pmTranslateTargetToC1Point(const pmTranslateTargetToC1Point& oldMessage);

	virtual ~pmTranslateTargetToC1Point();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmTranslateTargetToC1Point& message);
	friend zInStream&  operator>>(zInStream& is,        pmTranslateTargetToC1Point& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmTranslateTargetToC1Point pointer,

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

    inline const zString GetTargetLabel() const {return m_Label;}
	inline double GetXCentre()            const {return m_Centre[0];}
	inline double GetYCentre()            const {return m_Centre[1];}
	inline double GetZCentre()            const {return m_Centre[2];}

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

	zString m_Label;       // Name of target to translate
	double	m_Centre[3];   // Fractional distance of point along SimBox in X,Y,Z direction (0 to 1 exclusive)
};

#endif // !defined(AFX_PMTRANSLATETARGETTOC1POINT_H__ed3fed71_5c09_4a62_ab07_b1d7838ca0b9__INCLUDED_)
