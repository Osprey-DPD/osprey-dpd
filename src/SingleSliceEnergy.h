// SingleSliceEnergy.h: interface for the CSingleSliceEnergy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLESLICEENERGY_H__FDD8CC7A_6901_4FBB_8B01_C0C019C5E946__INCLUDED_)
#define AFX_SINGLESLICEENERGY_H__FDD8CC7A_6901_4FBB_8B01_C0C019C5E946__INCLUDED_


// Forward declarations

class CCNTCellSlice;



#include "IRegionAnalysis.h"

class CSingleSliceEnergy : public IRegionAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CSingleSliceEnergy(const ISimBox* pISimBox, bool bNormalize, long xn, long yn, long zn, 
					   long sliceIndex);

	virtual ~CSingleSliceEnergy();

	// ****************************************
	// Static member functions and data members
public:

	
	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Pure virtual functions that must be implemented by all derived classes
public:

	// Function to analyse the selected region of the SimBox

	void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);



protected:


private:

	const bool	 m_bNormalize;		// true = divide energy by number of beads in region
	const long   m_Xn;				// Slice normal
	const long   m_Yn;
	const long   m_Zn;
	      long   m_SliceIndex;		// Index of CNT slice to analyse: not const to allow
									// adjustment if it lies outside SimBox

	long   m_BeadTotal;				// Number of beads in region
	double m_KineticEnergy;			// KE of beads in region
	double m_PotentialEnergy;		// PE of beads in region
	double m_TotalEnergy;			// Total energy of beads in region

	double m_LowerCoord;			// Lower coordinate of slice edge (units of r0)
	double m_UpperCoord;			// Upper coordinate of slice edge	 "

	CCNTCellSlice*	m_pSlice;		// Container of cells within the slice
};

#endif // !defined(AFX_SINGLESLICEENERGY_H__FDD8CC7A_6901_4FBB_8B01_C0C019C5E946__INCLUDED_)
