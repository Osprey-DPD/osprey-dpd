// ctSpringForceBetweenTargets.h: interface for the ctSpringForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSPRINGFORCEBETWEENTARGETS_H__84f199bf_c343_4297_b756_92b80d175278__INCLUDED_)
#define AFX_CTSPRINGFORCEBETWEENTARGETS_H__84f199bf_c343_4297_b756_92b80d175278__INCLUDED_


#include "ctApplyCommand.h"

class ctSpringForceBetweenTargets : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSpringForceBetweenTargets(long executionTime);
	ctSpringForceBetweenTargets(const ctSpringForceBetweenTargets& oldCommand);

	ctSpringForceBetweenTargets(long executionTime, const zString target1, const zString target2,
						  const zString decLabel, double keff, double deltax, long period);

	virtual ~ctSpringForceBetweenTargets();
	
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

	inline const zString GetTarget2Label()	const {return m_Target2Label;}
	inline const zString GetDecLabel()	    const {return m_DecLabel;}
	inline double		 GetMagnitude()     const {return m_Keff;}
	inline double		 GetDeltaX()        const {return m_DeltaX;}
	inline long		     GetPeriod()        const {return m_Period;}


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

	zString	m_Target2Label;	// Label for the second target object
	zString	m_DecLabel;	    // Label for the decorator object
	double	m_Keff;			// Magnitude of force (= spring constant of harmonic potential)
    double  m_DeltaX;       // Step change in the separation
    long    m_Period;       // Number of time-steps between step changes in the separation

};

#endif // !defined(AFX_CTSPRINGFORCEBETWEENTARGETS_H__84f199bf_c343_4297_b756_92b80d175278__INCLUDED_)
