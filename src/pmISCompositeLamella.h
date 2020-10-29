// pmISCompositeLamella.h: interface for the pmISCompositeLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMISCOMPOSITELAMELLA_H__76B55F52_C040_41F8_B939_955F0982EB37__INCLUDED_)
#define AFX_PMISCOMPOSITELAMELLA_H__76B55F52_C040_41F8_B939_955F0982EB37__INCLUDED_


// Forward declarations

class CInitialStateData;


#include "mpmInitialState.h"

class pmISCompositeLamella : public mpmInitialState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmISCompositeLamella();
	
	pmISCompositeLamella(const pmISCompositeLamella& oldMessage);

	virtual ~pmISCompositeLamella();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmISCompositeLamella& message);
	friend zInStream&  operator>>(zInStream& is,        pmISCompositeLamella& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmISCompositeLamella pointer,

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

    inline  StringSequence GetPolymers()           const {return m_Polymers;}
    inline  zDoubleVector  GetUpperFractions()     const {return m_UpperFractions;}
    inline  long           GetXNormal()            const {return m_X;}
    inline  long           GetYNormal()            const {return m_Y;}
    inline  long           GetZNormal()            const {return m_Z;}
    inline  double         GetCentre()             const {return m_Centre;}
    inline  double         GetThickness()          const {return m_Thickness;}
    inline  bool           GetLinearise()          const {return m_bLinearise;}
	inline  bool           GetUpperPatchesFlag()   const {return m_bPatches[0];}
	inline  bool           GetLowerPatchesFlag()   const {return m_bPatches[1];}


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

	StringSequence    m_Polymers;               // Polymer composing bilayer
	zDoubleVector	  m_UpperFractions;			// Fraction of each polymer in upper monolayer
	long	          m_X;						
	long	          m_Y;						// Bilayer normal: only x, y or z allowed
	long	          m_Z;
	double	          m_Centre;					// Bilayer centre as fraction of SimBox thickness
	double	          m_Thickness;				// In units of bead diameters
	bool	          m_bLinearise;				// Flag showing whether polymer is initially straightened
	bool			  m_bPatches[2];			// Arrange polymers in patches: 0/1

    // Local data used by MPI

    long    m_LongLinearise;            // long representation of a boolean
    long    m_LongUpperPatches;            
    long    m_LongLowerPatches;            
};

#endif // !defined(AFX_PMISCOMPOSITELAMELLA_H__76B55F52_C040_41F8_B939_955F0982EB37__INCLUDED_)
