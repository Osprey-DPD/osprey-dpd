// LogctCountActiveCommandTargets.h: interface for the CLogctCountActiveCommandTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCOUNTACTIVECOMMANDTARGETS_H__2A82DDF6_074F_4F3C_8DFC_48E9E30944A8__INCLUDED_)
#define AFX_LOGCTCOUNTACTIVECOMMANDTARGETS_H__2A82DDF6_074F_4F3C_8DFC_48E9E30944A8__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctCountActiveCommandTargets : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctCountActiveCommandTargets(long time, long total);

	virtual ~CLogctCountActiveCommandTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctCountActiveCommandTargets& rMsg);

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

	CLogctCountActiveCommandTargets(const CLogctCountActiveCommandTargets& oldMessage);
	CLogctCountActiveCommandTargets& operator=(const CLogctCountActiveCommandTargets& rhs);


	// ****************************************
	// Data members
private:

	const long m_ActiveTargetTotal;		// Number of active command targets

};

#endif // !defined(AFX_LOGCTCOUNTACTIVECOMMANDTARGETS_H__2A82DDF6_074F_4F3C_8DFC_48E9E30944A8__INCLUDED_)
