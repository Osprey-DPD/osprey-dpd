// taPolymerisePolymers.h: interface for the taPolymerisePolymers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAPOLYMERISEPOLYMERS_H__e12206acf_dd36_4094_84a7_33a3148b4f0f__INCLUDED_)
#define AFX_TAPOLYMERISEPOLYMERS_H__e12206acf_dd36_4094_84a7_33a3148b4f0f__INCLUDED_



// Forward declarations

class CBond;
class CCommandTargetPolymer;
class CSimBox;

#include "taActionLabel.h"

class taPolymerisePolymers : public taActionLabel
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taPolymerisePolymers(const zString label, CCommandTargetNode* const pDec, CSimBox* const pSimBox,
                         CBond* pPolyBondTemplate, long polyType, zLongVector vBeadTypes, long maxBonds,
                         double maxDistance, double springConst, double unstrLength,
                         double bondPrefactor, double bondBarrier);

	virtual ~taPolymerisePolymers();

    // ****************************************
	// Global functions, static member functions and variables
public:

    static const zString GetType();		// return the target's type

private:

	static const zString m_Type;

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

    const zString GetTargetType() const;    // return the target's type

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;


	virtual void Execute(long simTime);


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

    long GetSequentialBeadIndex(long beadId) const;
    
    void IncrementBondTotal(long beadId1, long beadId2);

    long MakeStickySiteBondIndex(long site1, long site2) const;

    bool AreStickySitesUnbonded(long site1, long site2) const;

    void UpdateStickySiteClusters(long site1, long site2);

    long GetSSBonds(long site) const;

    long GetStickySiteFromBead(long beadId) const;

    bool IsClusterSizeAllowed(long site1, long site2) const;
    
    bool ArrheniusFactorTest() const;  // Function that returns true if bond formation passes an Arrhenius function test
    
	// ****************************************
	// Data members

protected:


private:

    CSimBox* const          m_pSimBox;            // Pointer to CSimBox instance to allow new bond type to be created
    CCommandTargetPolymer*  m_pPolymerTarget;     // Pointer to actual polymer target
    CBond* const            m_pPolyBondTemplate;  // Template bond type to create the dynamic bonds
    
    long           m_PolymerType;   // Numeric identifier for polymers to dynamically bond together
    zLongVector    m_vBeadTypes;    // Container of numeric identifiers of bead types in polymers (may be dynamically created)
    long           m_MaxBonds;      // Max number of polymers per cluster: 2 = dimer, 3 = trimer, etc.
    double         m_MaxDistance;   // Range within which polymers will be tied together
    double         m_sprConst;      // Spring constant of harmonic potential tying polymers together
    double         m_unStrLength;   // Unstretched length of harmonic potential
    double         m_BondPrefactor; // Prefactor for the bond formation Arrhenius function
    double         m_BondBarrier;   // Energy barrier for the bond formation Arrhenius function
    
    // Parameter combinations for speed
    
    const double   m_MaxDistSquared;   // Square of max range to simpify calculation
    const double   m_ArrheniusExp;     // Constant part of the Arrhenius factor : exp(-barrrier/temperature)


    PolymerVector         m_vPolymers;        // Container of polymers that can be polymerised
    AbstractBeadVector    m_vStickyBeads;     // Container of beads that are polymerisable
    
    long           m_PolyBondCounter;  // Running count of number of polymerised bonds created 
    long           m_PolyBeadTotal;    // Number of beads of polymerisable type, to allow map key construction
    long           m_StickySiteTotal;  // Number of sticky sites in total
    long           m_ClusterCounter;   // Running count of number of clusters created 
    
    LongLongMap    m_mOrderedBeadIds;  // Map from non-sequential bead ids to sequential

    zLongVector    m_vBeadInStickySite;  // Vector from sequential bead index to which sticky site they belong to, a sticky site being a cluster of sticky beads
    
    LongLongMap    m_mClusterSize;     // Current size of a cluster indexed by cluster id

    std::map<long, bool> m_mStickySiteBonded; // Map that indicates true if two sticky sites are bound
    
    zLongVector    m_vSSBondTotals;  // Number of bonds made by a sticky site indexed by sequential id

    zLongVector    m_vSSToCluster;     // Cluster id indexed by sticky site sequential id

};

#endif // !defined(AFX_TAPOLYMERISEPOLYMERS_H__e12206acf_dd36_4094_84a7_33a3148b4f0f__INCLUDED_)
