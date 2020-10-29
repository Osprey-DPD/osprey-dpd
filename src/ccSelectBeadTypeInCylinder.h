// ccSelectBeadTypeInCylinder.h: interface for the ccSelectBeadTypeInCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINCYLINDER_H__B06F1327_0CF0_4293_858C_42F2850D9387__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINCYLINDER_H__B06F1327_0CF0_4293_858C_42F2850D9387__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInCylinder : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInCylinder(long executionTime);
	ccSelectBeadTypeInCylinder(const ccSelectBeadTypeInCylinder& oldCommand);

	ccSelectBeadTypeInCylinder(long executionTime, const zString target, const zString bead,
							   long xn, long yn, long zn, double xc, double yc, double zc, 
							   double halfLength, double inRadius, double outRadius);

	virtual ~ccSelectBeadTypeInCylinder();
	
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

#endif // !defined(AFX_CCSELECTBEADTYPEINCYLINDER_H__B06F1327_0CF0_4293_858C_42F2850D9387__INCLUDED_)
