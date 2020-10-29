// pmSendMembraneAreaMoleculeTotals.h: interface for the pmSendMembraneAreaMoleculeTotals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSENDMEMBRANEAREAMOLECULETOTALS_H__e5a98641_0a3a_4f85_bd2e_7248fc9c6263__INCLUDED_)
#define AFX_PMSENDMEMBRANEAREAMOLECULETOTALS_H__e5a98641_0a3a_4f85_bd2e_7248fc9c6263__INCLUDED_


#include "mpmMessage.h"

class pmSendMembraneAreaMoleculeTotals : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmSendMembraneAreaMoleculeTotals();
	
	pmSendMembraneAreaMoleculeTotals(const pmSendMembraneAreaMoleculeTotals& oldMessage);

	virtual ~pmSendMembraneAreaMoleculeTotals();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmSendMembraneAreaMoleculeTotals& message);
	friend zInStream&  operator>>(zInStream& is,        pmSendMembraneAreaMoleculeTotals& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmSendMembraneAreaMoleculeTotals pointer,

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const mpmMessage* GetMessage()  const;

    // ********************
    // Messaging interface that defines PVFs that must be overridden by derived classes,
    // and VFs that are overridden  when the derived class wants to call the 
    // corresponding MPI function.

	virtual void BuildDerivedType(MsgDataTypePtr pMsgDataType);

    virtual bool Validate();

    virtual void Send(long procId);
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to set the single data value for the message

	void SetMessageData(double area, double moleculeTotal);

    // Accessor functions to the message data

    inline double GetArea()          const {return m_Value[0];}
    inline double GetMoleculeTotal() const {return m_Value[1];}

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

    double  m_Value[2];         // Membrane area and molecule total in one processor space
};

#endif // !defined(AFX_PMSENDMEMBRANEAREAMOLECULETOTALS_H__e5a98641_0a3a_4f85_bd2e_7248fc9c6263__INCLUDED_)
