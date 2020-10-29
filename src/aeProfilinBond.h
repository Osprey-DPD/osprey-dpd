// aeProfilinBond.h: interface for the aeProfilinBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEPROFILINBOND_H__7E4A45AA_7252_442B_AE89_C12101D72ABB__INCLUDED_)
#define AFX_AEPROFILINBOND_H__7E4A45AA_7252_442B_AE89_C12101D72ABB__INCLUDED_


#include "aeActiveBond.h"

class aeProfilinBond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aeProfilinBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aeProfilinBond();


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

#endif // !defined(AFX_AEPROFILINBOND_H__7E4A45AA_7252_442B_AE89_C12101D72ABB__INCLUDED_)
