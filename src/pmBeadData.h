// pmBeadData.h: interface for the pmBeadData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMBEADDATA_H__6868FACF_5D21_48AA_A8F2_792A422968E3__INCLUDED_)
#define AFX_PMBEADDATA_H__6868FACF_5D21_48AA_A8F2_792A422968E3__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmBeadData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmBeadData();
	
	pmBeadData(const pmBeadData& oldMessage);

	virtual ~pmBeadData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmBeadData& message);
	friend zInStream&  operator>>(zInStream& is,        pmBeadData& message);

	static const zString GetType();	// Return the type of message

    static long GetTypeTotal();     // Return the number of types created

private:

	static const zString m_Type;	// Identifier for the message

    static long  m_TypeTotal;      // Number of types created so far

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmBeadData pointer,

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

    inline  const zString GetBeadName()  const {return m_BeadName;}
    inline  long          GetBeadType()  const {return m_BeadType;}
    inline  double        GetRadius()    const {return m_BeadRadius;}
    inline  double        GetLGRadius()  const {return m_LGRadius;}

    inline zDoubleVector  GetConsInt()   const {return m_vConsInt;}
    inline zDoubleVector  GetDissInt()   const {return m_vDissInt;}
    inline zDoubleVector  GetLGInt()     const {return m_vLGInt;}


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

    zString m_BeadName;        // String identifier
    long    m_BeadType;        // Numeric type
    double  m_BeadRadius;      // DPD interaction cut-off
    double  m_LGRadius;        // LG DPD cut-off - only for LG DPD simulations

    zDoubleVector m_vConsInt;  // Conservative interactions with self and previous types
    zDoubleVector m_vDissInt;  // Dissipative       "
    zDoubleVector m_vLGInt;    // Only for LG DPD simulations


};

#endif // !defined(AFX_PMBEADDATA_H__6868FACF_5D21_48AA_A8F2_792A422968E3__INCLUDED_)
