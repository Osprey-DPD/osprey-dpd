// prLightField.h: interface for the prLightField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRLIGHTFIELD_H__B8208BC2_AE39_4699_BADB_0E36298CBB93__INCLUDED_)
#define AFX_PRLIGHTFIELD_H__B8208BC2_AE39_4699_BADB_0E36298CBB93__INCLUDED_


#include "prProbeFieldProcess.h"

class prLightField : public prProbeFieldProcess  
{
	// ****************************************
	// Construction/Destruction
public:

	prLightField();
	virtual ~prLightField();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:	


	// ****************************************
	// Data members

private:						// Data specific to the process

	bool	m_bPolarised;	// Is radiation polarised?

	double  m_Wavelength;	// Wavelength of radiation (nm)
	double	m_Spread;		// Range of wavelengths present (half-width in nm)
	double	m_Source[3];	// Coordinates of radiation source in SimBox
	double  m_PolAxis[3];	// Polarisation axis unit vector

	// Absorption/scattering data for the SimBox


};

#endif // !defined(AFX_PRLIGHTFIELD_H__B8208BC2_AE39_4699_BADB_0E36298CBB93__INCLUDED_)
