// aeForminBond.h: interface for the aeForminBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFORMINBOND_H__BBA2F51D_EECA_481F_AC30_547CF5B74454__INCLUDED_)
#define AFX_AEFORMINBOND_H__BBA2F51D_EECA_481F_AC30_547CF5B74454__INCLUDED_



#include "aeActiveBond.h"

class aeForminBond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aeForminBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aeForminBond();


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

#endif // !defined(AFX_AEFORMINBOND_H__BBA2F51D_EECA_481F_AC30_547CF5B74454__INCLUDED_)
