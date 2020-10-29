// mpsCorner.h: interface for the mpsCorner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSCORNER_H__63D9242E_4CE8_4541_8209_B6BBD40460B4__INCLUDED_)
#define AFX_MPSCORNER_H__63D9242E_4CE8_4541_8209_B6BBD40460B4__INCLUDED_


// Forward declarations




#include "mpsBorder.h"

class mpsCorner : public mpsBorder
{
public:
	// ****************************************
	// Construction/Destruction
public:

    mpsCorner(mpsSimBox* const pSimBox, mpsFace* const pF1, mpsFace* const pF2, mpsFace* const pF3, 
              long i, long j, long k, long lx, long ly, long lz, 
              CCNTCell* const pCell);

	mpsCorner(const mpsCorner& oldBorder);

	virtual ~mpsCorner();

	// ****************************************
	// Global functions, static member functions and variables

public:



	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool IsExternal() const;
	virtual void UpdatePos();
	virtual void UpdateSharedPos();
	virtual void UpdateForce();
	virtual void CalculateSharedForce();
	virtual void UpdateSharedForce();
	virtual void CalculateSharedBondForce();


    // Secondary non-bonded bead force calculations specific to certain Border regions

    void CalculateSharedForceUTR();
    void CalculateSharedForceUBR();
    void CalculateSharedForceDTR();
    void CalculateSharedForceDBR();
    void CalculateSharedForceUTL();
    void CalculateSharedForceDTL();


	// ****************************************
	// Public access functions
public:

    // Function to return beads in the external CNT cell adajacent to this Corner diagonal
	BeadList GetExternalBeads();


    // Function to add a bead's coordinates to the Corner's single external cell
	void AddBeadToCell(long index, long id, long type, double x, double y, double z, 
                       double vx, double vy, double vz);

    // Function to clear bead coordinates out of external cells before the next calculation of forces
	
	void ClearExternalCells();
	
	// Function to remove bead pointers from the lone external cell in this Corner
	void RemoveBeadsFromExternalCells();

	// ****************************************
	// Protected local functions


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:



	// ****************************************
	// Data members
private:

    mpsFace* const m_pFace1;  // First Face that defines this Corner
    mpsFace* const m_pFace2;  // Second Face that defines this Corner
    mpsFace* const m_pFace3;  // Third Face that defines this Corner

    const long m_i;
    const long m_j;  // Indices of the single CNT cell in the Corner
    const long m_k;



};

#endif // !defined(AFX_MPSCORNER_H__63D9242E_4CE8_4541_8209_B6BBD40460B4__INCLUDED_)
