// pmISVesicle.h: interface for the pmISVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMISVESICLE_H__96E25645_513C_485F_90B8_AAE7A3A8F3F3__INCLUDED_)
#define AFX_PMISVESICLE_H__96E25645_513C_485F_90B8_AAE7A3A8F3F3__INCLUDED_


// Forward declarations

class CInitialStateData;


#include "mpmInitialState.h"

class pmISVesicle : public mpmInitialState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmISVesicle();
	
	pmISVesicle(const pmISVesicle& oldMessage);

	virtual ~pmISVesicle();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmISVesicle& message);
	friend zInStream&  operator>>(zInStream& is,        pmISVesicle& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmISVesicle pointer,

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

    inline const zString GetPolymer()          const {return m_Polymers.at(0);}
    inline const zString GetInteriorPolymer()  const {return m_InteriorPolymers.at(0);}

    inline  double       GetXCentre()        const {return m_XC;}
    inline  double       GetYCentre()        const {return m_YC;}
    inline  double       GetZCentre()        const {return m_ZC;}
    inline  double       GetOuterRadius()    const {return m_OuterRadius;}
    inline  double       GetThickness()      const {return m_Thickness;}
    inline  double       GetOuterFraction()  const {return m_OuterFractions.at(0);}
	inline  bool         GetOuterPatches()   const {return m_bPatches[0];}
	inline  bool         GetInnerPatches()   const {return m_bPatches[1];}


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

	StringSequence  m_Polymers;			// Polymer composing bilayer
	StringSequence	m_InteriorPolymers;	// Polymers in interior of vesicle
	double	        m_XC;						
	double	        m_YC;				// Vesicle centre as fraction of SimBox thickness
	double	        m_ZC;
	double	        m_OuterRadius;		// Vesicle outer monolayer head radius in units of bead diameter
	double	        m_Thickness;	    // Vesicle bilayer thickness in units of bead diameter
	zDoubleVector	m_OuterFractions;	// Fractions of polymers in outer monolayer
	bool			m_bPatches[2];		// Arrange polymers in patches: 0/1

};

#endif // !defined(AFX_PMISVESICLE_H__96E25645_513C_485F_90B8_AAE7A3A8F3F3__INCLUDED_)
