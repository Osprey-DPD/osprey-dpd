// mcSaveNonBeadInclusiveRestartState.h: interface for the mcSaveNonBeadInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVENONBEADINCLUSIVERESTARTSTATE_H__B2925B5C_458A_4BB6_9AF8_A417FF9A55A9__INCLUDED_)
#define AFX_MCSAVENONBEADINCLUSIVERESTARTSTATE_H__B2925B5C_458A_4BB6_9AF8_A417FF9A55A9__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSaveNonBeadInclusiveRestartState : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveNonBeadInclusiveRestartState(long executionTime);
	mcSaveNonBeadInclusiveRestartState(const mcSaveNonBeadInclusiveRestartState& oldCommand);

	virtual ~mcSaveNonBeadInclusiveRestartState();
	
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

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command.

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

#endif // !defined(AFX_MCSAVENONBEADINCLUSIVERESTARTSTATE_H__B2925B5C_458A_4BB6_9AF8_A417FF9A55A9__INCLUDED_)
