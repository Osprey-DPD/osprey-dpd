// pcBLMVesicleFusionSetMinMixThreshold.h: interface for the pcBLMVesicleFusionSetMinMixThreshold class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETMINMIXTHRESHOLD_H__3161D45B_6C55_4470_91F7_1BC868F3316E__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETMINMIXTHRESHOLD_H__3161D45B_6C55_4470_91F7_1BC868F3316E__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetMinMixThreshold : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetMinMixThreshold(long executionTime);
	pcBLMVesicleFusionSetMinMixThreshold(const pcBLMVesicleFusionSetMinMixThreshold& oldCommand);

	virtual ~pcBLMVesicleFusionSetMinMixThreshold();
	
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

	inline const     zString GetPid() const {return m_pid;}
	inline long GetMinMixThreshold()  const {return m_MinMixThreshold;}

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
	long	 m_MinMixThreshold;	// Minimum number of amphiphiles to indicate out leaflet mixing

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETMINMIXTHRESHOLD_H__3161D45B_6C55_4470_91F7_1BC868F3316E__INCLUDED_)
