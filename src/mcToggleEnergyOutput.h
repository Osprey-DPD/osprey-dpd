// mcToggleEnergyOutput.h: interface for the mcToggleEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEENERGYOUTPUT_H__54521238_A89A_42BB_8896_A704E4C4B851__INCLUDED_)
#define AFX_MCTOGGLEENERGYOUTPUT_H__54521238_A89A_42BB_8896_A704E4C4B851__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcToggleEnergyOutput : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction

	mcToggleEnergyOutput(long executionTime);
	mcToggleEnergyOutput(const mcToggleEnergyOutput& oldCommand);

	virtual ~mcToggleEnergyOutput();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// Public access functions
public:

	bool NormalizePerBead() const;

	// ****************************************
	// PVFs that must be overridden by all derived classes

public:
	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

protected:
	virtual const zString GetCommandType() const;


	// ****************************************
	// Data members
private:

	bool  m_bNormalizePerBead;		// Flag showing whether U or U/N is output
};

#endif // !defined(AFX_MCTOGGLEENERGYOUTPUT_H__54521238_A89A_42BB_8896_A704E4C4B851__INCLUDED_)
