// mcSaveProtocolFile.h: interface for the mcSaveProtocolFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPROTOCOLFILE_H__78B1585E_F909_4312_A353_3F74A17104A9__INCLUDED_)
#define AFX_MCSAVEPROTOCOLFILE_H__78B1585E_F909_4312_A353_3F74A17104A9__INCLUDED_


// Forward declarations

class ISimCmd;

#include "xxCommand.h"

class mcSaveProtocolFile : public xxCommand 
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	mcSaveProtocolFile(long executionTime);
	mcSaveProtocolFile(const mcSaveProtocolFile& oldCommand);

	virtual ~mcSaveProtocolFile();
	
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

#endif // !defined(AFX_MCSAVEPROTOCOLFILE_H__78B1585E_F909_4312_A353_3F74A17104A9__INCLUDED_)
