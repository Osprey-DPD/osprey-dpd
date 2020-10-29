// pmCreateNanoparticleSphere.h: interface for the pmCreateNanoparticleSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMCREATENANOPARTICLESPHERE_H__5ba21859_8096_4ffa_a720_b79f595f2048__INCLUDED_)
#define AFX_PMCREATENANOPARTICLESPHERE_H__5ba21859_8096_4ffa_a720_b79f595f2048__INCLUDED_


// Forward declarations

class ccSetTimeStepSize;


#include "mpmCommandWrapper.h"

class pmCreateNanoparticleSphere : public mpmCommandWrapper  
{
	// ****************************************
	// Construction/Destruction: 
public:

	pmCreateNanoparticleSphere();
		
    // Constructor for use when creating the command internally
    
	pmCreateNanoparticleSphere(const zString polyName, long procId, long colourId, 
                               double xc, double yc, double  zc, double innerRadius, double outerRadius,
                               long maxBonds, double range, double fraction, double springConstant, double unstretchedLength, 
                               long numberTypes, zLongVector vBeadTypes, zDoubleVector vConsInt);
		
	pmCreateNanoparticleSphere(const pmCreateNanoparticleSphere& oldMessage);

	virtual ~pmCreateNanoparticleSphere();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const pmCreateNanoparticleSphere& message);
	friend zInStream&  operator>>(zInStream& is,        pmCreateNanoparticleSphere& message);

	static const zString GetType();	// Return the type of message

private:

	static const zString m_Type;	// Identifier for the message

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an pmCreateNanoparticleSphere pointer,

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
    
	inline double GetXCentre()                    const {return m_XC;}
	inline double GetYCentre()                    const {return m_YC;}
	inline double GetZCentre()                    const {return m_ZC;}
	inline double GetInnerRadius()                const {return m_InnerRadius;}
	inline double GetOuterRadius()                const {return m_OuterRadius;}
    
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
    // Following arguments define the shape of the nanoparticle - here it is a sphere
	double		m_XC;
	double		m_YC;		            // Sphere centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_InnerRadius;	        // Inner radius of sphere	(units of bead diameter)
	double		m_OuterRadius;	        // Outer radius of sphere	(units of bead diameter)
    
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

#endif // !defined(AFX_PMCREATENANOPARTICLESPHERE_H__5ba21859_8096_4ffa_a720_b79f595f2048__INCLUDED_)
