// pmISLamella.h: interface for the pmISLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMISLAMELLA_H__8BC8842D_0FC3_4F07_BF73_3449437A32C4__INCLUDED_)
#define AFX_PMISLAMELLA_H__8BC8842D_0FC3_4F07_BF73_3449437A32C4__INCLUDED_


// Forward declarations

class CInitialStateData;


#include "mpmInitialState.h"

class pmISLamella : public mpmInitialState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmISLamella();
	
	pmISLamella(const pmISLamella& oldMessage);

	virtual ~pmISLamella();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmISLamella& message);
	friend zInStream&  operator>>(zInStream& is,        pmISLamella& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmISLamella pointer,

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

    // Function to copy the data from the initial state object to the message instance

	void SetMessageData(const CInitialStateData* const pISD);

    // Accessor functions to the message data

    inline const zString GetPolymer()        const {return m_Polymer;}

    inline  bool         GetLinearise()      const {return m_bLinearise;}
    inline  long         GetXNormal()        const {return m_X;}
    inline  long         GetYNormal()        const {return m_Y;}
    inline  long         GetZNormal()        const {return m_Z;}
    inline  double       GetCentre()         const {return m_Centre;}
    inline  double       GetThickness()      const {return m_Thickness;}
    inline  double       GetUpperFraction()  const {return m_UpperFraction;}


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

	zString m_Polymer;					// Polymer composing bilayer
	bool	m_bLinearise;				// Flag showing whether polymer is initially straightened
	long	m_X;						
	long	m_Y;						// Bilayer normal: only x, y or z allowed
	long	m_Z;
	double	m_Centre;					// Bilayer centre as fraction of SimBox thickness
	double	m_Thickness;				// In units of bead diameters
	double	m_UpperFraction;			// Fraction of polymer in upper monolayer

    // Local data used by MPI

    long    m_LongLinearise;            // long representation of a boolean

};

#endif // !defined(AFX_PMISLAMELLA_H__8BC8842D_0FC3_4F07_BF73_3449437A32C4__INCLUDED_)
