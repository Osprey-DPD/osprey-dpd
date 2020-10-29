// IModifyForminNetwork.h: interface for the IModifyForminNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYFORMINNETWORK_H__095E1D61_70CF_4EAC_8891_A8E5B59C7067__INCLUDED_)
#define AFX_IMODIFYFORMINNETWORK_H__095E1D61_70CF_4EAC_8891_A8E5B59C7067__INCLUDED_


#include "IModifyActiveCellNetwork.h"

class IModifyForminNetwork : public IModifyActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyForminNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void ActivateForminNucleation()					= 0;
	virtual void InactivateForminNucleation()				= 0;
	virtual void DisassembleForminFilaments()				= 0;
	virtual void SeverForminFilaments()						= 0;

    virtual void SetFixedHeadOffRate(double rate)           = 0;
    virtual void SetFixedTailOffRate(double rate)           = 0;

	virtual void SetProximityOnSeparation(double max)		= 0;
	virtual void SetProximityOffSeparation(double max)		= 0;
	virtual void SetProximityHeadOnSeparation(double max)   = 0;
	virtual void SetProximityHeadOffSeparation(double max)  = 0;
	virtual void SetProximityTailOnSeparation(double max)   = 0;
	virtual void SetProximityTailOffSeparation(double max)  = 0;



	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyForminNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn, double xw, double yw, double zw);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IMODIFYFORMINNETWORK_H__095E1D61_70CF_4EAC_8891_A8E5B59C7067__INCLUDED_)
