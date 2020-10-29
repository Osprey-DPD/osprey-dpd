// ccSelectPolymerTypeHeadInEllipsoid.h: interface for the ccSelectPolymerTypeHeadInEllipsoid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOID_H__3B484A38_4907_4623_A92D_24276F86C67A__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOID_H__3B484A38_4907_4623_A92D_24276F86C67A__INCLUDED_


#include "ccSelectPolymerTypeInRegion.h"

class ccSelectPolymerTypeHeadInEllipsoid : public ccSelectPolymerTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectPolymerTypeHeadInEllipsoid(long executionTime);
	ccSelectPolymerTypeHeadInEllipsoid(const ccSelectPolymerTypeHeadInEllipsoid& oldCommand);

	ccSelectPolymerTypeHeadInEllipsoid(long executionTime, const zString target, const zString poly,
							   double xc, double yc, double zc, 
							   double boundingRadius, double semiMajorAxis,
							   double firstSemiMinorAxis, double secondSemiMinorAxis,
							   double theta, double phi);

	virtual ~ccSelectPolymerTypeHeadInEllipsoid();
	
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

    // VF to fill up an empty command instace with values for all of its parameters.

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline double	GetXCentre()			 const {return m_XC;}
	inline double	GetYCentre()			 const {return m_YC;}
	inline double	GetZCentre()			 const {return m_ZC;}
	inline double	GetBoundingRadius()		 const {return m_BoundingRadius;}
	inline double	GetSemiMajorAxis()		 const {return m_SMA;}
	inline double	GetFirstSemiMinorAxis()	 const {return m_SMB;}
	inline double	GetSecondSemiMinorAxis() const {return m_SMC;}
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
	double		m_YC;	   // Ellipsoid's centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_BoundingRadius;	// Radius of sphere bounding the ellipsoid	(units of bead diameter)
	double		m_SMA;	   // Semi-major axis of ellipsoid (must be largest dimension)
	double		m_SMB;	   // First semi-minor axis of ellipsoid (next largest dimension)
	double		m_SMC;	   // Second semi-minor axis of ellipsoid (smallest dimension)
	double      m_Theta;   // Polar angle of ellipse's SMA (degrees)
	double      m_Phi;     // Azimuthal angle of ellipse's SMA (degrees)


};

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINELLIPSOID_H__3B484A38_4907_4623_A92D_24276F86C67A__INCLUDED_)
