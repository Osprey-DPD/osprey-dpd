// aeBondInline.h: interface for the aeBondInline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDINLINE_H__B72D747E_2A9A_4904_AFAB_3D7740958195__INCLUDED_)
#define AFX_AEBONDINLINE_H__B72D747E_2A9A_4904_AFAB_3D7740958195__INCLUDED_



#include "aeBondConnection.h"

class aeBondInline : public aeBondConnection  
{
	// ****************************************
	// Construction/Destruction 
public:

	aeBondInline(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
				 double k2, double l0, double k3, double phi0);

	virtual ~aeBondInline();


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

	// Bond parameters

	double m_SpringConstant;
	double m_UnstretchedLength;

	double m_BendingModulus;
	double m_Phi0;

	// CBond and CBondPair objects used by this type of connection to define
	// the link between two active bonds.

	CBond*		const	  m_pInternalBond;			// Links the head and tail beads within the active bond
	CBond*		const	  m_pLinkerBond;			// Links the active bond to the next bond
	CBond*		const	  m_pHeadInternalBond;		// Links the head and tail beads within the next active bond
	CBondPair*  const	  m_pInternalBondPair;		// Adds bending stiffness within the active bond
	CBondPair*  const	  m_pLinkerBondPair;		// Adds bending stiffness between the active bonds


};

#endif // !defined(AFX_AEBONDINLINE_H__B72D747E_2A9A_4904_AFAB_3D7740958195__INCLUDED_)
