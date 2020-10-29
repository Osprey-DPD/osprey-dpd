// aeBondTriangle.h: interface for the aeBondTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDTRIANGLE_H__345941A6_4AE3_41EE_8992_11D4038DAF50__INCLUDED_)
#define AFX_AEBONDTRIANGLE_H__345941A6_4AE3_41EE_8992_11D4038DAF50__INCLUDED_



#include "aeBondConnection.h"

class aeBondTriangle : public aeBondConnection  
{
	// ****************************************
	// Construction/Destruction 
public:

	aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
				   double k2, double l0);

	aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
				   double k2internal, double l0internal, double k2linker, double l0linker);

	aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
				   double k2internal, double l0internal, double k2linker, double l0linker,
				   double k3linker, double phi0linker);

	virtual ~aeBondTriangle();


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
	virtual void AddThreeBodyForce();

	
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

	// Bond and bondpair parameters

	double m_SpringConstantInternal;
	double m_UnstretchedLengthInternal;

	double m_SpringConstantLinker;
	double m_UnstretchedLengthLinker;

	double m_UnstretchedLengthCrossLinker; // HM, HT bonds used for stiffness

	double m_BendingModulusLinker;
	double m_Phi0Linker;

	// CBond objects used by this type of connection to define
	// the link between two active bonds.

	CBond*		const	  m_pInternalHTBond;	// Links the head and tail beads within the active bond
	CBond*		const	  m_pInternalHMBond;	// Links the head and middle beads within the active bond
	CBond*		const	  m_pInternalMTBond;	// Links the middle and tail beads within the active bond

	CBond*		const	  m_pLinkerHHBond;		// Links the head beads between tail and head active bonds
	CBond*		const	  m_pLinkerMMBond;		// Links the middle beads between tail and head active bonds
	CBond*		const	  m_pLinkerTTBond;		// Links the tail beads between tail and head active bonds
	CBond*		const	  m_pLinkerHMBond;		// Links the previous head bead and next middle bead
	CBond*		const	  m_pLinkerHTBond;		// Links the previous head bead and next tail bead
	CBond*		const	  m_pLinkerTMBond;		// Links the previous tail bead and next middle bead
	CBond*		const	  m_pLinkerTHBond;		// Links the previous tail bead and next head bead
	CBond*		const	  m_pLinkerMTBond;		// Links the previous middle bead and next tail bead
	CBond*		const	  m_pLinkerMHBond;		// Links the previous middle bead and next head bead

	// Bondpairs linking adjacent triples of monomers

	CBondPair*  const	  m_pLinkerHHBondPair;	// Links monomers via stiff bonds
	CBondPair*  const	  m_pLinkerMMBondPair;	// Links monomers via stiff bonds
	CBondPair*  const	  m_pLinkerTTBondPair;	// Links monomers via stiff bonds

};

#endif // !defined(AFX_AEBONDTRIANGLE_H__345941A6_4AE3_41EE_8992_11D4038DAF50__INCLUDED_)
