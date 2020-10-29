// ctSetTargetInvisible.h: interface for the ctSetTargetInvisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETINVISIBLE_H__E2DAEE11_6095_4B82_8404_A5C462647F5E__INCLUDED_)
#define AFX_CTSETTARGETINVISIBLE_H__E2DAEE11_6095_4B82_8404_A5C462647F5E__INCLUDED_


#include "ctApplyCommand.h"

class ctSetTargetInvisible : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetTargetInvisible(long executionTime);
	ctSetTargetInvisible(const ctSetTargetInvisible& oldCommand);

	ctSetTargetInvisible(long executionTime, const zString name);

	virtual ~ctSetTargetInvisible();
	
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

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

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

};

#endif // !defined(AFX_CTSETTARGETINVISIBLE_H__E2DAEE11_6095_4B82_8404_A5C462647F5E__INCLUDED_)
