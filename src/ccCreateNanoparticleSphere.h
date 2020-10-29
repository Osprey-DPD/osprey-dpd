// ccCreateNanoparticleSphere.h: interface for the ccCreateNanoparticleSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCREATENANOPARTICLESPHERE_H__a7e8c3d5_38a6_4a92_800c_b875ae9ae6fa__INCLUDED_)
#define AFX_CCCREATENANOPARTICLESPHERE_H__a7e8c3d5_38a6_4a92_800c_b875ae9ae6fa__INCLUDED_


#include "xxCommand.h"

class ccCreateNanoparticleSphere : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccCreateNanoparticleSphere(long executionTime);
	ccCreateNanoparticleSphere(const ccCreateNanoparticleSphere& oldCommand);

	ccCreateNanoparticleSphere(long executionTime, 
							   const zString poly, long procId, long colourId,
							   double xc, double yc, double zc,
							   double inRadius, double outRadius,
                               long maxBonds, double range, double fraction, double springConstant, double unstretchedLength, long NumberTypes, zLongVector vBeadTypes, zDoubleVector vConsInt);

	virtual ~ccCreateNanoparticleSphere();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;


	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymerName() const {return m_PolymerName;}
	inline long          GetProcessorId() const {return m_ProcId;}
	inline long          GetColourId()    const {return m_ColourId;}
    
	inline double	GetXCentre()		  const {return m_XC;}
	inline double	GetYCentre()	 	  const {return m_YC;}
	inline double	GetZCentre()		  const {return m_ZC;}
	inline double	GetInnerRadius()	  const {return m_InnerRadius;}
	inline double	GetOuterRadius()	  const {return m_OuterRadius;}
    
	inline long     GetMaxBondsPerPolymer()   const {return m_MaxBonds;}
	inline double   GetRange()			      const {return m_Range;}
	inline double   GetBondFraction()		  const {return m_Fraction;}
	inline double   GetSpringConstant()	      const {return m_SpringConstant;}
	inline double   GetUnstretchedLength()    const {return m_UnstretchedLength;}
	inline long     GetNumberTypes()		  const {return m_NumberTypes;}
	inline zLongVector   GetBeadTypes()	      const {return m_vBeadTypes;}
	inline zDoubleVector   GetConsInt()      const {return m_vConsInt;}


	// ****************************************
	// Protected local functions
protected:

	virtual const zString GetCommandType() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

    zString     m_PolymerName;  // Polymer type that composes the nanoparticle
    long        m_ProcId;       // Processor that will create the nanoparticle
    long        m_ColourId;     // Colour of nanoparticle (see CurrentStateFormat.cpp for colour sequence)
    // Following arguments define the shape of the nanoparticle - here it is a sphere
	double		m_XC;
	double		m_YC;		    // Sphere centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_InnerRadius;	// Inner radius of sphere	(units of bead diameter)
	double		m_OuterRadius;	// Outer radius of sphere	(units of bead diameter)
    
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

#endif // !defined(AFX_CCCREATENANOPARTICLESPHERE_H__a7e8c3d5_38a6_4a92_800c_b875ae9ae6fa__INCLUDED_)
