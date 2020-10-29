// ccSelectBeadTypeInSphere.h: interface for the ccSelectBeadTypeInSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTBEADTYPEINSPHERE_H__8A7037BB_5A11_4BCD_989E_FFA46B823106__INCLUDED_)
#define AFX_CCSELECTBEADTYPEINSPHERE_H__8A7037BB_5A11_4BCD_989E_FFA46B823106__INCLUDED_


#include "ccSelectBeadTypeInRegion.h"

class ccSelectBeadTypeInSphere : public ccSelectBeadTypeInRegion  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccSelectBeadTypeInSphere(long executionTime);
	ccSelectBeadTypeInSphere(const ccSelectBeadTypeInSphere& oldCommand);

	virtual ~ccSelectBeadTypeInSphere();
	
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

	inline double	GetXCentre()		const {return m_XC;}
	inline double	GetYCentre()		const {return m_YC;}
	inline double	GetZCentre()		const {return m_ZC;}
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

	double		m_XC;
	double		m_YC;		    // Sphere's centre as fractions of SimBox side lengths
	double		m_ZC;
	double		m_InnerRadius;	// Inner radius of spherical shell	(units of bead diameter)
	double		m_OuterRadius;	// Outer radius of spherical shell	(units of bead diameter)
};

#endif // !defined(AFX_CCSELECTBEADTYPEINSPHERE_H__8A7037BB_5A11_4BCD_989E_FFA46B823106__INCLUDED_)
