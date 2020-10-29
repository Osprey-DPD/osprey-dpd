// ccSelectBeadTypeInSphericalCap.h: interface for the ccSelectBeadTypeInSphericalCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSPHERICALCAP_H__a71cdb8d_8d4a_4e51_8ec4_e1709247f97a__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSPHERICALCAP_H__a71cdb8d_8d4a_4e51_8ec4_e1709247f97a__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInSphericalCap : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInSphericalCap(long executionTime);
	ccSelectBeadTypeInSphericalCap(long executionTime, const zString target, const zString bead,
	                               double xc, double yc, double zc,  double inRadius, double outRadius, 
								   double theta, double phi, double gamma);
	
	ccSelectBeadTypeInSphericalCap(const ccSelectBeadTypeInSphericalCap& oldCommand);

	virtual ~ccSelectBeadTypeInSphericalCap();
	
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

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline double	GetXCentre()         const {return m_XC;}
	inline double	GetYCentre()         const {return m_YC;}
	inline double	GetZCentre()         const {return m_ZC;}
	inline double	GetInnerRadius()     const {return m_InnerRadius;}
	inline double	GetOuterRadius()     const {return m_OuterRadius;}
	inline double	GetPolarAngle()      const {return m_Theta;}
	inline double	GetAzimuthalAngle()  const {return m_Phi;}
	inline double	GetHalfAngle()       const {return m_Gamma;}
    


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

	double		m_XC;
	double		m_YC;		    // Sphere's centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_InnerRadius;	// Inner radius of spherical shell	(units of bead diameter)
	double		m_OuterRadius;	// Outer radius of spherical shell	(units of bead diameter)
	double		m_Theta;	    // Polar angle defining normal to spherical cap
	double		m_Phi;	        // Azimuthal angle defining normal to spherical cap
	double		m_Gamma;	    // Half angle defining size of spherical cap
};

#endif // !defined(AFX_CCSELECTBEADTYPEINSPHERICALCAP_H__a71cdb8d_8d4a_4e51_8ec4_e1709247f97a__INCLUDED_)
