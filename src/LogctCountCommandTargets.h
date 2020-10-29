// LogctCountCommandTargets.h: interface for the CLogctCountCommandTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCOUNTCOMMANDTARGETS_H__98432E7C_D2F9_4937_B3C6_AEEC38013DDD__INCLUDED_)
#define AFX_LOGCTCOUNTCOMMANDTARGETS_H__98432E7C_D2F9_4937_B3C6_AEEC38013DDD__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctCountCommandTargets : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctCountCommandTargets(long time, long total);

	virtual ~CLogctCountCommandTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctCountCommandTargets& rMsg);

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

	CLogctCountCommandTargets(const CLogctCountCommandTargets& oldMessage);
	CLogctCountCommandTargets& operator=(const CLogctCountCommandTargets& rhs);


	// ****************************************
	// Data members
private:

	const long m_TargetTotal;		// Number of command targets

};

#endif // !defined(AFX_LOGCTCOUNTCOMMANDTARGETS_H__98432E7C_D2F9_4937_B3C6_AEEC38013DDD__INCLUDED_)
