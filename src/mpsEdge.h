// mpsEdge.h: interface for the mpsEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSEDGE_H__EC1642F7_609A_4F21_9EA5_6C2E40D1FC66__INCLUDED_)
#define AFX_MPSEDGE_H__EC1642F7_609A_4F21_9EA5_6C2E40D1FC66__INCLUDED_


// Forward declarations



#include "mpsBorder.h"

class mpsEdge : public mpsBorder
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsEdge(mpsSimBox* const pSimBox, mpsFace* const pF1, mpsFace* const pF2,
            long lx, long ly, long lz, long xc, long yc, long zc, long xn, long yn, long zn,  
            CNTCellVector vAllCNTCells);

	mpsEdge(const mpsEdge& oldBorder);

	virtual ~mpsEdge();

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
    void CalculateSharedForceUT();
    void CalculateSharedForceUR();
    void CalculateSharedForceDT();
    void CalculateSharedForceTR();
    void CalculateSharedForceDR();
    void CalculateSharedForceBR();
    void CalculateSharedForceTL();


	// ****************************************
	// Public access functions
public:

	// Function to return beads in this Edge/Corner diagonal cells
	BeadList GetECBeads() const;

    // Function to return beads in the external CNT cells adajacent to this Edge/Corner diagonal
	BeadList GetExternalECBeads();

    // Function to return cells along this Edge/Corner diagonal
	CNTCellVector& GetECCells();

    // Function to return external cells along this Edge/Corner diagonal
	CNTCellVector& GetExternalECCells();

    // Function to add a bead's coordinates to one of the Edge's external cells
	void AddBeadToCell(long index, long ix, long id, long type, 
                       double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to one of the Edge/Corner external CNT cells
	void AddBeadToECCell(long index, long ix, long id, long type, 
                       double x, double y, double z, double vx, double vy, double vz);

    
	// Function to empty external cells in this Face/Edge/Corner
	void ClearExternalECCells();

	// Function to remove bead pointers from external cells in this Face/Edge/Corner
	void RemoveBeadsFromExternalECCells();



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

    mpsFace* const m_pFace1;  // First Face that defines this Edge
    mpsFace* const m_pFace2;  // Second Face that defines this Edge


    long m_XN;  // Normal vector to Edge: with -1 indicating the actual normal 
    long m_YN;
    long m_ZN;

    CNTCellVector m_vECCells;          // CNT cells contained in this Edge AND its two Corners
    CNTCellVector m_vExternalECCells;  // External CNT cells for this Edge AND its two Corners

};

#endif // !defined(AFX_MPSEDGE_H__EC1642F7_609A_4F21_9EA5_6C2E40D1FC66__INCLUDED_)
