// mcSaveAmiraCurrentState.h: interface for the mcSaveAmiraCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEAMIRACURRENTSTATE_H__4F3A6445_CE31_4CDF_9E62_BD02C309FE3E__INCLUDED_)
#define AFX_MCSAVEAMIRACURRENTSTATE_H__4F3A6445_CE31_4CDF_9E62_BD02C309FE3E__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSaveAmiraCurrentState : public xxCommand  
{
	// ****************************************
	// Construction/Destruction
public:

	mcSaveAmiraCurrentState(long executionTime);
	mcSaveAmiraCurrentState(const mcSaveAmiraCurrentState& oldCommand);

	// Constructor for use when executing the command internally

	mcSaveAmiraCurrentState(long executionTime, bool bLog);

	virtual ~mcSaveAmiraCurrentState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;


	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

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

#endif // !defined(AFX_MCSAVEAMIRACURRENTSTATE_H__4F3A6445_CE31_4CDF_9E62_BD02C309FE3E__INCLUDED_)
