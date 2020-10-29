// pmProcessorBeadAngularMomentum.h: interface for the pmProcessorBeadAngularMomentum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPROCESSORBEADANGULARMOMENTUM_H__056377BE_2963_4113_A15E_D21B8F4BF18D__INCLUDED_)
#define AFX_PMPROCESSORBEADANGULARMOMENTUM_H__056377BE_2963_4113_A15E_D21B8F4BF18D__INCLUDED_


#include "mpmMessage.h"

class pmProcessorBeadAngularMomentum : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmProcessorBeadAngularMomentum();
	
	pmProcessorBeadAngularMomentum(const pmProcessorBeadAngularMomentum& oldMessage);

	virtual ~pmProcessorBeadAngularMomentum();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmProcessorBeadAngularMomentum& message);
	friend zInStream&  operator>>(zInStream& is,        pmProcessorBeadAngularMomentum& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmProcessorBeadAngularMomentum pointer,

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

	void SetMessageData(long beadTotal, double xrv, double yrv, double zrv);

    // Accessor functions to the message data for extraction by P0.

	long GetBeadTotal(long pn) const;
	double GetXAngMom(long pn) const;
	double GetYAngMom(long pn) const;
	double GetZAngMom(long pn) const;

	// Functions to return the data accumulated for all processors

	long GetWorldBeadTotal() const;
	double GetWorldXAngMom() const;
	double GetWorldYAngMom() const;
	double GetWorldZAngMom() const;

	void ZeroAngMom();

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
	double  m_XAngMom;
	double  m_YAngMom;
	double  m_ZAngMom;


	// Local storage for P0 to receive data from all PN

    zLongVector    m_vBeadTotal;    // Total number of beads in each processor's Space

	zDoubleVector  m_vXAngMom;
	zDoubleVector  m_vYAngMom;       // Angular momentum components for each PN
	zDoubleVector  m_vZAngMom;

};

#endif // !defined(AFX_PMPROCESSORBEADANGULARMOMENTUM_H__056377BE_2963_4113_A15E_D21B8F4BF18D__INCLUDED_)
