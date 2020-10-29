// pmProcessorBeadVelocity.h: interface for the pmProcessorBeadVelocity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPROCESSORBEADVELOCITY_H__64A79839_B4C8_4CD3_AF31_DC039ACFDC89__INCLUDED_)
#define AFX_PMPROCESSORBEADVELOCITY_H__64A79839_B4C8_4CD3_AF31_DC039ACFDC89__INCLUDED_


#include "mpmMessage.h"

class pmProcessorBeadVelocity : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmProcessorBeadVelocity();
	
	pmProcessorBeadVelocity(const pmProcessorBeadVelocity& oldMessage);

	virtual ~pmProcessorBeadVelocity();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmProcessorBeadVelocity& message);
	friend zInStream&  operator>>(zInStream& is,        pmProcessorBeadVelocity& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmProcessorBeadVelocity pointer,

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
	// the data are the number of beads and the total bead velocity 
	// for the sending processor.

	void SetMessageData(long beadTotal, double xv, double yv, double zv);

    // Accessor functions to the message data for extraction by P0.

	long GetBeadTotal(long pn)   const;
	double GetXVel(long pn)    const;
	double GetYVel(long pn)    const;
	double GetZVel(long pn)    const;
	
	// Function returning the total for all processors (including P0)

	long GetWorldBeadTotal() const;
	double GetWorldXVel() const;
	double GetWorldYVel() const;
	double GetWorldZVel() const;

	void ZeroVel();

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
	double  m_XVel;
	double  m_YVel;
	double  m_ZVel;


	// Local storage for P0 to receive data from all PN

    zLongVector    m_vBeadTotal;    // Total number of beads in each processor's Space

	zDoubleVector  m_vXVelTotal;
	zDoubleVector  m_vYVelTotal;       // Total bead velocity components for all PN
	zDoubleVector  m_vZVelTotal;

};

#endif // !defined(AFX_PMPROCESSORBEADVELOCITY_H__64A79839_B4C8_4CD3_AF31_DC039ACFDC89__INCLUDED_)
