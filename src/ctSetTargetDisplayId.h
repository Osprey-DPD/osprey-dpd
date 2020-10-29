// ctSetTargetDisplayId.h: interface for the ctSetTargetDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETDISPLAYID_H__2535387E_DF2D_46D2_9C18_C9A908E64C4D__INCLUDED_)
#define AFX_CTSETTARGETDISPLAYID_H__2535387E_DF2D_46D2_9C18_C9A908E64C4D__INCLUDED_


#include "ctApplyCommand.h"

class ctSetTargetDisplayId : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetTargetDisplayId(long executionTime);
	ctSetTargetDisplayId(const ctSetTargetDisplayId& oldCommand);

	ctSetTargetDisplayId(long executionTime, const zString name, long displayId);

	virtual ~ctSetTargetDisplayId();
	
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

	long GetDisplayId() const {return m_DisplayId;}

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

	long m_DisplayId;	// Display id to assign to all beads in the target (-1 means original bead type)
};

#endif // !defined(AFX_CTSETTARGETDISPLAYID_H__2535387E_DF2D_46D2_9C18_C9A908E64C4D__INCLUDED_)
