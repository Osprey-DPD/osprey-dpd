// ccRadialForce.h: interface for the ccRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCRADIALFORCE_H__79C59C47_697A_4DD6_B825_388F6C1C6CCA__INCLUDED_)
#define AFX_CCRADIALFORCE_H__79C59C47_697A_4DD6_B825_388F6C1C6CCA__INCLUDED_


#include "ccApplyForce.h"

class ccRadialForce : public ccApplyForce  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccRadialForce(long executionTime);
	ccRadialForce(const ccRadialForce& oldCommand);

	virtual ~ccRadialForce();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	// Functions to return the details for the given force law

	inline double GetXCentre()   const {return m_CX;}
	inline double GetYCentre()   const {return m_CY;}
	inline double GetZCentre()	 const {return m_CZ;}
	inline double GetMagnitude() const {return m_Magnitude;}

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

	double m_CX;
	double m_CY;			// Coordinates of force origin as fractions of SimBox sides
	double m_CZ;
	double m_Magnitude;		// Magnitude of force

};

#endif // !defined(AFX_CCRADIALFORCE_H__79C59C47_697A_4DD6_B825_388F6C1C6CCA__INCLUDED_)
