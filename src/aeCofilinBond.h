// aeCofilinBond.h: interface for the aeCofilinBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AECOFILINBOND_H__A3905D72_038A_43F0_9832_55E2FDBE01DC__INCLUDED_)
#define AFX_AECOFILINBOND_H__A3905D72_038A_43F0_9832_55E2FDBE01DC__INCLUDED_


#include "aeActiveBond.h"

class aeCofilinBond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aeCofilinBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aeCofilinBond();


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

#endif // !defined(AFX_AECOFILINBOND_H__A3905D72_038A_43F0_9832_55E2FDBE01DC__INCLUDED_)
