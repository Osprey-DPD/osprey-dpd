// ctCompositeCommandTarget.h: interface for the ctCompositeCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCOMPOSITECOMMANDTARGET_H__3B8A523D_8050_4BEF_8A4C_9A4CC2C9EDCF__INCLUDED_)
#define AFX_CTCOMPOSITECOMMANDTARGET_H__3B8A523D_8050_4BEF_8A4C_9A4CC2C9EDCF__INCLUDED_


#include "xxCommand.h"

class ctCompositeCommandTarget : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctCompositeCommandTarget(long executionTime);
	ctCompositeCommandTarget(const ctCompositeCommandTarget& oldCommand);

	ctCompositeCommandTarget(long executionTime, const zString target);

	virtual ~ctCompositeCommandTarget();
	
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

	inline const zString GetTargetLabel() const {return m_TargetLabel;}


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

	zString		m_TargetLabel;

};

#endif // !defined(AFX_CTCOMPOSITECOMMANDTARGET_H__3B8A523D_8050_4BEF_8A4C_9A4CC2C9EDCF__INCLUDED_)
