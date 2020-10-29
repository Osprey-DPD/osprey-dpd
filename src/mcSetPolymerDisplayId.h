// mcSetPolymerDisplayId.h: interface for the mcSetPolymerDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETPOLYMERDISPLAYID_H__01875676_A52E_4A41_B4EB_AB07CE7F6E21__INCLUDED_)
#define AFX_MCSETPOLYMERDISPLAYID_H__01875676_A52E_4A41_B4EB_AB07CE7F6E21__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetPolymerDisplayId : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetPolymerDisplayId(long executionTime);
	mcSetPolymerDisplayId(const mcSetPolymerDisplayId& oldCommand);

	mcSetPolymerDisplayId(long executionTime, const zString name, long displayId);

	virtual ~mcSetPolymerDisplayId();
	
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

	const zString GetPolymerName() const {return m_PolymerName;}
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

	zString	m_PolymerName;	// String identifier for polymer to modify
	long    m_DisplayId;	// Display id to assign to polymer type (-1 means each bead uses its type)

};

#endif // !defined(AFX_MCSETPOLYMERDISPLAYID_H__01875676_A52E_4A41_B4EB_AB07CE7F6E21__INCLUDED_)
