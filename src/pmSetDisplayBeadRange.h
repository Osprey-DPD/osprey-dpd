// pmSetDisplayBeadRange.h: interface for the pmSetDisplayBeadRange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSETDISPLAYBEADRANGE_H__84CD9731_274F_4D89_A26F_137B1756E8E5__INCLUDED_)
#define AFX_PMSETDISPLAYBEADRANGE_H__84CD9731_274F_4D89_A26F_137B1756E8E5__INCLUDED_


// Forward declarations

class mcSetDisplayBeadRange;


#include "mpmCommandWrapper.h"

class pmSetDisplayBeadRange : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSetDisplayBeadRange();
	
	pmSetDisplayBeadRange(const pmSetDisplayBeadRange& oldMessage);

	virtual ~pmSetDisplayBeadRange();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSetDisplayBeadRange& message);
	friend zInStream&  operator>>(zInStream& is,        pmSetDisplayBeadRange& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSetDisplayBeadRange pointer,

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

	const zString GetAxis() const;
	long GetAxisNo(const zString axis) const;
	inline double GetMinFraction()	const {return m_MinFraction;}
	inline double GetMaxFraction()	const {return m_MaxFraction;}

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

	const long  m_X;
	const long  m_Y;        // Numeric identifiers for the axes
	const long  m_Z;

	long    m_AxisNo;       // Axis no
	double  m_MinFraction;	// Minimum coordinate as fraction of SimBox size
	double  m_MaxFraction;	// Maximum     "             "        "
};

#endif // !defined(AFX_PMSETDISPLAYBEADRANGE_H__84CD9731_274F_4D89_A26F_137B1756E8E5__INCLUDED_)
