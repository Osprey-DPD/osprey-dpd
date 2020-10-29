// ctSelectBeadsInSphericalTargetRegion.h: interface for the ctSelectBeadsInSphericalTargetRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSELECTBEADSINSPHERICALTARGETREGION_H__4CFFC009_4C86_4A86_A5DD_05C15E361F9B__INCLUDED_)
#define AFX_CTSELECTBEADSINSPHERICALTARGETREGION_H__4CFFC009_4C86_4A86_A5DD_05C15E361F9B__INCLUDED_


#include "xxCommand.h"

class ctSelectBeadsInSphericalTargetRegion : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSelectBeadsInSphericalTargetRegion(long executionTime);
    ctSelectBeadsInSphericalTargetRegion(long executionTime, const zString sourceLabel, 
                               const zString newLabel, const zString beadName,
                               double radius1, double radius2, 
                               double theta1, double theta2, 
                               double phi1, double phi2);

	ctSelectBeadsInSphericalTargetRegion(const ctSelectBeadsInSphericalTargetRegion& oldCommand);

	virtual ~ctSelectBeadsInSphericalTargetRegion();
	
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

	inline const zString  GetSourceLabel()      const {return m_SourceLabel;}
	inline const zString  GetNewTargetLabel()   const {return m_NewLabel;}
	inline const zString  GetBeadName()         const {return m_BeadName;}
	inline double GetInnerRadius()              const {return m_Radius1;}
	inline double GetOuterRadius()              const {return m_Radius2;}
	inline double GetLowerPolarAngle()          const {return m_Theta1;}
	inline double GetUpperPolarAngle()          const {return m_Theta2;}
	inline double GetLowerAzimuthalAngle()      const {return m_Phi1;}
	inline double GetUpperAzimuthalAngle()      const {return m_Phi2;}


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

    zString     m_SourceLabel;  // Source target name
    zString     m_NewLabel;     // Bead target name
    zString     m_BeadName;     // String bead name

	double      m_Radius1;   // Inner radius of region (units of bead diameter)
	double      m_Radius2;   // Outer radius of region
	double      m_Theta1;    // Lower polar angle of region (degrees)
	double      m_Theta2;    // Uppper polar angle of region (degrees)
	double      m_Phi1;      // Lower azimuthal angle of region (degrees)
	double      m_Phi2;      // Upper azimuthal angle of region (degrees)
	
};

#endif // !defined(AFX_CTSELECTBEADSINSPHERICALTARGETREGION_H__4CFFC009_4C86_4A86_A5DD_05C15E361F9B__INCLUDED_)

