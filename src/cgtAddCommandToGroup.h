// cgtAddCommandToGroup.h: interface for the cgtAddCommandToGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTADDCOMMANDTOGROUP_H__08B6BA92_0B0A_4F93_A2AC_329358861411__INCLUDED_)
#define AFX_CGTADDCOMMANDTOGROUP_H__08B6BA92_0B0A_4F93_A2AC_329358861411__INCLUDED_



#include "xxCommand.h"

class cgtAddCommandToGroup : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgtAddCommandToGroup(long executionTime);
	cgtAddCommandToGroup(const cgtAddCommandToGroup& oldCommand);

	cgtAddCommandToGroup(long executionTime, const zString groupName, const zString cmdName,
                         StringSequence argNames);

	virtual ~cgtAddCommandToGroup();
	
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
	
	inline const zString  GetCommandGroupName()	    const {return m_GroupName;};
	inline const zString  GetCommandName()	        const {return m_CommandName;};
    inline const zString  GetArgumentName(long i)   const {return m_ArgNames.at(i);}
    inline StringSequence GetArguments()            const {return m_ArgNames;}

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

	zString         m_GroupName;	// Command group name
	zString         m_CommandName;  // Name of command to add to the group
    StringSequence  m_ArgNames;     // Placeholder names for command's arguments

};

#endif // !defined(AFX_CGTADDCOMMANDTOGROUP_H__08B6BA92_0B0A_4F93_A2AC_329358861411__INCLUDED_)
