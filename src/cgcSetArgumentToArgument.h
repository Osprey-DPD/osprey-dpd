// cgcSetArgumentToArgument.h: interface for the cgcSetArgumentToArgument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOARGUMENT_H__09467AF1_06D3_4BBB_8F1C_2961CAE813BE__INCLUDED_)
#define AFX_CGCSETARGUMENTTOARGUMENT_H__09467AF1_06D3_4BBB_8F1C_2961CAE813BE__INCLUDED_



#include "xxCommand.h"

class cgcSetArgumentToArgument : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgcSetArgumentToArgument(long executionTime);
	cgcSetArgumentToArgument(const cgcSetArgumentToArgument& oldCommand);

	cgcSetArgumentToArgument(long executionTime,  const zString groupName, 
                             const zString sourceArgName, const zString destArgName, 
                             long sourceCmdIndex, long destCmdIndex);

	virtual ~cgcSetArgumentToArgument();
	
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
	
	inline const zString GetCommandGroupName()	        const {return m_GroupName;};
    inline const zString GetSourceArgumentName()        const {return m_SourceArgName;}
    inline const zString GetDestinationArgumentName()   const {return m_DestArgName;}
	inline long          GetSourceCommandIndex()        const {return m_SourceCmdIndex;};
	inline long          GetDestinationCommandIndex()   const {return m_DestCmdIndex;};

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

	zString      m_GroupName;	    // Command group name
    zString      m_SourceArgName;   // Placeholder name for the source command's argument
    zString      m_DestArgName;     // Placeholder name for destination command's argument
    long         m_SourceCmdIndex;  // Index of the source command whose argument is being copied
    long         m_DestCmdIndex;    // Index of the destination command whose argument is being modified

};

#endif // !defined(AFX_CGCSETARGUMENTTOARGUMENT_H__09467AF1_06D3_4BBB_8F1C_2961CAE813BE__INCLUDED_)
