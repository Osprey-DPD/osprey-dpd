// cgtCreateCommandGroup.h: interface for the cgtCreateCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTCREATECOMMANDGROUP_H__E9085F69_02B1_4863_A965_5C3260E250FD__INCLUDED_)
#define AFX_CGTCREATECOMMANDGROUP_H__E9085F69_02B1_4863_A965_5C3260E250FD__INCLUDED_



#include "xxCommand.h"

class cgtCreateCommandGroup : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgtCreateCommandGroup(long executionTime);
	cgtCreateCommandGroup(const cgtCreateCommandGroup& oldCommand);

	cgtCreateCommandGroup(long executionTime, const zString name);

	virtual ~cgtCreateCommandGroup();
	
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
	
	inline const zString GetCommandGroupName()	const {return m_Name;};

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

	zString m_Name;		// Command group name

};

#endif // !defined(AFX_CGTCREATECOMMANDGROUP_H__E9085F69_02B1_4863_A965_5C3260E250FD__INCLUDED_)
