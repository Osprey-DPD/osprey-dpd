// mcToggleSliceEnergyOutput.h: interface for the mcToggleSliceEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLESLICEENERGYOUTPUT_H__C69168BD_8265_4D1C_A525_489ED554B146__INCLUDED_)
#define AFX_MCTOGGLESLICEENERGYOUTPUT_H__C69168BD_8265_4D1C_A525_489ED554B146__INCLUDED_


#include "xxCommand.h"

class mcToggleSliceEnergyOutput : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction

	mcToggleSliceEnergyOutput(long executionTime);
	mcToggleSliceEnergyOutput(const mcToggleSliceEnergyOutput& oldCommand);

	virtual ~mcToggleSliceEnergyOutput();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of command

private:

	static const zString m_Type;	// Identifier used in control data file for command

	// Note that there are no access functions to get the m_CommandCounter or
	// the m_CurrentId membrane variables as xxCommand already defines a 
	// GetId() function and m_id. The variables defined below are implementation 
	// details to allow toggling of the state of the mcToggleSliceEnergyOutput 
	// commands. We keep track of how many slice energy commands have been created
	// using a static counter.

	static long	m_CommandCounter;

	// ****************************************
	// Public access functions
public:

	long	GetXNormal() const;
	long	GetYNormal() const;
	long	GetZNormal() const;
	double	GetLowerFractionCoord() const;
	double	GetUpperFractionCoord() const;
	bool	NormalizePerBead() const;
	long	GetToggleId() const;

	// ****************************************
	// PVFs that must be overridden by all derived classes

public:
	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimCmd* const pISimCmd) const;

	virtual const xxCommand* GetCommand() const;

	virtual bool IsDataValid(const CInputData& riData) const;

protected:
	virtual const zString GetCommandType() const;


	// ****************************************
	// Data members
private:

	bool	m_bNormalizePerBead;	// Flag showing whether U or U/N is output

	const long	m_CurrentId;		// Id of current command

	long	m_ToggleId;				// Id of toggle command target: 0 = create new command

	long	m_X;					// Normal defining slice orientation
	long	m_Y;					// Must be one of the major axes
	long	m_Z;

	double	m_LowerFractionCoord;	// Lower slice edge as a fraction of SimBox size
	double	m_UpperFractionCoord;	// Upper slice edge as a fraction of SimBox size

};

#endif // !defined(AFX_MCTOGGLESLICEENERGYOUTPUT_H__C69168BD_8265_4D1C_A525_489ED554B146__INCLUDED_)
