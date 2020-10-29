// mcSetCurrentStateLight.h: interface for the mcSetCurrentStateLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATELIGHT_H__363DE315_A7ED_4A32_A71E_C222E4EDE724__INCLUDED_)
#define AFX_MCSETCURRENTSTATELIGHT_H__363DE315_A7ED_4A32_A71E_C222E4EDE724__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetCurrentStateLight : public xxCommand    
{
public:
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:
	mcSetCurrentStateLight(long executionTime);
	mcSetCurrentStateLight(const mcSetCurrentStateLight& oldCommand);

    // Constructor for use when executing the command internally
    
    mcSetCurrentStateLight(long executionTime, bool bLog, double light[3]);

	virtual ~mcSetCurrentStateLight();

	// ****************************************
	// Global functions, static member functions and variables

	// ****************************************
	// Public access functions
public:

	inline double	GetLight(short int i) const {return m_Light[i];}

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const zString GetCommandType() const;

	static const zString GetType();	// Return the type of command

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	static const zString m_Type;	// Identifier used in control data file for command

	double	m_Light[3];				// Light coordinates as multiple of SimBox size
};

#endif // !defined(AFX_MCSETCURRENTSTATELIGHT_H__363DE315_A7ED_4A32_A71E_C222E4EDE724__INCLUDED_)
