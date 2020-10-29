// cgtToggleCommandExecutionInGroup.h: interface for the cgtToggleCommandExecutionInGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTTOGGLECOMMANDEXECUTIONINGROUP_H__0B65B421_1670_4065_BA77_100D793D5015__INCLUDED_)
#define AFX_CGTTOGGLECOMMANDEXECUTIONINGROUP_H__0B65B421_1670_4065_BA77_100D793D5015__INCLUDED_



#include "xxCommand.h"

class cgtToggleCommandExecutionInGroup : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgtToggleCommandExecutionInGroup(long executionTime);
	cgtToggleCommandExecutionInGroup(const cgtToggleCommandExecutionInGroup& oldCommand);

	cgtToggleCommandExecutionInGroup(long executionTime, const zString groupName, long cmdIndex);

	virtual ~cgtToggleCommandExecutionInGroup();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	 // Return the type of command
    static long  GetArgumentTotal(); // Return the number of arguments 

private:

	static const zString m_Type;	// Identifier used in control data file for command
    static long  m_ArgumentTotal;   // Number of arguments required not including the execution time

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

    virtual long GetCommandArgumentTotal() const {return m_ArgumentTotal;}

    virtual bool Pack(const tguArgumentSequence& vArguments);

	// ****************************************
	// Public access functions
	
	inline const zString GetCommandGroupName()	 const {return m_GroupName;};
	inline long          GetCommandIndex()	     const {return m_CommandIndex;};

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

	zString  m_GroupName;	 // Command group name
	long     m_CommandIndex; // Index of command whose execution status is being toggled

};

#endif // !defined(AFX_CGTTOGGLECOMMANDEXECUTIONINGROUP_H__0B65B421_1670_4065_BA77_100D793D5015__INCLUDED_)
