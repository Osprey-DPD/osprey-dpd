// aeCNTCell.h: interface for the aeCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AECNTCELL_H__3D4DB8A6_3679_402F_A337_C8F5580EAD94__INCLUDED_)
#define AFX_AECNTCELL_H__3D4DB8A6_3679_402F_A337_C8F5580EAD94__INCLUDED_


#include "AbstractCell.h"

class aeCNTCell : public CAbstractCell
{
	// ****************************************
	// Construction/Destruction
public:

	aeCNTCell(long index, long coords[3], bool flag);

	virtual ~aeCNTCell();
	
	// ****************************************
	// Global functions, static member functions and variables
public:

	static void SetSimBoxLengths(const long nx, const long ny, const long nz,
							     const double cntlx, const double cntly, const double cntlz);

public:

	static long m_CNTXCellNo;
	static long m_CNTYCellNo;
	static long m_CNTZCellNo;
	static double m_CNTXCellWidth;
	static double m_CNTYCellWidth;
	static double m_CNTZCellWidth;


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	void UpdateForce();

	aeActiveBond* UpdateNucleation(aeActiveBond* const pBond);
	aeActiveBond* UpdatePolymerisation(aeActiveBond* const pBond);

	void SetNNCellIndex(long index, aeCNTCell *pCell);
	void SetIntNNCellIndex(long index, aeCNTCell* pCell);

	void AddActiveBondToCell(aeActiveBond* pBond);
	void RemoveActiveBondFromCell(aeActiveBond* pBond);
	ActiveBondList GetBonds();

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

	bool m_bExternal;

    aeCNTCell* m_aNNCells[27];		// Allow for both 2d and 3d
    aeCNTCell* m_aIntNNCells[13];

	ActiveBondList	m_lBonds;		// Set of active bonds in the cell

};

#endif // !defined(AFX_AECNTCELL_H__3D4DB8A6_3679_402F_A337_C8F5580EAD94__INCLUDED_)
