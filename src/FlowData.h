// FlowData.h: interface for the CFlowData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOWDATA_H__8DA209D4_63A3_11D4_BF45_004095086186__INCLUDED_)
#define AFX_FLOWDATA_H__8DA209D4_63A3_11D4_BF45_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CFlowData : public CAnalysisData  
{
	friend class CFlow;		// Needed to copy data into the CFlow object

	// ****************************************
	// Construction/Destruction
public:

	CFlowData();

	virtual ~CFlowData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static zString m_Type;		// Keyword used to identify aggregate analysis

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual const zString GetAnalysisType() const;

	// Function to copy the data into a CAnalysis-derived object

	virtual CAnalysis* CreateAnalysis(const zString runId, long samplePeriod);

	virtual bool ValidateData(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

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

	zString m_PolymerMajor;			// Polymer whose motion is measured
	long m_X;						// Normal to flow: must be rational hence we use integers
	long m_Y;							
	long m_Z;
	long m_FlowX;					// Direction of flow
	long m_FlowY;
	long m_FlowZ;
	long m_LaminarTotal;			// No of slices used in velocity gradient analysis
};

#endif // !defined(AFX_FLOWDATA_H__8DA209D4_63A3_11D4_BF45_004095086186__INCLUDED_)


