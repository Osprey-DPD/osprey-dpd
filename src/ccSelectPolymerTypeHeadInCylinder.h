// ccSelectPolymerTypeHeadInCylinder.h: interface for the ccSelectPolymerTypeHeadInCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTPOLYMERTYPEHEADINCYLINDER_H__75B54AB2_8A9A_4190_A3AC_076C6E4507C9__INCLUDED_)
#define AFX_CCSELECTPOLYMERTYPEHEADINCYLINDER_H__75B54AB2_8A9A_4190_A3AC_076C6E4507C9__INCLUDED_


#include "ccSelectPolymerTypeInRegion.h"

class ccSelectPolymerTypeHeadInCylinder : public ccSelectPolymerTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectPolymerTypeHeadInCylinder(long executionTime);
	ccSelectPolymerTypeHeadInCylinder(const ccSelectPolymerTypeHeadInCylinder& oldCommand);

	ccSelectPolymerTypeHeadInCylinder(long executionTime, const zString target, const zString poly,
							   long xn, long yn, long zn, double xc, double yc, double zc, 
							   double halfLength, double inRadius, double outRadius);

	virtual ~ccSelectPolymerTypeHeadInCylinder();
	
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

	inline long		GetXNormal()		const {return m_X;}
	inline long		GetYNormal()		const {return m_Y;}
	inline long		GetZNormal()		const {return m_Z;}
	inline double	GetXCentre()		const {return m_XC;}
	inline double	GetYCentre()		const {return m_YC;}
	inline double	GetZCentre()		const {return m_ZC;}
	inline double	GetHalfLength()		const {return m_HalfLength;}
	inline double	GetInnerRadius()	const {return m_InnerRadius;}
	inline double	GetOuterRadius()	const {return m_OuterRadius;}



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

	long		m_X; 			// X 
	long		m_Y; 			// Y components of cylinder normal
	long		m_Z; 			// Z 
	double		m_XC;
	double		m_YC;		    // Cylinder centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_HalfLength;	// Half of cylinder end-to-end length (fraction of SimBox)
	double		m_InnerRadius;	// Inner radius of cylinder	(units of bead diameter)
	double		m_OuterRadius;	// Outer radius of cylinder	(units of bead diameter)


};

#endif // !defined(AFX_CCSELECTPOLYMERTYPEHEADINCYLINDER_H__75B54AB2_8A9A_4190_A3AC_076C6E4507C9__INCLUDED_)
