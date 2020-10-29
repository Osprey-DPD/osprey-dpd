// LogctListActiveCommandTargets.h: interface for the CLogctListActiveCommandTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTLISTACTIVECOMMANDTARGETS_H__95C31528_978B_440A_9DA8_57E8B165658B__INCLUDED_)
#define AFX_LOGCTLISTACTIVECOMMANDTARGETS_H__95C31528_978B_440A_9DA8_57E8B165658B__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctListActiveCommandTargets : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctListActiveCommandTargets(long time, long total, const StringSequence& targetList);

	virtual ~CLogctListActiveCommandTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctListActiveCommandTargets& rMsg);

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

	CLogctListActiveCommandTargets(const CLogctListActiveCommandTargets& oldMessage);
	CLogctListActiveCommandTargets& operator=(const CLogctListActiveCommandTargets& rhs);


	// ****************************************
	// Data members
private:

    const long      m_TargetTotal; // Total number of targets
    StringSequence  m_TargetList;	// Set of command target properties

    long            m_PerLine;     // Number of properties printer per line

};

#endif // !defined(AFX_LOGCTLISTACTIVECOMMANDTARGETS_H__95C31528_978B_440A_9DA8_57E8B165658B__INCLUDED_)
