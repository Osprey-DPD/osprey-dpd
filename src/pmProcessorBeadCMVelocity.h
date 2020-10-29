// pmProcessorBeadCMVelocity.h: interface for the pmProcessorBeadCMVelocity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPROCESSORBEADCMVELOCITY_H__73BF649A_1F3C_4292_B627_37C6F0A772C8__INCLUDED_)
#define AFX_PMPROCESSORBEADCMVELOCITY_H__73BF649A_1F3C_4292_B627_37C6F0A772C8__INCLUDED_


#include "mpmMessage.h"

class pmProcessorBeadCMVelocity : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmProcessorBeadCMVelocity();
	
	pmProcessorBeadCMVelocity(const pmProcessorBeadCMVelocity& oldMessage);

	virtual ~pmProcessorBeadCMVelocity();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmProcessorBeadCMVelocity& message);
	friend zInStream&  operator>>(zInStream& is,        pmProcessorBeadCMVelocity& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmProcessorBeadCMVelocity pointer,

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
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function for PN to set the message data prior to sending it to P0: 
	// the bead total and CM velocity for a processor.

	void SetMessageData(long beadTotal, double xvcm, double yvcm, double zvcm);

    // Accessor functions to the message data for extraction by P0.

	long GetBeadTotal(long pn)   const;
	double GetXVel(long pn)    const;
	double GetYVel(long pn)    const;
	double GetZVel(long pn)    const;
	
	// Function returning the total for all processors (including P0)

	long GetWorldBeadTotal() const;
	double GetWorldXCMVel() const;
	double GetWorldYCMVel() const;
	double GetWorldZCMVel() const;

	void ZeroCMVel();

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

	// Local storage for each PN to send its data to P0

	long    m_BeadTotal;
	double  m_XCMVel;
	double  m_YCMVel;
	double  m_ZCMVel;


	// Local storage for P0 to receive data from all PN

    zLongVector    m_vBeadTotal;    // Total number of beads in each processor's Space

	zDoubleVector  m_vXCMVel;
	zDoubleVector  m_vYCMVel;       // Bead CM velocity components for all PN
	zDoubleVector  m_vZCMVel;

};

#endif // !defined(AFX_PMPROCESSORBEADCMVELOCITY_H__73BF649A_1F3C_4292_B627_37C6F0A772C8__INCLUDED_)
