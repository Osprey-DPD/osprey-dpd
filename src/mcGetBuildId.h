// mcGetBuildId.h: interface for the mcGetBuildId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGETBUILDID_H__4DB0C6A3_DED3_4106_95E4_37236333FFA7__INCLUDED_)
#define AFX_MCGETBUILDID_H__4DB0C6A3_DED3_4106_95E4_37236333FFA7__INCLUDED_


#include "xxCommand.h"

class mcGetBuildId : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcGetBuildId(long executionTime);

	mcGetBuildId(const mcGetBuildId& oldCommand);

	virtual ~mcGetBuildId();
	
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

#endif // !defined(AFX_MCGETBUILDID_H__4DB0C6A3_DED3_4106_95E4_37236333FFA7__INCLUDED_)
