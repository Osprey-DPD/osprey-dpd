// Raft.h: interface for the CRaft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAFT_H__614B1F21_DBED_449E_AD6D_94DE5D165E0C__INCLUDED_)
#define AFX_RAFT_H__614B1F21_DBED_449E_AD6D_94DE5D165E0C__INCLUDED_


// Forward declarations

class CCNTCell;

// Include files

#include "CNTCellProfile.h"


class CRaft  
{
public:
	CRaft(zLongVector headTypes, zLongVector tailTypes);
	~CRaft();

	// ****************************************
	// Public access functions
public:

	inline void	AddCell(CCNTCell* const pCell)  {m_Cells.AddCell(pCell);}

	inline long GetCellTotal()		const {return m_Cells.Size();}

	long		GetPolymerTotal();

	// ****************************************
	// Implementation
private:

	zLongVector		m_HeadBeadTypes;	// Must be distinct from remaining polymer bead types
	zLongVector		m_TailBeadTypes;

	// Local data

	CCNTCellProfile m_Cells;			// Set of CNT cells in one "raft"

	long			m_PolymerTotal;		// Number of polymers (of all raft types) in the raft

};

#endif // !defined(AFX_RAFT_H__614B1F21_DBED_449E_AD6D_94DE5D165E0C__INCLUDED_)
