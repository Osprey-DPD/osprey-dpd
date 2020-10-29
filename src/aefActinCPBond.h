// aefActinCPBond.h: interface for the aefActinCPBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEFACTINCPBOND_H__E34E6607_72A9_45F8_8724_1A54E7F1B095__INCLUDED_)
#define AFX_AEFACTINCPBOND_H__E34E6607_72A9_45F8_8724_1A54E7F1B095__INCLUDED_


#include "aeActiveBond.h"

class aefActinCPBond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aefActinCPBond(CPolymer* const pTail, double k2, double l0, double k3, double phi0);
	virtual ~aefActinCPBond();


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

#endif // !defined(AFX_AEFACTINCPBOND_H__E34E6607_72A9_45F8_8724_1A54E7F1B095__INCLUDED_)
