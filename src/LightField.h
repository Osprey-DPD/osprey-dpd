// LightField.h: interface for the CLightField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTFIELD_H__324EAB2D_94CB_46BF_8880_CB09A9D9BBE5__INCLUDED_)
#define AFX_LIGHTFIELD_H__324EAB2D_94CB_46BF_8880_CB09A9D9BBE5__INCLUDED_


#include "ILightField.h"

class CLightField : public ILightField  
{
	// ****************************************
	// Construction/Destruction

public:

	CLightField();
	virtual ~CLightField();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// Public access functions
public:

	inline virtual double GetLightSpeed() const {return m_c;}


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function called by a prProbeFieldProcess to calculate the new 
	// state of the radiation field given a new state of the SimBox 
	// and the field data contained in the derived class.

	void UpdateState();

	// Handler function used by a prProbeFieldProcess to register the 
	// density grid from the SimBox and use it to calculate 
	// the interactions of a radiation field with the SimBox.

	void RegisterBeadDensityGrid();

	// Handler function used by an IDetectorGrid-derived class to register 
	// the scattered field grid calculated by an IProbeField-derived class.
	// This allows it to integrate the scattered radiation field across
	// the SimBox with a specific pattern of detectors to output the 
	// appropriate output signals.

	void RegisterScatteredFieldGrid();


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

private:

	const double	m_c;	// Speed of light	(m/sec)

	bool	m_bPolarised;	// Is radiation polarised?

	double  m_Wavelength;	// Wavelength of radiation (nm)
	double	m_Spread;		// Range of wavelengths present (half-width in nm)
	double	m_Source[3];	// Coordinates of radiation source in SimBox
	double  m_PolAxis[3];	// Polarisation axis unit vector

	// Local data derived from input data

	double	m_Frequency;	// Frequency of radiation (1/sec)

};

#endif // !defined(AFX_LIGHTFIELD_H__324EAB2D_94CB_46BF_8880_CB09A9D9BBE5__INCLUDED_)
