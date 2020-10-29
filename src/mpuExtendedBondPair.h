// mpuExtendedBondPair.h: interface for the mpuExtendedBondPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUEXTENDEDBONDPAIR_H__5FB9A277_336E_4DD5_A6BF_8EF36A755773__INCLUDED_)
#define AFX_MPUEXTENDEDBONDPAIR_H__5FB9A277_336E_4DD5_A6BF_8EF36A755773__INCLUDED_


// Forward declarations

class CBond;
class CBondPair;
class mpuExtendedBond;


class mpuExtendedBondPair  
{
	// Friend classes need access to bond data in order to
	// calculate forces between bonded beads and observables

//	friend class CMonitor;

	// ****************************************
	// Construction/Destruction
public:

	mpuExtendedBondPair();
	mpuExtendedBondPair(CBondPair* pBondPair, mpuExtendedBond* pBond1, mpuExtendedBond* pBond2);

	mpuExtendedBondPair(const mpuExtendedBondPair& oldBondPair);
	~mpuExtendedBondPair();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	// Function to return the wrapped stiff bond instance
	inline CBondPair* GetBondPair() const {return m_pBondPair;}

	// Functions to return the bond instances belonging to the stiff bond
	inline mpuExtendedBond* GetFirstBond()  const {return m_pBond1;}
	inline mpuExtendedBond* GetSecondBond() const {return m_pBond2;}


	// Forwarding functions to the wrapped bond instance

	long   GetId()         const;
	long   GetType()       const;
    double GetModulus()    const;
	double GetPhi0()       const;

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

	CBondPair*        m_pBondPair;  // Pointer to the wrapped stiff bond instance
	mpuExtendedBond*  m_pBond1;     // Pointer to first extended bond instance
	mpuExtendedBond*  m_pBond2;     // Pointer to second extended bond instance

	long   m_id;		// Id of bond pair
	long   m_Type;		// Integer type of bond pair
	double m_Modulus;	// Stiffness of bonds
	double m_Phi0;	    // Preferred angle


};

#endif // !defined(AFX_MPUEXTENDEDBONDPAIR_H__5FB9A277_336E_4DD5_A6BF_8EF36A755773__INCLUDED_)
