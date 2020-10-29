// IModifyfActinNetwork.h: interface for the IModifyfActinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYFACTINNETWORK_H__1FE9D978_052A_429E_A4F9_2CEBF8CE7935__INCLUDED_)
#define AFX_IMODIFYFACTINNETWORK_H__1FE9D978_052A_429E_A4F9_2CEBF8CE7935__INCLUDED_


#include "IModifyActiveCellNetwork.h"

class IModifyfActinNetwork : public IModifyActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyfActinNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    virtual void SetFixedHeadOffRate(double rate)           = 0;
    virtual void SetFixedTailOffRate(double rate)           = 0;

	virtual void SetProximityOnSeparation(double max)		= 0;
	virtual void SetProximityOffSeparation(double max)		= 0;
	virtual void SetProximityHeadOnSeparation(double max)   = 0;
	virtual void SetProximityHeadOffSeparation(double max)  = 0;
	virtual void SetProximityTailOnSeparation(double max)   = 0;
	virtual void SetProximityTailOffSeparation(double max)  = 0;

    virtual void SetATPHydrolysisProbability(double rate)      = 0;
    virtual void SetADPReleasePiProbability(double rate)       = 0;
    virtual void SetADPPhosphorylationProbability(double rate) = 0;

	virtual void SetHeadADPPiMultiplier(double factor) = 0;
    virtual	void SetTailADPPiMultiplier(double factor) = 0;
	virtual void SetHeadADPMultiplier(double factor)   = 0;
	virtual void SetTailADPMultiplier(double factor)   = 0;

    virtual void ColourBondsByATPState(long ATPId, long ADPPiId, long ADPId) = 0;


	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyfActinNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn, double xw, double yw, double zw);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IMODIFYFACTINNETWORK_H__1FE9D978_052A_429E_A4F9_2CEBF8CE7935__INCLUDED_)
