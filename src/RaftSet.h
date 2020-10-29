// RaftSet.h: interface for the CRaftSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAFTSET_H__35470192_67EA_497E_A81D_3EB8C2947508__INCLUDED_)
#define AFX_RAFTSET_H__35470192_67EA_497E_A81D_3EB8C2947508__INCLUDED_


// Forward declarations


// Include files

#include "Raft.h"


class CRaftSet  
{
public:
	CRaftSet();
	~CRaftSet();

	// Access functions

	inline void         Clear()							{m_Rafts.clear();}
	inline void         AddRaft(CRaft* const pRaft)		{m_Rafts.push_back(pRaft);}
	inline CRaft*       GetRaft(long index)		 const	{return m_Rafts.at(index);}

	inline long         GetRaftTotal()			 const	{return m_Rafts.size();}
	inline double		GetMeanRaftCellSize()    const	{return m_MeanRaftCellSize;}
	inline double		GetMeanRaftPolymerSize() const	{return m_MeanRaftPolymerSize;}
	inline double		GetMeanRaftArea()		 const	{return m_MeanRaftArea;}
	inline double		GetMeanRaftPerimeter()   const	{return m_MeanRaftPerimeter;}
	inline double		GetSDevRaftCellSize()    const	{return m_SDevRaftCellSize;}
	inline double		GetSDevRaftPolymerSize() const	{return m_SDevRaftPolymerSize;}
	inline double		GetSDevRaftArea()		 const	{return m_SDevRaftArea;}
	inline double		GetSDevRaftPerimeter()   const	{return m_SDevRaftPerimeter;}

	// Functions to calculate the properties of the rafts and delete all
	// raft objects without destroying the CRaftSet object

	void DeleteAll();

	double CalculateMeanRaftCellSize();
	double CalculateMeanRaftPolymerSize();
	double CalculateMeanRaftArea();
	double CalculateMeanRaftPerimeter();

private:

	RaftSequence	m_Rafts;				// Container holding the CRaft objects

	double			m_MeanRaftCellSize;		// Mean number of CNT cells per raft
	double			m_MeanRaftPolymerSize;	// Mean number of polymers per raft
	double			m_MeanRaftArea;			// Mean area of rafts
	double			m_MeanRaftPerimeter;	// Mean perimeter of rafts

	double			m_SDevRaftCellSize;		// Standard deviation of number of cells per raft
	double			m_SDevRaftPolymerSize;	// Standard deviation of number of polymers per raft
	double			m_SDevRaftArea;			// Standard deviation of raft area
	double			m_SDevRaftPerimeter;	// Standard deviation of raft perimeter


};

#endif // !defined(AFX_RAFTSET_H__35470192_67EA_497E_A81D_3EB8C2947508__INCLUDED_)
