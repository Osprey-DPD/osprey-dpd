// pcBLMVesicleFusionSetMaxCMApproach.h: interface for the pcBLMVesicleFusionSetMaxCMApproach class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETMAXCMAPPROACH_H__90F351FD_5939_471B_B3C1_72A9F68E58C1__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETMAXCMAPPROACH_H__90F351FD_5939_471B_B3C1_72A9F68E58C1__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetMaxCMApproach : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetMaxCMApproach(long executionTime);
	pcBLMVesicleFusionSetMaxCMApproach(const pcBLMVesicleFusionSetMaxCMApproach& oldCommand);

	virtual ~pcBLMVesicleFusionSetMaxCMApproach();
	
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

	inline const    zString GetPid() const {return m_pid;}
	inline double GetMaxCMApproach() const {return m_MaxCMApproach;}

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

	zString	 m_pid;				// Process id that uniquely identifies each process
	double	 m_MaxCMApproach;	// New distance at which vesicle is "close" to bilayer

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETMAXCMAPPROACH_H__90F351FD_5939_471B_B3C1_72A9F68E58C1__INCLUDED_)
