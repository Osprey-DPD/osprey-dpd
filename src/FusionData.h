// FusionData.h: interface for the CFusionData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSIONDATA_H__88FC9137_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_FUSIONDATA_H__88FC9137_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CFusionData : public CAnalysisData  
{
	friend class CFusion;		// Needed to copy data into CFusion

	// ****************************************
	// Construction/Destruction
public:

	CFusionData();

	virtual ~CFusionData();

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


};

#endif // !defined(AFX_FUSIONDATA_H__88FC9137_4101_11D4_BF3D_004095086186__INCLUDED_)

