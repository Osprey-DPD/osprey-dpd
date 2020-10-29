// pmDouble3d.h: interface for the pmDouble3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMDOUBLE3D_H__9862ABA5_1CAC_414D_9056_3B63DC666ED5__INCLUDED_)
#define AFX_PMDOUBLE3D_H__9862ABA5_1CAC_414D_9056_3B63DC666ED5__INCLUDED_


#include "mpmMessage.h"

class pmDouble3d : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmDouble3d();
	
	pmDouble3d(const pmDouble3d& oldMessage);

	virtual ~pmDouble3d();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmDouble3d& message);
	friend zInStream&  operator>>(zInStream& is,        pmDouble3d& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmDouble3d pointer,

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

    // Function to set the single data value for the message

	void SetMessageData(double x, double y, double z);

    // Accessor functions to the message data

    inline double GetXValue() const {return m_Value[0];}
    inline double GetYValue() const {return m_Value[1];}
    inline double GetZValue() const {return m_Value[2];}

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

    double  m_Value[3];
};

#endif // !defined(AFX_PMDOUBLE3D_H__9862ABA5_1CAC_414D_9056_3B63DC666ED5__INCLUDED_)
