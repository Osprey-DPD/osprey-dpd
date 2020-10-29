// FusionPoreData.h: interface for the CFusionPoreData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSIONPOREDATA_H__E8DD1924_C12B_11D4_BF58_004095086186__INCLUDED_)
#define AFX_FUSIONPOREDATA_H__E8DD1924_C12B_11D4_BF58_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CFusionPoreData : public CAnalysisData  
{
	friend class CFusionPore;		// Needed to copy data into CFusionPore

	// ****************************************
	// Construction/Destruction
public:

	CFusionPoreData();

	virtual ~CFusionPoreData();

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

#endif // !defined(AFX_FUSIONPOREDATA_H__E8DD1924_C12B_11D4_BF58_004095086186__INCLUDED_)

