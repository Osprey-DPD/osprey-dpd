// ctSpringForceOnTarget.h: interface for the ctSpringForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSPRINGFORCEONTARGET_H__fc4fafb0_9e79_4002_be88_e077d5bcb389__INCLUDED_)
#define AFX_CTSPRINGFORCEONTARGET_H__fc4fafb0_9e79_4002_be88_e077d5bcb389__INCLUDED_


#include "ctApplyCommand.h"

class ctSpringForceOnTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSpringForceOnTarget(long executionTime);
	ctSpringForceOnTarget(const ctSpringForceOnTarget& oldCommand);

	ctSpringForceOnTarget(long executionTime, const zString target,
						  const zString decLabel, double xc, double yc, double zc, double keff);

	virtual ~ctSpringForceOnTarget();
	
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


	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()	const {return m_DecLabel;}
	inline double		 GetXCentre()	const {return m_XC;}
	inline double		 GetYCentre()	const {return m_YC;}
	inline double		 GetZCentre()	const {return m_ZC;}
	inline double		 GetMagnitude() const {return m_keff;}


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

	zString	m_DecLabel;	// Label for the decorator object
	double	m_XC;
	double	m_YC;			// Coordinates of restraining "origin" as fractions of SimBox sides
	double	m_ZC;
	double	m_keff;			// Magnitude of force (= spring constant of harmonic potential)

};

#endif // !defined(AFX_CTSPRINGFORCEONTARGET_H__fc4fafb0_9e79_4002_be88_e077d5bcb389__INCLUDED_)
