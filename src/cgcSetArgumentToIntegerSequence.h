// cgcSetArgumentToIntegerSequence.h: interface for the cgcSetArgumentToIntegerSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOINTEGERSEQUENCE_H__85D53D14_66B2_4F89_B8E5_53F4328AE1FB__INCLUDED_)
#define AFX_CGCSETARGUMENTTOINTEGERSEQUENCE_H__85D53D14_66B2_4F89_B8E5_53F4328AE1FB__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentToIntegerSequence : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentToIntegerSequence(long executionTime);
	cgcSetArgumentToIntegerSequence(const cgcSetArgumentToIntegerSequence& oldCommand);

	cgcSetArgumentToIntegerSequence(long executionTime, const zString groupName, long cmdIndex,
                         const zString argName, long initialValue, long increment);

	virtual ~cgcSetArgumentToIntegerSequence();
	
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
    inline long          GetInitialValue()      const {return m_InitialValue;}
    inline long          GetIncrement()         const {return m_Increment;}

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
    long     m_InitialValue; // Initial value of argument
    long     m_Increment;    // Increment

};

#endif // !defined(AFX_CGCSETARGUMENTTOINTEGERSEQUENCE_H__85D53D14_66B2_4F89_B8E5_53F4328AE1FB__INCLUDED_)
