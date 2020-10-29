// ctChangeNamedBeadType.h: interface for the ctChangeNamedBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGENAMEDBEADTYPE_H__E3FA4560_043A_4A90_B823_3761582A865C__INCLUDED_)
#define AFX_CTCHANGENAMEDBEADTYPE_H__E3FA4560_043A_4A90_B823_3761582A865C__INCLUDED_


#include "ctApplyCommand.h"

class ctChangeNamedBeadType : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctChangeNamedBeadType(long executionTime);
	ctChangeNamedBeadType(const ctChangeNamedBeadType& oldCommand);

	ctChangeNamedBeadType(long executionTime, const zString target, const zString newBeadName);

	virtual ~ctChangeNamedBeadType();
	
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


#endif // !defined(AFX_CTCHANGENAMEDBEADTYPE_H__E3FA4560_043A_4A90_B823_3761582A865C__INCLUDED_)
