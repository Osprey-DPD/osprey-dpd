// aeBondLigand.h: interface for the aeBondLigand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDLIGAND_H__0b803a6e_e84d_4fda_b74a_e9d1d452d59f__INCLUDED_)
#define AFX_AEBONDLIGAND_H__0b803a6e_e84d_4fda_b74a_e9d1d452d59f__INCLUDED_



#include "aeBondConnection.h"

class aeBondLigand : public aeBondConnection  
{
	// ****************************************
	// Construction/Destruction 
public:

	aeBondLigand(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
				   double k2, double l0);

	virtual ~aeBondLigand();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool AddHeadMonomer(CPolymer* const pHead);
	bool AddHeadMonomer(aeActiveBond* const pHeadBond);
	bool RemoveHeadMonomer(aeActiveBond* const pHeadBond);

	virtual void AddForce();

	
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

	// Bond 

	double m_SpringConstantLinker;
	double m_UnstretchedLengthLinker;

	// CBond objects used by this type of connection to define the link between two active bonds.

	CBond*	const  m_pLinkerHHBond;	// A single Hookean spring linking the head beads of the two monomers within the active bond

};

#endif // !defined(AFX_AEBONDLIGAND_H__0b803a6e_e84d_4fda_b74a_e9d1d452d59f__INCLUDED_)
