// ctSetTargetPolymerTypeDisplayId.h: interface for the ctSetTargetPolymerTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETPOLYMERTYPEDISPLAYID_H__EE2B02A0_BFDA_40D2_9387_8137AD769C1D__INCLUDED_)
#define AFX_CTSETTARGETPOLYMERTYPEDISPLAYID_H__EE2B02A0_BFDA_40D2_9387_8137AD769C1D__INCLUDED_


#include "ctApplyCommand.h"

class ctSetTargetPolymerTypeDisplayId : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetTargetPolymerTypeDisplayId(long executionTime);
	ctSetTargetPolymerTypeDisplayId(const ctSetTargetPolymerTypeDisplayId& oldCommand);

	ctSetTargetPolymerTypeDisplayId(long executionTime, const zString name, long polymerType, long displayId);

	virtual ~ctSetTargetPolymerTypeDisplayId();
	
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

	long GetPolymerType() const {return m_PolymerType;}
	long GetDisplayId()   const {return m_DisplayId;}

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

	long m_PolymerType;	// Numeric identifier for polymers whose display id is to be modified
	long m_DisplayId;	// Display id to assign to all beads in polymers of the specified type 
	                    // in the target (-1 means original bead type)
};

#endif // !defined(AFX_CTSETTARGETPOLYMERTYPEDISPLAYID_H__EE2B02A0_BFDA_40D2_9387_8137AD769C1D__INCLUDED_)
