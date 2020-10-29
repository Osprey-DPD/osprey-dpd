// pcBLMVesicleFusionSetRadialForce.h: interface for the pcBLMVesicleFusionSetRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBLMVESICLEFUSIONSETRADIALFORCE_H__1ADBCF88_E52B_466D_96B3_F48298CCDAC5__INCLUDED_)
#define AFX_PCBLMVESICLEFUSIONSETRADIALFORCE_H__1ADBCF88_E52B_466D_96B3_F48298CCDAC5__INCLUDED_


#include "xxCommand.h"

class pcBLMVesicleFusionSetRadialForce : public xxCommand  
{
	// ****************************************
	// Construction/Destruction: base class has protected constructor
public:

	pcBLMVesicleFusionSetRadialForce(long executionTime);
	pcBLMVesicleFusionSetRadialForce(const pcBLMVesicleFusionSetRadialForce& oldCommand);

	virtual ~pcBLMVesicleFusionSetRadialForce();
	
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

	inline const zString   GetPid() const {return m_pid;}
	inline double  GetRadialForce() const {return m_RadialForce;}

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
	double	 m_RadialForce;	// New magnitude for the radial stretching force

};

#endif // !defined(AFX_PCBLMVESICLEFUSIONSETRADIALFORCE_H__1ADBCF88_E52B_466D_96B3_F48298CCDAC5__INCLUDED_)
