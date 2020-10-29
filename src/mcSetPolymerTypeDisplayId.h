// mcSetPolymerTypeDisplayId.h: interface for the mcSetPolymerTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETPOLYMERTYPEDISPLAYID_H__9032E002_853B_4765_836A_F470A4FC81C3__INCLUDED_)
#define AFX_MCSETPOLYMERTYPEDISPLAYID_H__9032E002_853B_4765_836A_F470A4FC81C3__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetPolymerTypeDisplayId : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetPolymerTypeDisplayId(long executionTime);
	mcSetPolymerTypeDisplayId(const mcSetPolymerTypeDisplayId& oldCommand);

	mcSetPolymerTypeDisplayId(long executionTime, long polymerType, long displayId);

	virtual ~mcSetPolymerTypeDisplayId();
	
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

	long    m_PolymerType;	// Numeric identifier for polymer to modify
	long    m_DisplayId;	// Display id to assign to polymer type (-1 means each bead uses its type)

};

#endif // !defined(AFX_MCSETPOLYMERTYPEDISPLAYID_H__9032E002_853B_4765_836A_F470A4FC81C3__INCLUDED_)
