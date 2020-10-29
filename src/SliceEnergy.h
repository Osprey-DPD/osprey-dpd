// SliceEnergy.h: interface for the CSliceEnergy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICEENERGY_H__A637A368_0015_4921_9DDF_1A2AF23D9CF4__INCLUDED_)
#define AFX_SLICEENERGY_H__A637A368_0015_4921_9DDF_1A2AF23D9CF4__INCLUDED_



#include "CNTCellProfile.h"


#include "IRegionAnalysis.h"

class CSliceEnergy : public IRegionAnalysis
{
	// ****************************************
	// Construction/Destruction
public:

	CSliceEnergy(const ISimBox* pISimBox, bool bNormalize, long xn, long yn, long zn, 
				 double lowerFraction, double upperFraction);

	virtual ~CSliceEnergy();

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

	const bool	 m_bNormalize;		// true = divide energy by number of beads
	const long   m_Xn;				// Slice normal
	const long   m_Yn;
	const long   m_Zn;
		  double m_LowerFraction;	// Fractional coordinate of lower slice edge
		  double m_UpperFraction;	// Fractional coordinate of upper slice edge
									// Note const to allow for adjustment

	long   m_BeadTotal;				// Number of beads in region
	double m_KineticEnergy;			// KE of beads in region
	double m_PotentialEnergy;		// PE of beads in region
	double m_TotalEnergy;			// Total energy of beads in region

	double m_LowerCoord;			// Lower coordinate of slice edge (units of r0)
	double m_UpperCoord;			// Upper coordinate of slice edge	 "

	CCNTCellProfile	m_Cells;		// Container of cells within the slice
};

#endif // !defined(AFX_SLICEENERGY_H__A637A368_0015_4921_9DDF_1A2AF23D9CF4__INCLUDED_)
