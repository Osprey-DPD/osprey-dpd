// pcBLMVesicleFusionSetStretchDuration.h: interface for the pcBLMVesicleFusionSetStretchDuration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETSTRETCHDURATION_H__B1434001_C62B_4D01_B1D4_C6E7A23D2EFA__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETSTRETCHDURATION_H__B1434001_C62B_4D01_B1D4_C6E7A23D2EFA__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetStretchDuration : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetStretchDuration(long executionTime);
	pcBLMVesicleFusionSetStretchDuration(const pcBLMVesicleFusionSetStretchDuration& oldCommand);

	virtual ~pcBLMVesicleFusionSetStretchDuration();
	
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

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETSTRETCHDURATION_H__B1434001_C62B_4D01_B1D4_C6E7A23D2EFA__INCLUDED_)
