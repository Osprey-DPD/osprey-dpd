// aeReceptorBond.h: interface for the aeReceptorBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AERECEPTORBOND_H__F75856E5_83D8_4B62_950D_1A846EEE27A3__INCLUDED_)
#define AFX_AERECEPTORBOND_H__F75856E5_83D8_4B62_950D_1A846EEE27A3__INCLUDED_



#include "aeActiveBond.h"

class aeReceptorBond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aeReceptorBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aeReceptorBond();


	// ****************************************
	// Global functions, static member functions and variables
public:


private:

	static long	m_ActiveBondsPerPolymer;

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual long GetActiveBondsPerPolymer() const;
	virtual bool Activate(aeActiveBond* const pTargetBond);
	virtual bool Deactivate();

	// ****************************************
	// Public access functions
public:

    void SetFixedHeadOffRate(double rate);
	void SetFixedTailOffRate(double rate);

	void SetOnRateSeparation(double max);
	void SetOffRateSeparation(double max);
	void SetHeadOnRateSeparation(double max);
	void SetHeadOffRateSeparation(double max);
	void SetTailOnRateSeparation(double max);
	void SetTailOffRateSeparation(double max);


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

	// Bond parameters

	double m_SpringConstant;
	double m_UnstretchedLength;

	double m_BendingModulus;
	double m_Phi0;

};

#endif // !defined(AFX_AERECEPTORBOND_H__F75856E5_83D8_4B62_950D_1A846EEE27A3__INCLUDED_)
