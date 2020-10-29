// mcSetCurrentStateCamera.h: interface for the mcSetCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATECAMERA_H__031B8714_0ABC_4C25_8F21_3F9589D13AAF__INCLUDED_)
#define AFX_MCSETCURRENTSTATECAMERA_H__031B8714_0ABC_4C25_8F21_3F9589D13AAF__INCLUDED_


// Forward declarations

class ISimCmd;


#include "xxCommand.h"

class mcSetCurrentStateCamera : public xxCommand  
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetCurrentStateCamera(long executionTime);
	mcSetCurrentStateCamera(const mcSetCurrentStateCamera& oldCommand);

	// Constructor for use when executing the command internally

	mcSetCurrentStateCamera(long executionTime, bool bLog, double camera[3], double target[3]);

	virtual ~mcSetCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables


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
	// Public access functions
public:

	inline double	GetCamera(short int i) const {return m_Camera[i];}
	inline double	GetTarget(short int i) const {return m_Target[i];}

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

	double	m_Camera[3];			// Camera coordinates as multiple of SimBox size
	double	m_Target[3];			// Target (look at) coordinates        "
};

#endif // !defined(AFX_MCSETCURRENTSTATECAMERA_H__031B8714_0ABC_4C25_8F21_3F9589D13AAF__INCLUDED_)
