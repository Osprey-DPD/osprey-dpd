// pcBLMVesicleFusionSetStretchDelay.h: interface for the pcBLMVesicleFusionSetStretchDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETSTRETCHDELAY_H__326157CF_B16C_4007_B8A5_179AD23C5512__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETSTRETCHDELAY_H__326157CF_B16C_4007_B8A5_179AD23C5512__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetStretchDelay : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetStretchDelay(long executionTime);
	pcBLMVesicleFusionSetStretchDelay(const pcBLMVesicleFusionSetStretchDelay& oldCommand);

	virtual ~pcBLMVesicleFusionSetStretchDelay();
	
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

	inline const zString GetPid()   const {return m_pid;}
	inline long		     GetDelay() const {return m_Delay;}

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

	zString	 m_pid;		// Process id that uniquely identifies each process
	long	 m_Delay;	// New value of stretch delay

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETSTRETCHDELAY_H__326157CF_B16C_4007_B8A5_179AD23C5512__INCLUDED_)
