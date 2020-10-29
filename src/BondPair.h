// BondPair.h: interface for the CBondPair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONDPAIR_H__1A731793_8ECC_11D4_BF4B_004095086186__INCLUDED_)
#define AFX_BONDPAIR_H__1A731793_8ECC_11D4_BF4B_004095086186__INCLUDED_


// Forward declarations

class CBond;


class CBondPair  
{
public:
	CBondPair();
	CBondPair(long type, double strength, double phi0);
	CBondPair(const CBondPair& oldBondPair);

	virtual ~CBondPair();

	inline long    GetId()			const {return m_id;} 
	inline long    GetType()		const {return m_Type;} 
	inline double  GetModulus()		const {return m_Modulus;}
	inline double  GetPhi0()		const {return m_Phi0;}
	inline long    GetFirstIndex()	const {return m_FirstBondIndex;} 
	inline long    GetSecondIndex()	const {return m_SecondBondIndex;} 
	inline CBond*  GetFirst()		const {return m_pFirst;}
	inline CBond*  GetSecond()		const {return m_pSecond;}

	inline double  GetP2()			const {return m_P2;}
	inline double  GetFirstLength()	const {return m_FirstLength;}
	inline double  GetSecondLength()	const {return m_SecondLength;}
	inline double  GetPhiDiff()		const {return m_PhiDiff;}
	inline double  GetPhiDiffSq()	const {return m_PhiDiffSq;}

	// The following functions return the force on each bead that
	// makes up the triplet in the bondpair. The notation is that the 
	// first bead is labelled 0, the middle bead 1, and the third 2.

	inline double GetXForce(short int i) const {return m_BeadXForce[i];}
	inline double GetYForce(short int i) const {return m_BeadYForce[i];}
	inline double GetZForce(short int i) const {return m_BeadZForce[i];}

	// Access functions to set bondpair data

	inline void SetId(long id) {m_id = id;}
	void SetModulus(double newModulus);
	void SetPhi0(double newPhi0);
	void SetBondIndex(long firstBondIndex, long secondBondIndex);
	void SetBonds(CBond* pFirst, CBond* pSecond);

	// Function to add the 3-body forces to the beads and to calculate
	// the potential energy of the stiff bond

	void	AddForce();
	double	AddPotentialEnergy();

private:

	long m_id;					// Id of bond pair
	long m_Type;				// Integer type of bond pair
	double m_Modulus;	        // Stiffness of bonds
	double m_Phi0;				// Preferred angle

	double m_CosPhi0;			// Trig functions of the preferred angle
	double m_SinPhi0;
	double m_Prefactor;			// Prefactor depending on preferred angle

	long m_FirstBondIndex;		// Index pointing to first bond of pair
	long m_SecondBondIndex;		// Index pointing to second bond of pair
	CBond* m_pFirst;			// Actual CBond* pointers to bonds
	CBond* m_pSecond;

	double m_P2;				// P2(cos(phi-phi0)) observable
	double m_FirstLength;		// First bond length
	double m_SecondLength;		// Second bond length
	double m_PhiDiff;			
	double m_PhiDiffSq;			// Used for ratio of <(phi-phi0)**2)>/<phi-phi0>**2

	double m_BeadXForce[4];
	double m_BeadYForce[4];		// Forces on each bead in the bondpair object
	double m_BeadZForce[4];
};

#endif // !defined(AFX_BONDPAIR_H__1A731793_8ECC_11D4_BF4B_004095086186__INCLUDED_)
