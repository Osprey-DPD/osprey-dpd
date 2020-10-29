// xxCommand.h: interface for the xxCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXCOMMAND_H__B75BC543_E47F_11D3_BF23_004095086186__INCLUDED_)
#define AFX_XXCOMMAND_H__B75BC543_E47F_11D3_BF23_004095086186__INCLUDED_

// Forward declarations

class ISimCmd;
class CInputData;

// Include files that contain flags to control conditional compilation
// of the derived classes, and XML-related flags included here to avoid
// every derived class having to explicitly include it.

#include "SimCommandFlags.h"
#include "SimXMLFlags.h"

// Include file that contains a wrapper class for arguments to commands
// that are unpacked when the command is executed as part of a command group.

#include "tguArgumentType.h"

// Include file for the Command Factory object

#include "acfCommandFactory.h"


#include "xxBase.h"

class xxCommand : public xxBase  
{

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxCommand();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const xxCommand& command);
	friend zInStream&  operator>>(zInStream& is,        xxCommand& command);

	static long GetCommandTotal();
	static void ZeroCommandTotal();

	static long m_CommandTotal;	// Number of commands created

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	virtual zOutStream& put(zOutStream& os) const = 0;
	virtual zInStream&  get(zInStream& is)        = 0;

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const = 0;

	virtual const zString GetCommandType() const = 0;

	virtual const xxCommand* GetCommand()  const = 0;

	virtual bool IsDataValid(const CInputData& riData) const = 0;

	virtual long GetCommandArgumentTotal() const;

    // VF to fill up an empty command instace with values for all of its parameters.

	virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
public:

	inline long GetId()			                const {return m_id;}
	inline long GetExecutionTime()              const {return m_Time;}
    inline bool IsTimeToExecute(long simTime)   const {return (simTime == m_Time);}
	inline bool IsCommandValid()                const {return m_bCommandValid;}
	inline bool IsExecutionLogged()             const {return m_bLogExecution;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	xxCommand(long executionTime);
	
	// Constructor used by commands that are created internally so that they can
	// enable or disable execution logging.

	xxCommand(long executionTime, bool bLog);

	xxCommand(const xxCommand& oldCommand);

	void SetCommandValid(bool bValid);

	// ****************************************
	// Implementation

	// XML output
	zOutStream& putXMLStartTags(zOutStream& os) const;
	zOutStream& putXMLEndTags(zOutStream& os) const;

	// ASCII output
	zOutStream& putASCIIStartTags(zOutStream& os) const;
	zOutStream& putASCIIEndTags(zOutStream& os) const;

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const long m_id;			// Unique identifier for the command

	const long m_Time;			// Time at which command executes
	bool m_bCommandValid;		// Flag showing if command has been validated
	bool m_bLogExecution;		// Flag showing if command execution is to be logged

};

#endif // !defined(AFX_XXCOMMAND_H__B75BC543_E47F_11D3_BF23_004095086186__INCLUDED_)
