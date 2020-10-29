// mpsBorder.h: interface for the mpsBorder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSBORDER_H__1C262C62_4CB1_42D2_991F_07E945D40B86__INCLUDED_)
#define AFX_MPSBORDER_H__1C262C62_4CB1_42D2_991F_07E945D40B86__INCLUDED_


// Forward declarations

class mpsSimBox;
class mpsFace;    // included so that derived classes don't have to
class mpsEdge;
class mpsCorner;


// Include files

#include "CNTCell.h"
#include "ExternalCNTCell.h"


#include "xxParallelBase.h"

class mpsBorder : public xxParallelBase
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsBorder(mpsSimBox* const pSimBox, long lx, long ly, long lz, long xc, long yc, long zc);

	mpsBorder(const mpsBorder& oldBorder);

	virtual ~mpsBorder();

	// ****************************************
	// Global functions, static member functions and variables

public:



	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // PVF to show if the Border is internal or external: the definition of 
    // internal or external depends on the type of Border region

    virtual bool IsExternal() const = 0;

    // Function to update positions of beads in Borders that do NOT require
    // inter-processor communication
	virtual void UpdatePos() = 0;  

    // Function to update bead positions in Borders that DO require
    // inter-processor communication
    virtual void UpdateSharedPos() = 0;

    // Function to update bead-bead forces that do not depend on adjacent processors
    virtual void UpdateForce() = 0;  

    // Function to calculate bead-bead forces that depend on bead coordinates owned
    // by adjacent processors
    virtual void CalculateSharedForce() = 0;

    // Function to update bead-bead forces that depend on calculations already
    // done by adjacent processors
    virtual void UpdateSharedForce() = 0;

    // Function to calculate bond forces between beads in the Border's cells and those in its adjacent processors
	virtual void CalculateSharedBondForce() = 0;
	

	// ****************************************
	// Public access functions
public:

    inline long GetLX()              const {return m_LX;}
    inline long GetLY()              const {return m_LY;}
    inline long GetLZ()              const {return m_LZ;}
    inline long GetCNTXCellNo()      const {return m_CNTXCellNo;}
    inline long GetCNTYCellNo()      const {return m_CNTYCellNo;}
    inline long GetCNTZCellNo()      const {return m_CNTZCellNo;}
	inline long	GetCNTXYCellTotal()  const {return m_CNTXCellNo*m_CNTYCellNo;}
	inline long	GetCNTXZCellTotal()  const {return m_CNTXCellNo*m_CNTZCellNo;}
	inline long	GetCNTYZCellTotal()  const {return m_CNTYCellNo*m_CNTZCellNo;}
	inline long	GetCNTCellTotal()    const {return m_CNTCellTotal;}

	CNTCellVector& GetExternalCNTCells();

	inline const  CNTCellVector& GetCNTCells() const {return m_vCNTCells;}

	// Access functions to the helper calculations of bead properties

	inline double GetBeadXForce() const {return m_TotalForce[0];}
	inline double GetBeadYForce() const {return m_TotalForce[1];}
	inline double GetBeadZForce() const {return m_TotalForce[2];}

	inline double GetBeadXVel() const {return m_TotalVel[0];}
	inline double GetBeadYVel() const {return m_TotalVel[1];}
	inline double GetBeadZVel() const {return m_TotalVel[2];}

    // Function to return the number of beads in the Border region

    long GetBeadTotal() const;

    // Function to return the set of bead instances in this Border region

	BeadList GetBeads() const;
	
	// Function to count the number of beads per cell in this Border
	
	double CalculateBeadsPerCell() const;

    // Function to count the number of non-bonded, bead-bead interactions for each bead in this Border
	
	double CalculateInteractionsPerBead();
	
	// Function to calculate the total force on beads in this Border

	long CalculateForceOnBeads();

	// Function to calculate the total velocity of all beads in this Border

	long CalculateVelocityOfBeads();

    // Function to empty all external CNT cells adjacent to the Border region
    // prior to storing the beads in them for force calculations

    void ClearExternalCells();


    // Functions used to move beads between processors

	void ClearLostBeadMap();
    long GetLostBeadTotal() const;
    long GetLostBeadTotalForKey(long key) const;

    AbstractBeadVector GetLostBeadsForKey(long key);


    void AddLostBeadToMap(long key, CAbstractBead* pBead);
    void AddLostBeadToMap(long alpha, long beta, long gamma, CAbstractBead* pBead);
    void AddLostBeadToMap(long cntCellId, long alpha, long beta, long gamma, CAbstractBead* pBead);

    inline LongBeadMMap& GetLostBeadMap()     {return m_mmLostBeads;}
    inline LongLongMMap& GetLostBeadCellMap() {return m_mmLostBeadCells;}

	long GetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma) const;

    // ****************************************
	// Protected local functions

protected:

    long GetCellIndexFromCoords(long i, long j, long k) const;
    long GetCellIndexFromCoords(long i, long j, long k, long LX, long LY) const;
    long GetCellIndexFromCoords(long i, long j, long k, long LX, long LY, long LZ,
                                long alpha, long beta, long gamma) const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:



	// ****************************************
	// Data members
protected:

    mpsSimBox* const m_pSimBox;  // Pointer to the mpsSimBox instance

	const long m_LX;        // No of CNT cells in each dimension of processor's Space
	const long m_LY;
	const long m_LZ;

	long m_CNTXCellNo;      // No of CNT cells in each dimension of Border region
	long m_CNTYCellNo;
	long m_CNTZCellNo;
	long m_CNTCellTotal;    // Total number of CNT cells in region

    CNTCellVector m_vCNTCells;         // CCNTCell instances copied into this region
    CNTCellVector m_vExternalCNTCells; // CExternalCNTCell instances created by this region

    LongBeadMMap  m_mmLostBeads;      // Multimap of (procId, pBead) for beads moving to a neighbouring processor
    LongLongMMap  m_mmLostBeadCells;  // Multimap of (cntCellId, beadId) corresponding to above beads

	// Internal data

	double m_TotalForce[3]; // Total force on all beads in this Border region
	double m_TotalVel[3];   // Total velocity of all beads
};

#endif // !defined(AFX_MPSBORDER_H__1C262C62_4CB1_42D2_991F_07E945D40B86__INCLUDED_)
