// pmCreateTypesData.h: interface for the pmCreateTypesData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCREATETYPESDATA_H__597324C3_944D_421C_911D_8B6D6DD5E880__INCLUDED_)
#define AFX_PMCREATETYPESDATA_H__597324C3_944D_421C_911D_8B6D6DD5E880__INCLUDED_




#include "mpmMessage.h"

class pmCreateTypesData : public mpmMessage  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmCreateTypesData();
	
	pmCreateTypesData(const pmCreateTypesData& oldMessage);

	virtual ~pmCreateTypesData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmCreateTypesData& message);
	friend zInStream&  operator>>(zInStream& is,        pmCreateTypesData& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmCreateTypesData pointer,

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
    virtual void SendP0();
    virtual void SendAllP();
    virtual void Receive();
    virtual void Receive(long procId);

	// ****************************************
	// Public access functions
public:

    // Function to copy the data from the input data object to the message instance

	void SetMessageData(long beadTotal, long bondTotal, long bondPairTotal, long polymerTotal,
                        zLongVector vPolymerTypeTotals);

    // Accessor functions to the message data

    inline long GetBeadTotal()         const {return m_BeadTotal;}
    inline long GetBondTotal()         const {return m_BondTotal;}
    inline long GetBondPairTotal()     const {return m_BondPairTotal;}
    inline long GetPolymerTotal()      const {return m_PolymerTotal;}
    inline long GetPolymerTypeTotal()  const {return m_vPolymerTypeTotals.size();}

    zLongVector GetPolymerTypeTotals() const {return m_vPolymerTypeTotals;}


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

    long        m_BeadTotal;           // Total number of beads on P0
    long        m_BondTotal;           // Total number of bonds on P0
    long        m_BondPairTotal;       // Total number of bondpairs on P0
    long        m_PolymerTotal;        // Total number of polymers on P0

    zLongVector  m_vPolymerTypeTotals;   // Number of each type of polymer to be created

    long         m_Array[1000];          // We allow up to 1000 types
};

#endif // !defined(AFX_PMCREATETYPESDATA_H__597324C3_944D_421C_911D_8B6D6DD5E880__INCLUDED_)
