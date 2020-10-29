// ctChangeNamedBeadTypeInCompositeTarget.h: interface for the ctChangeNamedBeadTypeInCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__479BCECA_3EDC_4976_9ABA_F84DEF748581__INCLUDED_)
#define AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__479BCECA_3EDC_4976_9ABA_F84DEF748581__INCLUDED_


#include "ctApplyCommand.h"

class ctChangeNamedBeadTypeInCompositeTarget : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctChangeNamedBeadTypeInCompositeTarget(long executionTime);
	ctChangeNamedBeadTypeInCompositeTarget(const ctChangeNamedBeadTypeInCompositeTarget& oldCommand);

	ctChangeNamedBeadTypeInCompositeTarget(long executionTime, const zString target, const zString newBeadName);

	virtual ~ctChangeNamedBeadTypeInCompositeTarget();
	
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


	inline const zString GetNewBeadName()   const {return m_NewBeadName;}

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

	zString		m_NewBeadName; // New name for target's beads

};

#endif // !defined(AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__479BCECA_3EDC_4976_9ABA_F84DEF748581__INCLUDED_)
