// IProbeField.h: interface for the IProbeField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPROBEFIELD_H__1EB407FB_25E0_459F_9F69_1EED72AE2EB0__INCLUDED_)
#define AFX_IPROBEFIELD_H__1EB407FB_25E0_459F_9F69_1EED72AE2EB0__INCLUDED_


class IProbeField  
{
public:
	// ****************************************
	// Construction/Destruction

	// Protected constructor prevents instantiation of this class
public:

	virtual ~IProbeField();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function called by a prProbeFieldProcess to calculate the new 
	// state of the radiation field given a new state of the SimBox 
	// and the field data contained in the derived class.

	virtual void UpdateState() = 0;

	// Handler function used by a prProbeFieldProcess to register the 
	// density grid from the SimBox and use it to calculate 
	// the interactions of a radiation field with the SimBox.

	virtual void RegisterBeadDensityGrid() = 0;

	// Handler function used by an IDetectorGrid-derived class to register 
	// the scattered field grid calculated by an IProbeField-derived class.
	// This allows it to integrate the scattered radiation field across
	// the SimBox with a specific pattern of detectors to output the 
	// appropriate output signals.

	virtual void RegisterScatteredFieldGrid() = 0;

	// ****************************************
	// Protected local functions
protected:

	IProbeField();



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

private:

};

#endif // !defined(AFX_IPROBEFIELD_H__1EB407FB_25E0_459F_9F69_1EED72AE2EB0__INCLUDED_)
