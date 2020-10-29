// aeArp23Bond.h: interface for the aeArp23Bond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEARP23BOND_H__B7C58AAC_CA10_407F_B5B3_382D7E3B9329__INCLUDED_)
#define AFX_AEARP23BOND_H__B7C58AAC_CA10_407F_B5B3_382D7E3B9329__INCLUDED_


#include "aeActiveBond.h"

class aeArp23Bond : public aeActiveBond  
{
	// ****************************************
	// Construction/Destruction
public:

	aeArp23Bond(CPolymer* const pTail);
	virtual ~aeArp23Bond();


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


};

#endif // !defined(AFX_AEARP23BOND_H__B7C58AAC_CA10_407F_B5B3_382D7E3B9329__INCLUDED_)
