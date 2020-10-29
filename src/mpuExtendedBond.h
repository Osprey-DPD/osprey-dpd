// mpuExtendedBond.h: interface for the mpuExtendedBond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPUEXTENDEDBOND_H__82B979B3_9632_4387_80A2_19F186DADCC3__INCLUDED_)
#define AFX_MPUEXTENDEDBOND_H__82B979B3_9632_4387_80A2_19F186DADCC3__INCLUDED_


// Forward declarations

class CBond;


// Include file to gain access to the typedefs for beads and bonds

#include "xxBase.h"
#include "mpuGhostBead.h"



class mpuExtendedBond  
{
	// Friend classes need access to bond data in order to
	// calculate forces between bonded beads and observables

//	friend class CMonitor;

	// ****************************************
	// Construction/Destruction
public:

	mpuExtendedBond();
	
	// Constructor to wrap an existing CBond instance
	mpuExtendedBond(CBond* const pBond, mpuGhostBead* const pHeadGhost, mpuGhostBead* const pTailGhost);
	
	// Constructor that contains no concrete CBond instance: the passed-in bond is an exemplar
	// so we also pass in the first bond id .
	mpuExtendedBond(CBond* const pBond, long firstBondId, mpuGhostBead* const pHeadGhost, mpuGhostBead* const pTailGhost);

//	mpuExtendedBond(const mpuExtendedBond& oldBond);
	~mpuExtendedBond();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    // Function to calculate the force due to this extended bond when both its beads are in the same processor's Space
	void AddForce();

    // Function to calculate the force due to this extended bond when it spans a processor boundary
	void AddSharedForce(long id, double x, double y, double z, double* pfx, double* pfy, double* pfz);


    // Function returning the pid of the processor that owns the empty ghost bead if any or -1 for all others
	long GetPid() const;
	
    // Function to get and set the wrapped bond instance
	inline CBond* GetBond() const {return m_pBond;}
	inline void   SetBond(CBond* const pBond) {m_pBond = pBond;}
	
	// Functions to return the ghost beads wrapped by the extended bond 
	inline mpuGhostBead* GetHeadGhost() const {return m_pHead;}
	inline mpuGhostBead* GetTailGhost() const {return m_pTail;}

	// Function to show if the extended bond is empty, ie, both its ghost beads are empty.
	inline bool   IsEmpty()            const {return m_pHead->IsEmpty() && m_pTail->IsEmpty();}
	inline bool   IsEitherEmpty()      const {return m_pHead->IsEmpty() || m_pTail->IsEmpty();}
	inline bool   IsHeadGhostEmpty()   const {return m_pHead->IsEmpty();}
	inline bool   IsTailGhostEmpty()   const {return m_pTail->IsEmpty();}
	
	
	// Forwarding functions to the wrapped bond instance
	long   GetId()              const;
	long   GetType()            const;
	long   GetHeadIndex()       const;
	long   GetTailIndex()       const;
	CAbstractBead* GetHead()    const;
	CAbstractBead* GetTail()    const;
	
	inline double GetSpringConstant()    const {return m_SpringConstant;}
	inline double GetUnstretchedLength() const {return m_UnStretchedLength;}
	
	// Debug function to dump the contents of the bond instance
	void Dump() const;

	// Function to set the pid of the processor that owns the ghost bead
	inline void SetPid(long pid) {m_Pid = pid;}
	
	// Functions to remove each of the ghost beads at either end of the extended bond
	
	 void RemoveHeadGhost();
	 void RemoveTailGhost(); 

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

	CBond*        m_pBond;       // Pointer to the wrapped bond instance
	mpuGhostBead* m_pHead;       // Pointer to the extended bond's head ghost bead
	mpuGhostBead* m_pTail;       // Pointer to the extended bond's tail ghost bead
	
	long   m_Pid;                // Pid of processor owning the bond's empty ghost bead if any
	long   m_id;
	long   m_Type;
	long   m_HeadIndex;
	long   m_TailIndex;
	
	double m_SpringConstant;     // Value derived from the wrapped CBond instance
	double m_UnStretchedLength;  // Value derived from the wrapped CBond instance
	
	// Local data to hold the bond length and force components
	
	double m_dx;				
	double m_dy;				
	double m_dz;				
	double m_Length;  // Current bond length used in CBondPair
	double m_fx;				
	double m_fy;				
	double m_fz;				
	

};

#endif // !defined(AFX_MPUEXTENDEDBOND_H__82B979B3_9632_4387_80A2_19F186DADCC3__INCLUDED_)
