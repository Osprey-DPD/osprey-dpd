// CNTCellProfile.h: interface for the CCNTCellProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNTCELLPROFILE_H__1F48E677_585F_4951_8FDF_FED3F9CD3D03__INCLUDED_)
#define AFX_CNTCELLPROFILE_H__1F48E677_585F_4951_8FDF_FED3F9CD3D03__INCLUDED_


// Forward declarations

class CCNTCell;

#include "xxBase.h"



class CCNTCellProfile  
{
public:
	CCNTCellProfile();
	~CCNTCellProfile();

	// Access functions to the CNT cells contained in the profile. Note that most
	// of these are const as we don't want the cells altered in any way by the
	// analysis.

	inline  void Clear()								{m_vCells.clear();}
	inline  bool Empty() const							{return m_vCells.empty();}
	inline long  Size() const							{return m_vCells.size();}

	inline void AddCell(CCNTCell* const pCell)			{m_vCells.push_back(pCell);}
	inline const CCNTCell*       GetCell(long index)	{return m_vCells.at(index);}
	inline cCNTCellIterator Begin()						{return m_vCells.begin();}
	inline cCNTCellIterator End()						{return m_vCells.end();}


private:

	CNTCellVector	m_vCells;		// CNT cells contained in profile

};

#endif // !defined(AFX_CNTCELLPROFILE_H__1F48E677_585F_4951_8FDF_FED3F9CD3D03__INCLUDED_)
