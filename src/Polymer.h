// Polymer.h: interface for the CPolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMER_H__A2FACF41_3F61_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_POLYMER_H__A2FACF41_3F61_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class ISimBoxBase;
class mpuExtendedPolymer;


// Include file to gain access to the typedefs for beads and bonds

#include "xxBase.h"
#include "aaVector.h"
#include "SimMPSFlags.h"



class CPolymer  
{
    friend class mpuExtendedPolymer;  // Needed to allow extended polymers to modify their wrapped polymer instances
    friend class mpsSimBox;
	
	// ****************************************
	// Construction/Destruction: 
public:

	CPolymer();

	CPolymer(long type, bool blinear, double fraction, CBead* pHead, CBead* pTail, 
			 BeadVector& vBeads, BondVector& vBonds);

	CPolymer(long type, bool blinear, double fraction, CBead* pHead, CBead* pTail, 
			 BeadVector& vBeads, BondVector& vBonds, BondPairVector& vBondPairs);

	~CPolymer();

	CPolymer(const CPolymer& oldPolymer);
	CPolymer& operator =(const CPolymer &oldPolymer);

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator <( const CPolymer &a, const CPolymer &b);
	friend bool operator ==(const CPolymer &a, const CPolymer &b);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	inline long   GetId()         const {return m_id;}
	inline long   GetType()       const {return m_Type;}
	inline long   GetSize()       const {return m_vBeads.size();}

	inline CAbstractBead*    GetHead() const	 {return m_pHead;}
	inline CAbstractBead*    GetTail() const	 {return m_pTail;}
	inline BeadVector& GetBeads()			     {return m_vBeads;}
	inline BondVector& GetBonds()			     {return m_vBonds;}
	inline BondPairVector& GetBondPairs()        {return m_vBondPairs;}

	// Function to return a third bead in the polymer apart from its Head and Tail

	CAbstractBead* GetHeadAdjacentBead() const;
	
	// Function to return the zero-indexed location of a bead in the polymer counting from the first bead in the polymer's shape
	
	long GetHeadIndex() const;
	long GetTailIndex() const;
	long GetBeadLocation(const CAbstractBead* const pBead) const;
    long GetFirstBondId() const;
	long GetFirstBondPairId() const;

    // Function to return the CM coordinates of the polymer

    aaVector GetCM() const;

	inline void SetId(long id)     {m_id = id;}
	inline void SetType(long type) {m_Type = type;}

	long SetBeadIds(long startId);
	long SetBondIds(long startId);
	long SetBondPairIds(long startId);
	void ChangeHeadtoWallBead();


	// Functions to set the visibility of the polymer in current state snapshots

    bool GetVisible() const;
	void SetVisible(bool bVisible);
	void SetVisible();
	void SetInvisible();
	void SetDisplayId(long id);


	// Functions to calculate the internal forces due to the polymer's bonds
	// and stiff bonds. The first override just calculates the forces while
	// the second also sends the bond stresses to the Monitor for the stress profile
	// calculation.

	void AddBondForces();
	void AddBondForces(ISimBoxBase* const pISimBoxBase);

	void AddBondPairForces();
	void AddBondPairForces(ISimBoxBase* const pISimBoxBase);

	// Function to calculate the potential energy of the bonds and bondpairs

	double AddPotentialEnergy();

	void SetHead(CAbstractBead* const pHead);
	void SetTail(CAbstractBead* const pTail);

    // Function to validate the polymer's member variables when it moves to a new procesors: function body is only defined for parallel compilation
	
	bool Validate();
	
#if EnableParallelSimBox == SimMPSEnabled
    // We store a pointer to the wrapping extended polymer here so that the polymer's structure can be passed between processors
	// even when the polymer instance does not have a complete set of beads/bonds/bondpairs.
	
    inline mpuExtendedPolymer* GetExtendedPolymer() const {return m_pExtPolymer;}
    void SetExtendedPolymer(mpuExtendedPolymer* pExtPolymer);
	
#endif

	// ****************************************
	// Protected local functions
protected:

    // Function used by the mpsSimBox to manipulate polymers that cross processor boundaries.
	
	inline void ClearBeads()       {m_vBeads.clear();}       // Empty the bead container only
	inline void ClearBonds()       {m_vBonds.clear();}       // Empty the bond container only
	inline void ClearBondPairs()   {m_vBondPairs.clear();}   // Empty the bondpair container only
	
	void Clear();  // Empty all containers in polymer
	
	// Function to write out the polymer's complete member variables for debugging purposes
	
	void Dump() const;
	
    // Access functions for mpuExtendedPolymers and the mpsSimBox to manipulate polymer instances and replace beads/bonds/bondpairs
	// by their ghost equivalents and vice versa.
	
	bool AddSingleBead(CAbstractBead* const pBead);   // Used for single-bead polymers
	
	bool AddBead(CAbstractBead* const pBead);       // Used for multi-bead polymers
	bool AddBond(CBond* const pBond);
	bool AddBondPair(CBondPair* const pBondPair);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long m_id;
	long m_Type;

	CAbstractBead* m_pHead;		// allow for CWallBeads to be stored here
	CAbstractBead* m_pTail;
	
#if EnableParallelSimBox == SimMPSEnabled
    mpuExtendedPolymer*   m_pExtPolymer;   // Wrapper extended polymer needed when the polymer spans more than two processor Spaces
#endif


	BeadVector		m_vBeads;
	BondVector		m_vBonds;
	BondPairVector	m_vBondPairs;
};

#endif // !defined(AFX_POLYMER_H__A2FACF41_3F61_11D3_820E_0060088AD300__INCLUDED_)
