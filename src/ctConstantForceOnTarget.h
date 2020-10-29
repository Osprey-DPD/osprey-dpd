// ctConstantForceOnTarget.h: interface for the ctConstantForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCONSTANTFORCEONTARGET_H__32724CCD_5361_4621_B0AB_3795BAB1D5DC__INCLUDED_)
#define AFX_CTCONSTANTFORCEONTARGET_H__32724CCD_5361_4621_B0AB_3795BAB1D5DC__INCLUDED_


#include "ctApplyCommand.h"

class ctConstantForceOnTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctConstantForceOnTarget(long executionTime);
	ctConstantForceOnTarget(const ctConstantForceOnTarget& oldCommand);

	ctConstantForceOnTarget(long executionTime, const zString targetLabel, 
							const zString decLabel, long xn, long yn, long zn,
							double mag);

	virtual ~ctConstantForceOnTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time


	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()	const {return m_DecLabel;}
	inline long          GetXNormal()	const {return m_XN;}
	inline long          GetYNormal()	const {return m_YN;}
	inline long          GetZNormal()	const {return m_ZN;}
	inline double		 GetMagnitude() const {return m_Magnitude;}

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

	zString		m_DecLabel;		// Label for the decorator object
	long        m_XN;
	long        m_YN;			// Direction of force
	long        m_ZN;
	double		m_Magnitude;	// Magnitude of force

};

#endif // !defined(AFX_CTCONSTANTFORCEONTARGET_H__32724CCD_5361_4621_B0AB_3795BAB1D5DC__INCLUDED_)
