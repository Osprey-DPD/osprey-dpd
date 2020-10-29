// LogSetAnalysisPeriod.h: interface for the CLogSetAnalysisPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETANALYSISPERIOD_H__39B941E0_6FBF_4737_BE59_98149579A722__INCLUDED_)
#define AFX_LOGSETANALYSISPERIOD_H__39B941E0_6FBF_4737_BE59_98149579A722__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetAnalysisPeriod : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetAnalysisPeriod(long time, long newPeriod);

	virtual ~CLogSetAnalysisPeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetAnalysisPeriod& rMsg);

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

	CLogSetAnalysisPeriod(const CLogSetAnalysisPeriod& oldMessage);
	CLogSetAnalysisPeriod& operator=(const CLogSetAnalysisPeriod& rhs);


	// ****************************************
	// Data members
private:

	const long m_Period; // Number of timesteps between time-averaged data analysis
};

#endif // !defined(AFX_LOGSETANALYSISPERIOD_H__39B941E0_6FBF_4737_BE59_98149579A722__INCLUDED_)
