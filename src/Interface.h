// Interface.h: interface for the CInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_H__A4AEA917_9D0D_11D3_BF15_004095086186__INCLUDED_)
#define AFX_INTERFACE_H__A4AEA917_9D0D_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CInterfaceData;
class CSliceProfile;
class aaScalarProfile;
class aaVectorProfile;
class aaStressTensor1d;

// Include files

#include "Analysis.h"

class CInterface : public CAnalysis
{
	// ****************************************
	// Construction/Destruction
public:

	CInterface(const zString runId, long sample, const CInterfaceData* const pAD);
	virtual ~CInterface();

	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymerMajor() const {return m_PolymerMajor;}
	inline const zString GetPolymerMinor() const {return m_PolymerMinor;}
	inline double GetLowThreshold()  const {return m_LowDensity;}
	inline double GetHighThreshold() const {return m_HighDensity;}
	inline double GetXNormal() const {return m_X;}
	inline double GetYNormal() const {return m_Y;}
	inline double GetZNormal() const {return m_Z;}	
	
	// ****************************************
	// PVS inherited from CAnalysis base class
public:

	// Function to get information about the interface from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);

private:
	void Stress1d(const ISimBox* const pISimBox);
	void SurfaceTension();

private:						
	// Data specified by user for interfacial analysis

	zString m_PolymerMajor;		// Polymer on one side of interface
	zString m_PolymerMinor;		// Polymer on other side of interface
	double m_LowDensity;		// Thresholds for determining interface boundaries
	double m_HighDensity;
	long m_X;					// Normal to interface
	long m_Y;							
	long m_Z;				


	// Local data used in the analysis

	CTimeSeriesData* m_pTSD;		// Store time-dependent data here

	long m_MajorType;				// Integer versions of polymer and bead identifiers
	long m_MinorType;
	long m_MajorHeadType;
	long m_MinorHeadType;
	long m_MajorTailType;
	long m_MinorTailType;
	
	CSliceProfile* m_pMajorSP;				// SliceProfile for major polymer species
	CSliceProfile* m_pMinorSP;				//     "            minor    "       "
	
	aaScalarProfile* m_pMajorDensity;		// Density of major polymer species per slice
	aaScalarProfile* m_pMinorDensity;		//    "       minor    "       "     
	aaVectorProfile* m_pMajorMomentum;		// Momentum of major polymer species per slice
	aaVectorProfile* m_pMinorMomentum;		//    "       minor    "       "     

	long m_SliceTotal;						// No of slices in profile

	double m_MajorMaxDensity;			// Largest density value found
	double m_MajorMinDensity;			// Smallest density value found
	double m_MajorMaxLimit;				// includes threshold in its definition
	double m_MajorMinLimit;				//		"
	double m_MajorHighMean;				// Mean of densities above upper threshold
	double m_MajorLowMean;				// Mean of densities below lower threshold
	double m_MajorMidDensity;			// Mean of max and min densities
	long   m_MajorHighNo;				// No of slices in high mean
	long   m_MajorLowNo;				// No of slices in low mean
	long   m_MajorMaxId;				// Slice no of maximum density
	long   m_MajorMinId;				// Slice no of minimum density
	long   m_MajorMidPoint;				// Location of midpoint of major species profile

	double m_MinorMaxDensity;
	double m_MinorMinDensity;
	double m_MinorMaxLimit;				// includes threshold in its definition
	double m_MinorMinLimit;				//		"
	double m_MinorHighMean;
	double m_MinorLowMean;
	double m_MinorMidDensity;			// Mean of max and min densities
	long   m_MinorHighNo;				// No of slices in high mean
	long   m_MinorLowNo;				// No of slices in low mean
	long   m_MinorMaxId;				// Slice no of maximum density
	long   m_MinorMinId;				// Slice no of minimum density
	long   m_MinorMidPoint;				// Location of midpoint of minor species profile

	double m_SumMaxDensity;				// Maximum of summed density
	double m_SumMinDensity;				// Minimum of summed density
	long   m_SumMaxId;					// Slice no of maximum summed density
	long   m_SumMinId;					// Slice no of minimum summed density


// Stress tensor-related data

	aaStressTensor1d* m_pStress1d;
	aaScalarProfile*  m_pTotalStress;			// Stress profile for all interactions
	aaScalarProfile*  m_pMajorHHStress;
	aaScalarProfile*  m_pMajorMinorHHStress;
	aaScalarProfile*  m_pMinorHHStress;

	long m_StressGridTotal;
	double m_SurfaceTension;

	zLongVector		  m_vBeadStressIndex;		// Bead-Bead indexes for stress tensor


};

#endif // !defined(AFX_INTERFACE_H__A4AEA917_9D0D_11D3_BF15_004095086186__INCLUDED_)
