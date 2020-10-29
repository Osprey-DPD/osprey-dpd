// LogToggleCurrentStateAnalysis.h: interface for the CLogToggleCurrentStateAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLECURRENTSTATEANALYSIS_H__B67F17C1_9A22_4AD5_8DF6_009DA00D4BBF__INCLUDED_)
#define AFX_LOGTOGGLECURRENTSTATEANALYSIS_H__B67F17C1_9A22_4AD5_8DF6_009DA00D4BBF__INCLUDED_


#include "LogInfoMessage.h"

class CLogToggleCurrentStateAnalysis : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleCurrentStateAnalysis(long time, bool bAnalyse);

	virtual ~CLogToggleCurrentStateAnalysis();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleCurrentStateAnalysis& rMsg);

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

	CLogToggleCurrentStateAnalysis(const CLogToggleCurrentStateAnalysis& oldMessage);
	CLogToggleCurrentStateAnalysis& operator=(const CLogToggleCurrentStateAnalysis& rhs);


	// ****************************************
	// Data members
private:
	
	const bool m_bAnalyse;	// Flag showing whether analysis is performed or not

};

#endif // !defined(AFX_LOGTOGGLECURRENTSTATEANALYSIS_H__B67F17C1_9A22_4AD5_8DF6_009DA00D4BBF__INCLUDED_)
