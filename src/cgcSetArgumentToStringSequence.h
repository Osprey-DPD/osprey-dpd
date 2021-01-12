// cgcSetArgumentToStringSequence.h: interface for the cgcSetArgumentToStringSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOSTRINGSEQUENCE_H__111F7036_32BA_4C26_90E3_314F9BCC6CA4__INCLUDED_)
#define AFX_CGCSETARGUMENTTOSTRINGSEQUENCE_H__111F7036_32BA_4C26_90E3_314F9BCC6CA4__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentToStringSequence : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentToStringSequence(long executionTime);
	cgcSetArgumentToStringSequence(const cgcSetArgumentToStringSequence& oldCommand);

	cgcSetArgumentToStringSequence(long executionTime, const zString groupName, long cmdIndex,
                         const zString argName, const zString root);

	virtual ~cgcSetArgumentToStringSequence();
	
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
	
	inline const zString GetCommandGroupName()	const {return m_GroupName;};
	inline long          GetCommandIndex()	    const {return m_CommandIndex;};
    inline const zString GetArgumentName()      const {return m_ArgName;}
    inline const zString GetPrefix()            const {return m_Root;}

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
    long     m_CommandIndex; // Index of the command whose argument is being modified
    zString  m_ArgName;      // Placeholder name for command's argument
    zString  m_Root;         // Initial value of argument

};

#endif // !defined(AFX_CGCSETARGUMENTTOSTRINGSEQUENCE_H__111F7036_32BA_4C26_90E3_314F9BCC6CA4__INCLUDED_)
