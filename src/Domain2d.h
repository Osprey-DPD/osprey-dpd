// Domain2d.h: interface for the CDomain2d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOMAIN2D_H__E05918A7_99AC_413E_8962_5DAC4F4F0F9D__INCLUDED_)
#define AFX_DOMAIN2D_H__E05918A7_99AC_413E_8962_5DAC4F4F0F9D__INCLUDED_


// Forward declarations

class CCNTCell;


// Include files

#include "RaftSet.h"	// includes CNTCellProfile.h


#include "AnalysisTool.h"

class CDomain2d : public CAnalysisTool  
{
public:
	CDomain2d(zLongVector headTypes, zLongVector tailTypes, bool bMonolayers[]);
	virtual ~CDomain2d();


public:
	// Public access functions

	void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	inline long		GetRaftTotal()				const {return m_RaftTotal;}
	inline double	GetMeanRaftCellSize()		const {return m_MeanRaftCellSize;}
	inline double	GetMeanRaftPolymerSize()	const {return m_MeanRaftPolymerSize;}
	inline double	GetMeanRaftPerimeter()		const {return m_MeanRaftPerimeter;}
	inline double	GetMeanRaftArea()			const {return m_MeanRaftArea;}

	// Private helper functions for raft analysis
private:


	// Implementation
private:

	zLongVector		m_HeadBeadTypes;	// Must be distinct from remaining polymer bead types
	zLongVector		m_TailBeadTypes;
	bool			m_bMonolayers[2];	// Which monolayers to monitor: upper/lower

	// Local Data

	long	m_RaftTotal;				// Number of rafts in monolayer
	double	m_MeanRaftCellSize;			// Mean number of CNT cells per raft
	double	m_MeanRaftPolymerSize;		// Mean number of raft polymers per raft
	double	m_MeanRaftArea;				// Mean area of rafts
	double	m_MeanRaftPerimeter;		// Mean circumference of rafts

	CRaftSet		m_Rafts;			// Set of distinct rafts in the monolayer
	zLongVector		m_vCellInRaft;		// Number of cells in rafts indexed by raft id
	zLongVector		m_vCellTotal;		// Number of raft polymers in each cell

};

#endif // !defined(AFX_DOMAIN2D_H__E05918A7_99AC_413E_8962_5DAC4F4F0F9D__INCLUDED_)
