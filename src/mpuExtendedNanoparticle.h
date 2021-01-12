// mpuExtendedNanoparticle.h: interface for the mpuExtendedNanoparticle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUEXTENDEDNANOPARTICLE_H__e952ef18_da28_455a_8f70_7722035b163c__INCLUDED_)
#define AFX_MPUEXTENDEDNANOPARTICLE_H__e952ef18_da28_455a_8f70_7722035b163c__INCLUDED_


// Forward declarations

class CAbstractBead;
class CNanoparticle;
class mpuExtendedBond;
class mpuGhostBead;

// Include file to gain access to the typedefs for beads and bonds

#include "xxBase.h"
#include "aaVector.h"



class mpuExtendedNanoparticle  
{
	// ****************************************
	// Construction/Destruction: 
public:

    // Default constructor
	mpuExtendedNanoparticle();

    // Overloaded constructor that wraps a nanoparticle as it starts to leave its owning processor's space.    
	mpuExtendedNanoparticle(CNanoparticle* pNP);

    // Overloaded constructor that creates a nanoparticle as its first bead enters a processor's space.    
	mpuExtendedNanoparticle(CAbstractBead* const pBead);


	mpuExtendedNanoparticle(const mpuExtendedNanoparticle& oldNP);
	mpuExtendedNanoparticle& operator =(const mpuExtendedNanoparticle &oldNP);
	
	~mpuExtendedNanoparticle();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator <( const mpuExtendedNanoparticle &a, const mpuExtendedNanoparticle &b);
	friend bool operator ==(const mpuExtendedNanoparticle &a, const mpuExtendedNanoparticle &b);

	static long GetTotal();

private:

	static long m_CreatedExtNanoparticleTotal;		// Total number of created extended NP 

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

    // Function showing if the NP is leaving or entering the owning processor's space

	inline bool IsLeaving() const {return m_bIsLeaving;}

    // Function showing if the wrapped nanoparticle contains only concrete beads and bonds

    bool IsComplete() const;

	// Function to return the wrapped nanoparticle instance

	inline CNanoparticle* GetNanoparticle() const {return m_pNanoparticle;}
	
	// Access functions to the wrapped nanoparticle instance

	long   GetId()         const;
	long   GetSize()       const;
    
    void   AddBondForces();            // Function to calculate forces due to bonds wholly within one processor space
    void   AddExtendedBondForces();    // Function to calculate forces due to extended bonds


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

    const bool m_bIsLeaving;           // Flag showing if the wrapped nanoparticle instance is losing beads to other processors, or is made up of beads arriving
				        // from other processors.                           
    CNanoparticle*  m_pNanoparticle;   // Pointer to wrapped nanoparticle instance

    LongGhostBeadMap      m_mGhostBeads;
    ExtendedNPBondList    m_lConcreteNPBonds;    // Extended bonds with both concrete beads in owning proc space
    ExtendedNPBondList    m_lExtNPBonds;         // Extended bonds missing one or both beads
};

#endif // !defined(AFX_MPUEXTENDEDNANOPARTICLE_H__e952ef18_da28_455a_8f70_7722035b163c__INCLUDED_)
