// ctPolymerisePolymerTripleInTarget.h: interface for the ctPolymerisePolymerTripleInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGET_H__19D1F30D_1AAC_4213_8467_608A81CA6BDD__INCLUDED_)
#define AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGET_H__19D1F30D_1AAC_4213_8467_608A81CA6BDD__INCLUDED_


#include "ctApplyCommand.h"

class ctPolymerisePolymerTripleInTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctPolymerisePolymerTripleInTarget(long executionTime);
	ctPolymerisePolymerTripleInTarget(const ctPolymerisePolymerTripleInTarget& oldCommand);

	ctPolymerisePolymerTripleInTarget(long executionTime, const zString target,
									  const zString polyName, long bondTotal,
									  zLongVector vHeads, zLongVector vTails, 
									  double spring, double length);

	virtual ~ctPolymerisePolymerTripleInTarget();
	
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

	inline const zString GetPolymerName() const {return m_PolymerName;}
	inline long GetBondTotal()			  const {return m_BondTotal;}
	inline zLongVector GetHeadPositions() const {return m_HeadPositions;}
	inline zLongVector GetTailPositions() const {return m_TailPositions;}
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

	zString		m_PolymerName;			// Name of polymer type
	long		m_BondTotal;			// Number of positions to polymerise
	zLongVector	m_HeadPositions;		// Positions of bond heads in polymer shape string
	zLongVector	m_TailPositions;		// Positions of bond tails in polymer shape string
	double		m_SpringConstant;		// Hookean parameters for bonds
	double		m_UnstretchedLength;


};

#endif // !defined(AFX_CTPOLYMERISEPOLYMERTRIPLEINTARGET_H__19D1F30D_1AAC_4213_8467_608A81CA6BDD__INCLUDED_)
