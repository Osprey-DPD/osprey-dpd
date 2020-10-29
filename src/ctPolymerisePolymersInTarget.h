// ctPolymerisePolymersInTarget.h: interface for the ctPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTPOLYMERISEPOLYMERSINTARGET_H__D6AC3395_3222_4756_BA15_1FE6E2573207__INCLUDED_)
#define AFX_CTPOLYMERISEPOLYMERSINTARGET_H__D6AC3395_3222_4756_BA15_1FE6E2573207__INCLUDED_


#include "ctApplyCommand.h"

class ctPolymerisePolymersInTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctPolymerisePolymersInTarget(long executionTime);
	ctPolymerisePolymersInTarget(const ctPolymerisePolymersInTarget& oldCommand);

	ctPolymerisePolymersInTarget(long executionTime, const zString target,
								 long maxBonds, double range, double fraction, double spring, double length);

	virtual ~ctPolymerisePolymersInTarget();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command
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

	inline long   GetMaxBondsPerPolymer() const {return m_MaxBonds;}
	inline double GetRange()			  const {return m_Range;}
	inline double GetBondFraction()		  const {return m_Fraction;}
	inline double GetSpringConstant()	  const {return m_SpringConstant;}
	inline double GetUnstretchedLength()  const {return m_UnstretchedLength;}


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

	long		m_MaxBonds;				// Max bonds per polymer
	double		m_Range;				// Max separation within which to create bonds
	double		m_Fraction;				// Fraction of neighbours to be bound
	double		m_SpringConstant;		// Hookean parameters for bonds
	double		m_UnstretchedLength;


};

#endif // !defined(AFX_CTPOLYMERISEPOLYMERSINTARGET_H__D6AC3395_3222_4756_BA15_1FE6E2573207__INCLUDED_)
