// mpuExtendedPolymer.h: interface for the mpuExtendedPolymer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUEXTENDEDPOLYMER_H__35FBBDE2_71E7_4B90_B6B5_21B6F4063CD8__INCLUDED_)
#define AFX_MPUEXTENDEDPOLYMER_H__35FBBDE2_71E7_4B90_B6B5_21B6F4063CD8__INCLUDED_


// Forward declarations

class CAbstractBead;
class CPolymer;
class ISimBoxBase;
class mpuGhostBead;
class mpuExtendedBond;

// Include file to gain access to the typedefs for beads and bonds

#include "xxBase.h"
#include "aaVector.h"



class mpuExtendedPolymer  
{
	// ****************************************
	// Construction/Destruction: 
public:

	mpuExtendedPolymer();

    // Overloaded constructor that wraps a polymer in an extended polymer when its first bead leaves the owning processor's Space.
	// As we must not alter the passed-in bead we make it const.
	mpuExtendedPolymer(long newPid, const CAbstractBead* const pBead, CPolymer* const pPolymer);

    // Overloaded constructor that uses an exemplar polymer to construct a new extended polymer
	mpuExtendedPolymer(long oldPid, CAbstractBead* const pBead, CPolymer* const pPolymer, CPolymer* const pExemplar, long polySize, 
	                   long location, long firstBondId, long firstBondPairId);

	mpuExtendedPolymer(const mpuExtendedPolymer& oldPolymer);
	mpuExtendedPolymer& operator =(const mpuExtendedPolymer &oldPolymer);
	
	~mpuExtendedPolymer();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend bool operator <( const mpuExtendedPolymer &a, const mpuExtendedPolymer &b);
	friend bool operator ==(const mpuExtendedPolymer &a, const mpuExtendedPolymer &b);

	static long GetTotal();

private:

	static long m_CreatedExtPolymerTotal;		// Total number of created extended polymers - those deleted

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	// Function to return the wrapped polymer instance

	inline CPolymer* GetPolymer() const {return m_pPolymer;}
	
	inline bool GetValid() const {return m_bValid;}

	// Access functions to the wrapped polymer instance

	long   GetId()         const;
	long   GetType()       const;
	long   GetSize()       const;

	CAbstractBead* GetHead() const;
	CAbstractBead* GetTail() const;

    // Function to return the CM coordinates of the polymer

    aaVector GetCM() const;


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

//	double AddPotentialEnergy();

    // Function to write out the contents of the extended polymer for debug purposes
    void Dump() const;
		
	long GetConcreteBeadTotal() const;
	long GetEmptyGhostBeadTotal() const;
	long GetGhostBeadLocation(long id) const;
	long GetFirstExtendedBondId() const;
	long GetFirstExtendedBondPairId() const;


	bool IsEmpty() const;
	bool IsComplete() const;
	bool RemoveBeadFromGhost(long newPid, CAbstractBead* const pBead);
	bool AddBeadToGhost(long oldPid, CAbstractBead* pBead);
	
	bool UpdateGhostBeadCoords(long id, double x, double y, double z);
	
	zLongVector GetGhostBeadPids() const;
	
	inline LongGhostBeadMap&      GetGhostBeads()             {return m_mGhostBeads;}
	inline ExtendedBondList&      GetExtendedBonds()          {return m_lExtBonds;}
	inline ExtendedBondPairList&  GetExtendedBondPairs()      {return m_lExtBondPairs;}

	// ****************************************
	// Protected local functions
protected:

    void ListBeads() const;
    void ListConcreteBeads() const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	CPolymer*  m_pPolymer;   // Pointer to wrapped polymer instance
	
	const bool m_bValid;     // Flag showing if the wrapped polymer instance is losing beads to other processors, or is made up of beads arriving
							 // from other processors.
	long m_HeadId;          // Numeric id of the polymer's head bead
	long m_TailId;          // Numeric id of the polymer's tail bead

	LongGhostBeadMap      m_mGhostBeads;         // Set of ghost beads in this extended polymer instance
	ExtendedBondList      m_lExtBonds;           // Set of extended bonds 
	ExtendedBondPairList  m_lExtBondPairs;       // Set of extended stiff bonds 
	
};

#endif // !defined(AFX_MPUEXTENDEDPOLYMER_H__35FBBDE2_71E7_4B90_B6B5_21B6F4063CD8__INCLUDED_)
