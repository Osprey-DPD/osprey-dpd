// ccExtendTotalTime.h: interface for the ccExtendTotalTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCEXTENDTOTALTIME_H__FAC78621_A692_472C_988A_E0A0BDCCD4FD__INCLUDED_)
#define AFX_CCEXTENDTOTALTIME_H__FAC78621_A692_472C_988A_E0A0BDCCD4FD__INCLUDED_


#include "xxCommand.h"

class ccExtendTotalTime : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	ccExtendTotalTime(long executionTime);
	ccExtendTotalTime(const ccExtendTotalTime& oldCommand);

	virtual ~ccExtendTotalTime();
	
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

	inline long GetExtraTime() const {return m_ExtraTime;}

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

	long  m_ExtraTime;		// Number of time steps to extend simulation
};

#endif // !defined(AFX_CCEXTENDTOTALTIME_H__FAC78621_A692_472C_988A_E0A0BDCCD4FD__INCLUDED_)
