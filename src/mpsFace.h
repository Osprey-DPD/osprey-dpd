// mpsFace.h: interface for the mpsFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSFACE_H__9D2D0098_5201_4345_888B_A52082164AA6__INCLUDED_)
#define AFX_MPSFACE_H__9D2D0098_5201_4345_888B_A52082164AA6__INCLUDED_


// Forward declarations



#include "mpsBorder.h"

class mpsFace : public mpsBorder
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsFace(mpsSimBox* const pSimBox, bool bExternal, long lx, long ly, long lz,
            long xc, long yc, long zc, long xn, long yn, long zn,  
            CNTCellVector vAllCNTCells);

	mpsFace(const mpsFace& oldBorder);

	virtual ~mpsFace();

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
    void CalculateSharedForceT();
    void CalculateSharedForceR();
	
	// ****************************************
	// Public access functions
public:

    // Function to return beads in this Face/Edge/Corner
	BeadList GetFECBeads() const;
	
    // Function to return beads in the external CNT cells adajacent to this Face/Edge/Corner
	BeadList GetExternalFECBeads();

    // Function to return beads in the external CNT cells adajacent to this Face
	BeadList GetExternalFBeads();

    // Function to return cells in this Face/Edge/Corner
	CNTCellVector& GetFECCells();

    // Function to return external cells in this Face/Edge/Corner
	CNTCellVector& GetExternalFECCells();

    // Function to empty external cells in this Face/Edge/Corner
	void ClearExternalFECCells();
	
	// Function to remove bead pointers from external cells in this Face/Edge/Corner
	void RemoveBeadsFromExternalFECCells();

    // Function to add a bead's coordinates to one of an X Face's external CNT cells
	void AddBeadToXCell(long index, long ix, long iy, long id, long type, 
                       double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to one of a Y Face's external CNT cells
	void AddBeadToYCell(long index, long ix, long iy, long id, long type, 
                       double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to one of a Z Face's external CNT cells
	void AddBeadToZCell(long index, long ix, long iy, long id, long type, 
                       double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to an X Face/Edge/Corner external CNT cells
	void AddBeadToFECXCell(long index, long ix, long iy, long id, long type, 
                          double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to a Y Face/Edge/Corner external CNT cells
	void AddBeadToFECYCell(long index, long ix, long iy, long id, long type, 
                          double x, double y, double z, double vx, double vy, double vz);

    // Function to add a bead's coordinates to a Z Face/Edge/Corner external CNT cells
	void AddBeadToFECZCell(long index, long ix, long iy, long id, long type, 
                          double x, double y, double z, double vx, double vy, double vz);

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

    const bool m_bExternal;  // Flag showing if this Face is internal or external

    // External connections between Border regions

    long       m_NNProcId;   // Processor id of Face's single neighbour
    long       m_NNFaceId;   // Face id of adjacent Face (0 - 5)

    long       m_XN;  // Normal vector to Face 
    long       m_YN;
    long       m_ZN;

    CNTCellVector m_vFECCells;          // CNT cells contained in this Face AND its Edges and Corners
    CNTCellVector m_vExternalFECCells;  // External CNT cells for this Face AND its Edges and Corners

};

#endif // !defined(AFX_MPSFACE_H__9D2D0098_5201_4345_888B_A52082164AA6__INCLUDED_)
