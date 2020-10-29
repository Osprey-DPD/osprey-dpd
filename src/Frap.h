// Frap.h: interface for the CFrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAP_H__121402AD_7A6A_4961_8096_1074FE17B9E2__INCLUDED_)
#define AFX_FRAP_H__121402AD_7A6A_4961_8096_1074FE17B9E2__INCLUDED_


// Forward declarations

class CCNTCell;


// Include files

#include "CNTCellProfile.h"

#include "AnalysisTool.h"

class CFrap : public CAnalysisTool  
{
public:
	CFrap(long beadType);
	virtual ~CFrap();

	void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Access functions

	inline long GetCellTotal()			const {return m_CNTCellProfile.Size();}
	inline long GetCurrentTotal()		const {return m_CurrentTotal;}
	inline long GetCellTotal(long i)	const {return m_vCellTotal.at(i);}

private:
	const long		m_BeadType;			// Bead type to monitor

	CCNTCellProfile m_CNTCellProfile;	// Set of CNT cells making up 
	zLongVector		m_vCellTotal;		// Beads remaining in each cell
	long			m_CurrentTotal;		// Total number of beads remaining

	
};

#endif // !defined(AFX_FRAP_H__121402AD_7A6A_4961_8096_1074FE17B9E2__INCLUDED_)
