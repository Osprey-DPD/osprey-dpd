// cgtExecuteCommandGroupSequence.h: interface for the cgtExecuteCommandGroupSequence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTEXECUTECOMMANDGROUPSEQUENCE_H__D2DCBB24_AFAD_4D11_93D7_F14573DE7383__INCLUDED_)
#define AFX_CGTEXECUTECOMMANDGROUPSEQUENCE_H__D2DCBB24_AFAD_4D11_93D7_F14573DE7383__INCLUDED_



#include "xxCommand.h"

class cgtExecuteCommandGroupSequence : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	cgtExecuteCommandGroupSequence(long executionTime);
	cgtExecuteCommandGroupSequence(const cgtExecuteCommandGroupSequence& oldCommand);

	cgtExecuteCommandGroupSequence(long executionTime, const zString name,
                                   long total, long period);

	virtual ~cgtExecuteCommandGroupSequence();
	
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
	inline long          GetTotal()	            const {return m_Total;};
	inline long          GetPeriod()	        const {return m_Period;};

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
    long    m_Total;    // Number of times to execute the group
    long    m_Period;   // Number of time steps between executions of the group

};

#endif // !defined(AFX_CGTEXECUTECOMMANDGROUPSEQUENCE_H__D2DCBB24_AFAD_4D11_93D7_F14573DE7383__INCLUDED_)
