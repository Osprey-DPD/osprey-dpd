// pmBondData.h: interface for the pmBondData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMBONDDATA_H__ADD22B3D_45D1_4CB9_BBA0_2BD0961CEF08__INCLUDED_)
#define AFX_PMBONDDATA_H__ADD22B3D_45D1_4CB9_BBA0_2BD0961CEF08__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmBondData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmBondData();
	
	pmBondData(const pmBondData& oldMessage);

	virtual ~pmBondData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmBondData& message);
	friend zInStream&  operator>>(zInStream& is,        pmBondData& message);

	static const zString GetType();	// Return the type of message

    static long GetTypeTotal();     // Return the number of types created

private:

	static const zString m_Type;	// Identifier for the message

    static long  m_TypeTotal;       // Number of types created so far

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmBondData pointer,

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

    // Function to copy the data from the input data object to the message instance

	void SetMessageData(const CInputData* const pData);

    // Accessor functions to the message data for extraction by the input data object

    inline  const zString GetHeadName()    const {return m_HeadName;}
    inline  const zString GetTailName()    const {return m_TailName;}
    inline  long          GetBondType()    const {return m_BondType;}
    inline  double        GetSpringConst() const {return m_SprConst;}
    inline  double        GetLength()      const {return m_UnStrLen;}

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

    zString m_HeadName;      // Head bead string identifier
    zString m_TailName;      // Tail bead string identifier
    long    m_BondType;      // Numeric type
    double  m_SprConst;      // Hookean spring constant
    double  m_UnStrLen;      // Unstretched length

};

#endif // !defined(AFX_PMBONDDATA_H__ADD22B3D_45D1_4CB9_BBA0_2BD0961CEF08__INCLUDED_)
