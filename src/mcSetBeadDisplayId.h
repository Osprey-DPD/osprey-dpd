// mcSetBeadDisplayId.h: interface for the mcSetBeadDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETBEADDISPLAYID_H__932BC498_BD02_4160_ABA7_D2B494DF455E__INCLUDED_)
#define AFX_MCSETBEADDISPLAYID_H__932BC498_BD02_4160_ABA7_D2B494DF455E__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetBeadDisplayId : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetBeadDisplayId(long executionTime);
	mcSetBeadDisplayId(const mcSetBeadDisplayId& oldCommand);

	mcSetBeadDisplayId(long executionTime, const zString name, long displayId);

	virtual ~mcSetBeadDisplayId();
	
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

	const zString GetBeadName()  const {return m_BeadName;}
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

	zString	m_BeadName;	 // String identifier for bead type to modify
	long    m_DisplayId; // Display id to assign to all beads of type (-1 means use bead's type)

};

#endif // !defined(AFX_MCSETBEADDISPLAYID_H__932BC498_BD02_4160_ABA7_D2B494DF455E__INCLUDED_)
