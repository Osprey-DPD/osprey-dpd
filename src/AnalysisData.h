// AnalysisData.h: interface for the CAnalysisData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISDATA_H__A4AEA915_9D0D_11D3_BF15_004095086186__INCLUDED_)
#define AFX_ANALYSISDATA_H__A4AEA915_9D0D_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;
class CInputData;

// Include file for the Analysis Factory object: it is included here so that
// all derived classes don't have to include it separately.

#include "acfAnalysisFactory.h"

#include "xxBase.h"

class CAnalysisData : public xxBase
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CAnalysisData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CAnalysisData& AnData);
	friend zInStream&  operator>>(zInStream& is,        CAnalysisData& AnData);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual const zString GetAnalysisType() const = 0;

	virtual CAnalysis* CreateAnalysis(const zString runId, long sample) = 0;
	
	// The ValidateData() function is not yet a PVF because it has not been
	// implemented by all derived classes. Eventually it should become
	// one.

	virtual bool ValidateData(const CInputData& riData) const;

	// ****************************************
	// Public access functions
public:

	bool IsDataValid() const;
	long GetStartTime() const;
	long GetEndTime() const;

	// ****************************************
	// Protected local functions
protected:

	CAnalysisData(bool bValid);

	void SetDataValid(bool bValid);
	void SetStartTime(long start);
	void SetEndTime(long end);

	zOutStream&  putASCIIStartTags(zOutStream &os) const;
	zOutStream&  putASCIIEndTags(zOutStream &os) const;

	zOutStream&  putXMLStartTags(zOutStream &os) const;
	zOutStream&  putXMLEndTags(zOutStream &os) const;

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
private:

	bool m_bDataValid;				// flag showing whether data is valid 
	long m_Start;					// Simulation time to start analysis
	long m_End;						//     "       "      end      "
};

#endif // !defined(AFX_ANALYSISDATA_H__A4AEA915_9D0D_11D3_BF15_004095086186__INCLUDED_)
