// acSetEventExecutionPeriod.h: interface for the acSetEventExecutionPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSETEVENTEXECUTIONPERIOD_H__F1B6853B_4D3A_4F72_AFC6_936129D14C73__INCLUDED_)
#define AFX_ACSETEVENTEXECUTIONPERIOD_H__F1B6853B_4D3A_4F72_AFC6_936129D14C73__INCLUDED_


#include "xxCommand.h" 

class acSetEventExecutionPeriod : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	acSetEventExecutionPeriod(long executionTime);
	acSetEventExecutionPeriod(const acSetEventExecutionPeriod& oldCommand);

	virtual ~acSetEventExecutionPeriod();
	
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

	inline const zString GetACNType()   const {return m_ACNType;}
	inline const zString GetEventType() const {return m_EventType;}

	inline long GetPeriod() const {return m_Period;}

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

	zString m_ACNType;   // Name of ACN containing the event to modify
	zString m_EventType; // Name of event to modify

	long m_Period;	     // Period with which event's execution is checked

};

#endif // !defined(AFX_ACSETEVENTEXECUTIONPERIOD_H__F1B6853B_4D3A_4F72_AFC6_936129D14C73__INCLUDED_)
