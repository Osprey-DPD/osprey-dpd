// LogacACNCreateEventAnalysis.h: interface for the CLogacACNCreateEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACACNCREATEEVENTANALYSIS_H__21410820_6963_4057_A74D_34871CF94007__INCLUDED_)
#define AFX_LOGACACNCREATEEVENTANALYSIS_H__21410820_6963_4057_A74D_34871CF94007__INCLUDED_


#include "LogInfoMessage.h"

class CLogacACNCreateEventAnalysis : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacACNCreateEventAnalysis(long time, const zString sourceId, 
                                  const zString analysisId, long bins, double binWidth, long samples);

	virtual ~CLogacACNCreateEventAnalysis();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacACNCreateEventAnalysis& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogacACNCreateEventAnalysis(const CLogacACNCreateEventAnalysis& oldMessage);
	CLogacACNCreateEventAnalysis& operator=(const CLogacACNCreateEventAnalysis& rhs);


	// ****************************************
	// Data members
private:

    const zString m_SourceId;    // Name of the event source decorator
    const zString m_AnalysisId;  // Name of the event analysis decorator
    const long    m_BinTotal;    // Number of bins in the histogram
    const double  m_BinWidth;    // Fixed width of bins in the histogram
    const long    m_Samples;     // Number of sample periods to normalize over

};

#endif // !defined(AFX_LOGACACNCREATEEVENTANALYSIS_H__21410820_6963_4057_A74D_34871CF94007__INCLUDED_)
