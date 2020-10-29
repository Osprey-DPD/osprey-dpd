// ILightField.h: interface for the ILightField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILIGHTFIELD_H__9E70C872_1B39_472F_9DC2_E9B90B3C4C44__INCLUDED_)
#define AFX_ILIGHTFIELD_H__9E70C872_1B39_472F_9DC2_E9B90B3C4C44__INCLUDED_


#include "IProbeField.h"

class ILightField : public IProbeField  
{
public:
	// ****************************************
	// Construction/Destruction

	// Protected constructor prevents instantiation of this class
public:

	virtual ~ILightField();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// Public access functions
public:

	virtual double GetLightSpeed() const = 0;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void UpdateState() = 0;

	virtual void RegisterBeadDensityGrid() = 0;

	virtual void RegisterScatteredFieldGrid() = 0;

	// ****************************************
	// Protected local functions
protected:

	ILightField();



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

private:



};

#endif // !defined(AFX_ILIGHTFIELD_H__9E70C872_1B39_472F_9DC2_E9B90B3C4C44__INCLUDED_)
