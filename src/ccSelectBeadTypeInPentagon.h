// ccSelectBeadTypeInPentagon.h: interface for the ccSelectBeadTypeInPentagon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINPENTAGON_H__99750EE1_2204_4F71_AA9B_BE9DCA9B084F__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINPENTAGON_H__99750EE1_2204_4F71_AA9B_BE9DCA9B084F__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInPentagon : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInPentagon(long executionTime);

    ccSelectBeadTypeInPentagon(long executionTime, const zString target, const zString bead, 
							   double xc, double yc, double zc, 
                               double br, double side, double thickness,
                               double theta, double phi);

	ccSelectBeadTypeInPentagon(const ccSelectBeadTypeInPentagon& oldCommand);

	virtual ~ccSelectBeadTypeInPentagon();
	
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

	inline double	GetXCentre()			 const {return m_XC;}
	inline double	GetYCentre()			 const {return m_YC;}
	inline double	GetZCentre()			 const {return m_ZC;}
	inline double	GetBoundingRadius()		 const {return m_BoundingRadius;}
	inline double	GetSideLength()		     const {return m_Side;}
	inline double	GetThickness()	         const {return m_Thickness;}
	inline double	GetPolarAngle()          const {return m_Theta;}
	inline double	GetAzimuthalAngle()      const {return m_Phi;}

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
	double		m_YC;	// Pentagon's centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_BoundingRadius; // Radius of sphere bounding the pentagon (units of bead diameter)
	double		m_Side;	          // Side length of pentagon (units of bead diameter)
	double      m_Thickness;      // Thickness of pentagon (units of bead diameter)
	double      m_Theta;          // Polar angle of pentagon's normal (degrees)
	double      m_Phi;            // Azimuthal angle of pentagon's normal (degrees)
	
};

#endif // !defined(AFX_CCSELECTBEADTYPEINPENTAGON_H__99750EE1_2204_4F71_AA9B_BE9DCA9B084F__INCLUDED_)
