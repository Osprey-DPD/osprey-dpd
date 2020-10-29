// pmPolymerData.h: interface for the pmPolymerData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMPOLYMERDATA_H__A3851FB2_5DD1_45AF_9724_D82D492FAB0D__INCLUDED_)
#define AFX_PMPOLYMERDATA_H__A3851FB2_5DD1_45AF_9724_D82D492FAB0D__INCLUDED_


// Forward declarations

class CInputData;


#include "mpmMessage.h"

class pmPolymerData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmPolymerData();
	
	pmPolymerData(const pmPolymerData& oldMessage);

	virtual ~pmPolymerData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmPolymerData& message);
	friend zInStream&  operator>>(zInStream& is,        pmPolymerData& message);

	static const zString GetType();	// Return the type of message

    static long GetTypeTotal();     // Return the number of types created

private:

	static const zString m_Type;	// Identifier for the message

    static long  m_TypeTotal;       // Number of types created so far

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmPolymerData pointer,

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

    inline  const zString GetName()        const {return m_Name;}
    inline  const zString GetShape()       const {return m_Shape;}
    inline  const zString GetHeadName()    const {return m_HeadName;}
    inline  const zString GetTailName()    const {return m_TailName;}
    inline  double        GetFraction()    const {return m_Fraction;}

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

    zString m_Name;          // Polymer string identifier
    zString m_Shape;         // Polymer shape string
    zString m_HeadName;      // Head bead string identifier
    zString m_TailName;      // Tail bead string identifier
    long    m_PolymerType;   // Numeric type
    double  m_Fraction;      // Number fraction of this polymer type

};


#endif // !defined(AFX_PMPOLYMERDATA_H__A3851FB2_5DD1_45AF_9724_D82D492FAB0D__INCLUDED_)
