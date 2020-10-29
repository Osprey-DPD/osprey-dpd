// mcSetAllBeadsInvisible.h: interface for the mcSetAllBeadsInvisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETALLBEADSINVISIBLE_H__C20D6144_32F8_46C1_90BC_39D8A9704878__INCLUDED_)
#define AFX_MCSETALLBEADSINVISIBLE_H__C20D6144_32F8_46C1_90BC_39D8A9704878__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetAllBeadsInvisible : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSetAllBeadsInvisible(long executionTime);
	mcSetAllBeadsInvisible(const mcSetAllBeadsInvisible& oldCommand);

	mcSetAllBeadsInvisible(long executionTime, const zString name);

	virtual ~mcSetAllBeadsInvisible();
	
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

#endif // !defined(AFX_MCSETALLBEADSINVISIBLE_H__C20D6144_32F8_46C1_90BC_39D8A9704878__INCLUDED_)
