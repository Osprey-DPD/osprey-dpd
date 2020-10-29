// mcToggleCurrentStateAnalysis.h: interface for the mcToggleCurrentStateAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLECURRENTSTATEANALYSIS_H__2999DC62_7B94_4028_8A62_727062E5E039__INCLUDED_)
#define AFX_MCTOGGLECURRENTSTATEANALYSIS_H__2999DC62_7B94_4028_8A62_727062E5E039__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcToggleCurrentStateAnalysis : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:
	mcToggleCurrentStateAnalysis(long executionTime);
	mcToggleCurrentStateAnalysis(const mcToggleCurrentStateAnalysis& oldCommand);

	virtual ~mcToggleCurrentStateAnalysis();

	// ****************************************
	// Global functions, static member functions and variables

	// ****************************************
	// Public access functions
public:


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


#endif // !defined(AFX_MCTOGGLECURRENTSTATEANALYSIS_H__2999DC62_7B94_4028_8A62_727062E5E039__INCLUDED_)
