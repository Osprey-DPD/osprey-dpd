// IModifyReceptorNetwork.h: interface for the IModifyReceptorNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYRECEPTORNETWORK_H__62e2dc8a_797d_4e8e_98d6_28fe252c7390__INCLUDED_)
#define AFX_IMODIFYRECEPTORNETWORK_H__62e2dc8a_797d_4e8e_98d6_28fe252c7390__INCLUDED_



#include "IModifyActiveCellNetwork.h"

class IModifyReceptorNetwork : public IModifyActiveCellNetwork
{
	// ****************************************
	// Construction/Destruction: ctor below is needed to pass 
	// data to the base class.
public:

	virtual ~IModifyReceptorNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void ActivateReceptorNucleation()				= 0;
	virtual void DisassembleReceptorFilaments()				= 0;
	virtual void InactivateReceptorNucleation()				= 0;

    virtual void SetFixedHeadOffRate(double rate)           = 0;
    virtual void SetFixedTailOffRate(double rate)           = 0;

	virtual void SetProximityOnSeparation(double max)		= 0;
	virtual void SetProximityOffSeparation(double max)		= 0;
	virtual void SetProximityHeadOnSeparation(double max)   = 0;
	virtual void SetProximityHeadOffSeparation(double max)  = 0;
	virtual void SetProximityTailOnSeparation(double max)   = 0;
	virtual void SetProximityTailOffSeparation(double max)  = 0;

	virtual void SeverReceptorFilaments()					= 0;


	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	IModifyReceptorNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn, double xw, double yw, double zw);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members: No member variables as this is an interface class
private:


};

#endif // !defined(AFX_IMODIFYRECEPTORNETWORK_H__62e2dc8a_797d_4e8e_98d6_28fe252c7390__INCLUDED_)
