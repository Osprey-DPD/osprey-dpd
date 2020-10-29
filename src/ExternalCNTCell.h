// ExternalCNTCell.h: interface for the CExternalCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNALCNTCELL_H__194A7443_3C15_43AA_B5E9_C7220D03C1D5__INCLUDED_)
#define AFX_EXTERNALCNTCELL_H__194A7443_3C15_43AA_B5E9_C7220D03C1D5__INCLUDED_


// Forward declarations

class mpsSimBox;


#include "CNTCell.h"

class CExternalCNTCell : public CCNTCell
{
	// Friend classes 


	// ****************************************
	// Construction/Destruction
public:

	CExternalCNTCell(CCNTCell* const pCell);

    virtual ~CExternalCNTCell();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void UpdateForceBetweenCells(bool bExternal, CAbstractBead* const pBead);

	// ****************************************
	// Public access functions
public:

    inline long GetBeadTotal() const {return m_vBeadIds.size();}

    // Functions to return the set of forces on all beads in this cell

    inline const zLongVector&   GetBeadIds()      const {return m_vBeadIds;}
    // Next function returns the number of forces added to bead coordinates in this external cell: debug purposes only
    inline const zLongVector&   GetForceCounter() const {return m_vForceCounter;}
    inline const zDoubleVector& GetXForces()      const {return m_vFX;}
    inline const zDoubleVector& GetYForces()      const {return m_vFY;}
    inline const zDoubleVector& GetZForces()      const {return m_vFZ;}

    // Functions to return the forces on each bead in this cell

	double GetXForceOnBead(long i) const;
	double GetYForceOnBead(long i) const;
	double GetZForceOnBead(long i) const;

	void Clear();

	void AddBeadToCell(long id, long type, double x, double y, double z, double vx, double vy, double vz);


    // Function to calculate bond forces between beads in this external cell and any bonded beads in a Border region.
	void UpdateBondForceBetweenCells(mpsSimBox* const pSimBox);
	
	
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

    CCNTCell* const m_pCell;      // Pointer to the original wrapped cell

    zLongVector    m_vBeadIds;         // Number of interactions for each bead in the cell
    zLongVector    m_vForceCounter;    // Set of bead ids held in this cell
    zLongVector    m_vBeadTypes;      // Bead types corresponding to above beads

    zDoubleVector  m_vBeadRadii;  // Bead radii corresponding to above beads

    zDoubleVector  m_vLX;
    zDoubleVector  m_vLY;         // Position coordinates of beads in this cell
    zDoubleVector  m_vLZ;

    zDoubleVector  m_vVX;
    zDoubleVector  m_vVY;         // Velocity coordinates of beads in this cell
    zDoubleVector  m_vVZ;

    zDoubleVector  m_vFX;
    zDoubleVector  m_vFY;         // Velocity coordinates of beads in this cell
    zDoubleVector  m_vFZ;


};

#endif // !defined(AFX_EXTERNALCNTCELL_H__194A7443_3C15_43AA_B5E9_C7220D03C1D5__INCLUDED_)
