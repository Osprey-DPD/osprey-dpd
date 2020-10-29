// Flow.h: interface for the CFlow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOW_H__8DA209D3_63A3_11D4_BF45_004095086186__INCLUDED_)
#define AFX_FLOW_H__8DA209D3_63A3_11D4_BF45_004095086186__INCLUDED_


// Forward declarations

class CFlowData;
class CSliceProfile;
class aaScalarProfile;
class aaVectorProfile;


#include "Analysis.h"

class CFlow : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CFlow(const zString runId, long sample, const CFlowData* const pAD);
	virtual ~CFlow();

	  
	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymer() const {return m_PolymerMajor;}
	inline double GetXNormal()		  const {return m_X;}
	inline double GetYNormal()		  const {return m_Y;}
	inline double GetZNormal()		  const {return m_Z;}	
	inline double GetFlowXNormal()	  const {return m_FlowX;}
	inline double GetFlowYNormal()	  const {return m_FlowY;}
	inline double GetFlowZNormal()	  const {return m_FlowZ;}	
	inline double GetLaminarTotal()	  const {return m_LaminarTotal;}	

	// ****************************************
	// PVS inherited from CAnalysis base class
public:

	// Function to get information about the interface from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);

private:						
	// Data specified by user for flow analysis

	zString m_PolymerMajor;		// Polymer whose flow is investigated
	long m_X;					// Normal to flow
	long m_Y;							
	long m_Z;				
	long m_FlowX;				// Direction of flow
	long m_FlowY;							
	long m_FlowZ;				
	long m_LaminarTotal;		// No of laminars in the velocity profile

	// Local data used in the analysis

	CTimeSeriesData* m_pTSD;		// Store time-dependent data here

	long m_MajorType;				// Integer versions of polymer and bead identifiers
	long m_MajorHeadType;
	long m_MajorTailType;
	
	CSliceProfile* m_pMajorSP;				// SliceProfile for polymer species
	
	aaScalarProfile* m_pMajorDensity;		// Density of polymer species per slice
	aaVectorProfile* m_pMajorMomentum;		// Momentum of polymer species per slice

	long m_SliceTotal;						// No of slices in profile

	double m_MajorMaxDensity;				// Largest density value found
	double m_MajorMinDensity;				// Smallest density value found
	long   m_MajorMaxId;					// Slice with max density
	long   m_MajorMinId;					// Slice with min density

	double m_MajorMaxVelocity;				// Max vel in direction of flow
	double m_MajorMinVelocity;				// Min vel in direction of flow

};

#endif // !defined(AFX_FLOW_H__8DA209D3_63A3_11D4_BF45_004095086186__INCLUDED_)



