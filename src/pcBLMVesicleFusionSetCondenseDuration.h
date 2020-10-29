// pcBLMVesicleFusionSetCondenseDuration.h: interface for the pcBLMVesicleFusionSetCondenseDuration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETCONDENSEDURATION_H__E1C7F55C_A27D_44AE_9987_6C9FCC782E83__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETCONDENSEDURATION_H__E1C7F55C_A27D_44AE_9987_6C9FCC782E83__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetCondenseDuration : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetCondenseDuration(long executionTime);
	pcBLMVesicleFusionSetCondenseDuration(const pcBLMVesicleFusionSetCondenseDuration& oldCommand);

	virtual ~pcBLMVesicleFusionSetCondenseDuration();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

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

	// ****************************************
	// Public access functions
public:

	inline const zString GetPid()      const {return m_pid;}
	inline long		     GetDuration() const {return m_Duration;}

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

	zString	 m_pid;			// Process id that uniquely identifies each process
	long	 m_Duration;	// New value for duration of stretching force 

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETCONDENSEDURATION_H__E1C7F55C_A27D_44AE_9987_6C9FCC782E83__INCLUDED_)
