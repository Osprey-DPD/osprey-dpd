// AnalysisObject.h: interface for the CAnalysisObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISOBJECT_H__A4AEA914_9D0D_11D3_BF15_004095086186__INCLUDED_)
#define AFX_ANALYSISOBJECT_H__A4AEA914_9D0D_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CAnalysisData;
class CAnalysis;
class CInputData;

// Include file to gain access to the STL container classes 

#include "xxBase.h"



class CAnalysisObject  
{
public:
	CAnalysisObject(const zString type);
	~CAnalysisObject();

	friend zOutStream& operator<<(zOutStream& os, const CAnalysisObject& AnOb);
	friend zInStream&  operator>>(zInStream& is,        CAnalysisObject& AnOb);

	bool IsDataValid()  const;
	long GetStartTime() const;
	long GetEndTime()   const;

	CAnalysis* CreateAnalysis(const zString runId, long sample) const;

	bool ValidateData(const CInputData& riData) const;

private:
	void SetDataValid(bool bValid);


private:
	CAnalysisData* m_pAD;		// Pointer to analysis data object
	bool m_bpADValid;			// Flag showing if analysis data is valid
};

#endif // !defined(AFX_ANALYSISOBJECT_H__A4AEA914_9D0D_11D3_BF15_004095086186__INCLUDED_)
