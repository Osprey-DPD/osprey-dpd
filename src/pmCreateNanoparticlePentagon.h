// pmCreateNanoparticlePentagon.h: interface for the pmCreateNanoparticlePentagon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCREATENANOPARTICLEPENTAGON_H__b7aab2dd_843e_467e_9ac1_589ad63e18c0__INCLUDED_)
#define AFX_PMCREATENANOPARTICLEPENTAGON_H__b7aab2dd_843e_467e_9ac1_589ad63e18c0__INCLUDED_


// Forward declarations

class ccSetTimeStepSize;


#include "mpmCommandWrapper.h"

class pmCreateNanoparticlePentagon : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmCreateNanoparticlePentagon();
		
    // Constructor for use when creating the command internally
    
	pmCreateNanoparticlePentagon(const zString polyName, long procId, long colourId, 
                                 double xc, double yc, double zc, 
                                 double br, double side, double thickness,
                                 double theta, double phi,
                                 long maxBonds, double range, double fraction, double springConstant, double unstretchedLength, 
                                 long numberTypes, zLongVector vBeadTypes, zDoubleVector vConsInt);
		
	pmCreateNanoparticlePentagon(const pmCreateNanoparticlePentagon& oldMessage);

	virtual ~pmCreateNanoparticlePentagon();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmCreateNanoparticlePentagon& message);
	friend zInStream&  operator>>(zInStream& is,        pmCreateNanoparticlePentagon& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmCreateNanoparticlePentagon pointer,

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

    // Function to copy the data from the xxCommand-derived object to the message

	void SetMessageData(const xxCommand* const pCommand);

    // Accessor functions to the message data

    inline const zString GetPolymerName()         const {return m_PolymerName;}
    inline long          GetTargetProcessorId()   const {return m_TargetProcId;}
    inline long          GetColourId()            const {return m_ColourId;}
    
	inline double	GetXCentre()			      const {return m_XC;}
	inline double	GetYCentre()			      const {return m_YC;}
	inline double	GetZCentre()			      const {return m_ZC;}
	inline double	GetBoundingRadius()		      const {return m_BoundingRadius;}
	inline double	GetSideLength()		          const {return m_Side;}
	inline double	GetThickness()	              const {return m_Thickness;}
	inline double	GetPolarAngle()               const {return m_Theta;}
	inline double	GetAzimuthalAngle()           const {return m_Phi;}
    
    inline long   GetMaxBondsPerPolymer()         const {return m_MaxBonds;}
	inline double GetRange()                      const {return m_Range;}
	inline double GetBondFraction()               const {return m_Fraction;}
	inline double GetSpringConstant()	          const {return m_SpringConstant;}
	inline double GetUnstretchedLength()          const {return m_UnstretchedLength;}
    inline long   GetNumberTypes()                const {return m_NumberTypes;}
    inline zLongVector    GetBeadTypes()          const {return m_vBeadTypes;}
    inline zDoubleVector  GetConsIt()             const {return m_vConsInt;}

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

    zString     m_PolymerName;          // Polymer type that composes the nanoparticle
    long        m_TargetProcId;         // Processor that will create the nanoparticle (-1 means all)
    long        m_ColourId;             // Colour of nanoparticle (see CurrentStateFormat.cpp for colour sequence)
    
    // Following arguments define the shape of the nanoparticle - here it is a pentagon
	double		m_XC;
	double		m_YC;	// Pentagon's centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_BoundingRadius; // Radius of sphere bounding the pentagon (units of bead diameter)
	double		m_Side;	          // Side length of pentagon (units of bead diameter)
	double      m_Thickness;      // Thickness of pentagon (units of bead diameter)
	double      m_Theta;          // Polar angle of pentagon's normal (degrees)
	double      m_Phi;            // Azimuthal angle of pentagon's normal (degrees)
    
    // Following arguments define the springs used to bind the polymers into the nanoparticle
	long		m_MaxBonds;				// Max bonds per polymer
	double		m_Range;				// Max separation within which to create bonds
	double		m_Fraction;				// Fraction of neighbours to be bound
	double		m_SpringConstant;		// Hookean parameters for bonds
	double		m_UnstretchedLength;
    
    // Following arguments define the DPD interaction parameters of the nanoparticle with other bead types
    long m_NumberTypes;
    zLongVector m_vBeadTypes;
    zDoubleVector m_vConsInt;

};

#endif // !defined(AFX_PMCREATENANOPARTICLEPENTAGON_H__b7aab2dd_843e_467e_9ac1_589ad63e18c0__INCLUDED_)
