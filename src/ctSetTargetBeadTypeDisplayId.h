// ctSetTargetBeadTypeDisplayId.h: interface for the ctSetTargetBeadTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETBEADTYPEDISPLAYID_H__C559BA24_2136_4F6B_8723_C673F3C53D5E__INCLUDED_)
#define AFX_CTSETTARGETBEADTYPEDISPLAYID_H__C559BA24_2136_4F6B_8723_C673F3C53D5E__INCLUDED_


#include "ctApplyCommand.h"

class ctSetTargetBeadTypeDisplayId : public ctApplyCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ctSetTargetBeadTypeDisplayId(long executionTime);
	ctSetTargetBeadTypeDisplayId(const ctSetTargetBeadTypeDisplayId& oldCommand);

	ctSetTargetBeadTypeDisplayId(long executionTime, const zString name, long beadType, long displayId);

	virtual ~ctSetTargetBeadTypeDisplayId();
	
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

	long GetBeadType()  const {return m_BeadType;}
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

	long m_BeadType;	// Numeric identifier for beads whose display id is to be modified
	long m_DisplayId;	// Display id to assign to beads of the specified type in the 
	                    // target (-1 means original bead type)
};

#endif // !defined(AFX_CTSETTARGETBEADTYPEDISPLAYID_H__C559BA24_2136_4F6B_8723_C673F3C53D5E__INCLUDED_)
