// tguCommandGroup.h: interface for the tguCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGUCOMMANDGROUP_H__6AEB8958_DFCF_44D1_8A3C_4850A04F9BFB__INCLUDED_)
#define AFX_TGUCOMMANDGROUP_H__6AEB8958_DFCF_44D1_8A3C_4850A04F9BFB__INCLUDED_


// Forward declarations

class ISimState;
class tguCommands;

#include "xxBase.h"

class tguCommandGroup : public xxBase
{
	// ****************************************
	// Construction/Destruction
public:

	tguCommandGroup(const zString name);

    tguCommandGroup(const tguCommandGroup& oldGroup);


	virtual ~tguCommandGroup();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:


	// ****************************************
	// Public access functions
public:

    // Accessor functions to the group's data
    inline const zString GetName() const {return m_Name;}
    inline bool          IsActive() const {return m_bActive;}

	void SetActive(bool bActive);

    const zString GetCommandName(long cmdNo) const;
	bool IsCommandInGroup(long cmdNo) const;


    // Function used by the SimBox to repeatedly execute the group
	bool TimeToExecute(long currentTime) const;
    bool Execute(ISimState* const pISimState);


    // Execution profiles for the group
	bool ExecuteOnce(ISimState* const pISimState);
	bool ExecuteSequence(ISimState* const pISimState, long total, long period);


    // Functions that modify commands in the group
	void AddCommand(const zString cmdName, StringSequence argNames);
	bool ToggleCommandExecution(long cmdNo);

    // Functions that modify command arguments

	void         SetArgumentToInteger(long cmdNo, const zString argName, long argValue);
	void            SetArgumentToReal(long cmdNo, const zString argName, double argValue);
	void          SetArgumentToString(long cmdNo, const zString argName, const zString argValue);
	void        SetArgumentToArgument(long cmdNo, const zString argName, long cmdNoSource, const zString argNameSource);
	void SetArgumentToIntegerSequence(long cmdNo, const zString argName, long initial, long delta);
	void    SetArgumentToRealSequence(long cmdNo, const zString argName, double initial, double delta);
	void  SetArgumentToStringSequence(long cmdNo, const zString argName, const zString root);

    // Functions that modify several arguments simultaneously
    void SetArgumentsTo2dTriangularLatticeVector(long xCmdNo, long yCmdNo, const zString xArgName, const zString yArgName,
												long xmax, long ymax, double xorigin, double yorigin, double lx, double ly);
	
    void SetArgumentsTo2dRectangularLatticeVector(long xCmdNo, long yCmdNo, const zString xArgName, const zString yArgName,
                                                  long xmax, long ymax, double xorigin, double yorigin, double lx, double ly);
	void SetArgumentsTo3dRectangularLatticeVector(long xCmdNo, long yCmdNo, long zCmdNo, const zString xArgName, const zString yArgName, const zString zArgName,
                                                  long xmax, long ymax, long zmax, double xorigin, double yorigin, double zorigin, double lx, double ly, double lz);


	// ****************************************
	// Protected local functions
protected:

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

    // Function to create a command instance for execution

    bool InstantiateCommand(unsigned long index);

    // Functions to get/set the status of the group's commands.
    // All commands must be valid before the group can be executed.

    inline bool IsValid()    const {return m_bValid;}

	void SetValid(bool bValid);

    // Functions that check/modify the active status of commands in the group

	bool        IsCommandIndexValid(unsigned long index) const;
    bool            IsCommandActive(unsigned long index) const;
	void      AddCommandToActiveSet(unsigned long index);
	void RemoveCommandFromActiveSet(unsigned long index);

    const zString GetCommandNameFromZeroIndex(unsigned long cmdNo) const;
	const        zString GetHashedCommandName(unsigned long index) const;
    bool                     GetCommandStatus(unsigned long index) const;

    // Functions that manipulate the arguments to the commands

	long GetArgumentTotalForCommand(unsigned long index) const;

	const zString         GetHashedArgumentName(unsigned long index, const zString argName);
    const zString GetArgumentTypeFromHashedName(const zString key) const;

    bool       IsCommandArgumentInMultimap(const zString key, const zString argName) const;
	void      AddCommandArgumentToMultimap(const zString key, const zString argName);
    void RemoveCommandArgumentFromMultimap(const zString key, const zString argName);

    // Overloaded functions to store an argument/value pair in the appropriate typed map
    bool AddArgumentToMap(const zString key, long value);
	bool AddArgumentToMap(const zString key, double value);
	bool AddArgumentToMap(const zString key, const zString value);
    void RemoveArgumentFromTypedMap(const zString key);

    // Argument whose value is copied from another command/argument pair
    bool IsArgumentLinked(const zString key) const;
    bool SetArgumentFromLinkedValue(const zString key);

    // Argument whose value is obtained from a 1d sequence
	bool IsArgumentFromSequence(const zString key) const;
    bool SetArgumentFromSequence(const zString key);

    // Argument whose value is a component of an integer lattice vector
	bool IsArgumentFromLatticeVector(const zString key) const;
	bool SetArgumentFromLatticeVector(const zString key);

    // Argument whose value is a random point in an allowed 1, 2 or 3d space
    bool IsArgumentFromRandomVector(const zString key) const;


    // Functions to add/remove linked arguments from their map
    void AddLinkToArgArgMap(const zString hashedDestArgName, const zString hashedSourceArgName);
	void RemoveLinkFromArgArgMap(const zString hashedDestArgName);

    // Functions to add/remove arguments from the tguArgumentType map
	void AddArgumentToArgumentTypeMap(const zString key, tguArgumentType* pArgumentType);
	void RemoveArgumentFromArgumentTypeMap(const zString key);


	// ****************************************
	// Data members
private:

    const zString      m_Name;    // Name of the command group
    bool               m_bActive; // Flag showing if the command group is active or inactive
    bool               m_bValid;  // Flag showing if all commands in the group are valid
    long               m_Time;    // Execution time of the group
    long               m_Total;   // Number of times to execute in a sequence
    long               m_Period;  // Period of execution times in a sequence

    tguCommands*       m_pCommandSet;            // Set of concrete command instances
    StringSequence     m_Commands;               // Set of names of the command in the group
    zBoolVector        m_CommandStatus;          // Boolean flags showing the active status of commands

    // Containers that hold information on the command arguments

    StringLongMap           m_mIntegerArgumentMap;    // Map of integer-valued arguments
    StringDoubleMap         m_mRealArgumentMap;       // Map of real-valued arguments
    StringStringMap         m_mStringArgumentMap;     // Map of string arguments
    StringStringMap         m_mArgumentTypeMap;       // Map holding (hashed command/argument name, argument type) pairs
    StringStringMap         m_mStringSourceArgMap;    // Map of argument/source argument pairs
    StringArgumentTypeMap   m_mStringVariableArgMap;  // Map of argument/tguArgumentType-derived instances


    StringStringMMap        m_mmCommandArgumentMap;   // Multimap holding (hashed command name, argument placeholder name) pairs
    StringStringMMap        m_mmArgumentArgumentMap;  // Multimap of source argument/destination argument placeholder name pairs

};

#endif // !defined(AFX_TGUCOMMANDGROUP_H__6AEB8958_DFCF_44D1_8A3C_4850A04F9BFB__INCLUDED_)
