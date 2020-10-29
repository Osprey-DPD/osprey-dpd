// mcToggleCurrentStateBox.h: interface for the mcToggleCurrentStateBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLECURRENTSTATEBOX_H__96A05402_76E1_499C_9CB0_375FC93E1FCA__INCLUDED_)
#define AFX_MCTOGGLECURRENTSTATEBOX_H__96A05402_76E1_499C_9CB0_375FC93E1FCA__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcToggleCurrentStateBox : public xxCommand 
{

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:
	mcToggleCurrentStateBox(long executionTime);
	mcToggleCurrentStateBox(const mcToggleCurrentStateBox& oldCommand);

	virtual ~mcToggleCurrentStateBox();

	// ****************************************
	// Global functions, static member functions and variables


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	static const zString m_Type;	// Identifier used in control data file for command
};

#endif // !defined(AFX_MCTOGGLECURRENTSTATEBOX_H__96A05402_76E1_499C_9CB0_375FC93E1FCA__INCLUDED_)

