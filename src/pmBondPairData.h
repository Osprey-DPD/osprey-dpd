// pmBondPairData.h: interface for the pmBondPairData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMBONDPAIRDATA_H__452DDBBA_3B88_4B30_A4DC_D8A33FD42230__INCLUDED_)
#define AFX_PMBONDPAIRDATA_H__452DDBBA_3B88_4B30_A4DC_D8A33FD42230__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmBondPairData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmBondPairData();
	
	pmBondPairData(const pmBondPairData& oldMessage);

	virtual ~pmBondPairData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmBondPairData& message);
	friend zInStream&  operator>>(zInStream& is,        pmBondPairData& message);

	static const zString GetType();	// Return the type of message

    static long GetTypeTotal();     // Return the number of types created

private:

	static const zString m_Type;	// Identifier for the message

    static long  m_TypeTotal;       // Number of types created so far

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmBondPairData pointer,

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

    inline  const zString GetFirstName()   const {return m_Name1;}
    inline  const zString GetSecondName()  const {return m_Name2;}
    inline  const zString GetThirdName()   const {return m_Name3;}
    inline  double        GetModulus()     const {return m_Modulus;}
    inline  double        GetPhi0()        const {return m_Phi0;}

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

    zString m_Name1;        // First bead string identifier
    zString m_Name2;        // Second bead string identifier
    zString m_Name3;        // Third bead string identifier
    long    m_BondPairType; // Numeric type
    double  m_Modulus;      // Bending modulus of stiff bond
    double  m_Phi0;         // Preferred angle of stiff bond

};


#endif // !defined(AFX_PMBONDPAIRDATA_H__452DDBBA_3B88_4B30_A4DC_D8A33FD42230__INCLUDED_)
