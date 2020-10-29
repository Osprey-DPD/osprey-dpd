// mcSetAllBeadsVisible.h: interface for the mcSetAllBeadsVisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETALLBEADSVISIBLE_H__1C907E7F_B3AA_4C3D_8EFC_0922626A2290__INCLUDED_)
#define AFX_MCSETALLBEADSVISIBLE_H__1C907E7F_B3AA_4C3D_8EFC_0922626A2290__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetAllBeadsVisible : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetAllBeadsVisible(long executionTime);
	mcSetAllBeadsVisible(const mcSetAllBeadsVisible& oldCommand);

	mcSetAllBeadsVisible(long executionTime, const zString name);

	virtual ~mcSetAllBeadsVisible();
	
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

#endif // !defined(AFX_MCSETALLBEADSVISIBLE_H__1C907E7F_B3AA_4C3D_8EFC_0922626A2290__INCLUDED_)
