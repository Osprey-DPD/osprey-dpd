// cgcSetArgumentToRealSequence.h: interface for the cgcSetArgumentToRealSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOREALSEQUENCE_H__00706E60_D96A_46C7_A421_393CD1F8DEFE__INCLUDED_)
#define AFX_CGCSETARGUMENTTOREALSEQUENCE_H__00706E60_D96A_46C7_A421_393CD1F8DEFE__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentToRealSequence : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentToRealSequence(long executionTime);
	cgcSetArgumentToRealSequence(const cgcSetArgumentToRealSequence& oldCommand);

	cgcSetArgumentToRealSequence(long executionTime, const zString groupName, long cmdIndex,
                         const zString argName, double initialValue, double increment);

	virtual ~cgcSetArgumentToRealSequence();
	
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
    inline double        GetInitialValue()      const {return m_InitialValue;}
    inline double        GetIncrement()         const {return m_Increment;}

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

    zString  m_GroupName;    // Command group name
    zString  m_ArgName;      // Placeholder name for command's argument
    long     m_CommandIndex; // Index of the command whose argument is being modified
    double   m_InitialValue; // Initial value of argument
    double   m_Increment;    // Increment

};

#endif // !defined(AFX_CGCSETARGUMENTTOREALSEQUENCE_H__00706E60_D96A_46C7_A421_393CD1F8DEFE__INCLUDED_)
