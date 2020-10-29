// mcSetBeadTypeDisplayId.h: interface for the mcSetBeadTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETBEADTYPEDISPLAYID_H__8C33F7F4_0BD9_4313_AFE7_E5B829B30424__INCLUDED_)
#define AFX_MCSETBEADTYPEDISPLAYID_H__8C33F7F4_0BD9_4313_AFE7_E5B829B30424__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetBeadTypeDisplayId : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetBeadTypeDisplayId(long executionTime);
	mcSetBeadTypeDisplayId(const mcSetBeadTypeDisplayId& oldCommand);

	mcSetBeadTypeDisplayId(long executionTime, long beadType, long displayId);

	virtual ~mcSetBeadTypeDisplayId();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;


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

	long    m_BeadType;	    // Numeric identifier for bead type to modify
	long    m_DisplayId;	// Display id to assign to all beads of type (-1 means use bead's type)

};

#endif // !defined(AFX_MCSETBEADTYPEDISPLAYID_H__8C33F7F4_0BD9_4313_AFE7_E5B829B30424__INCLUDED_)
