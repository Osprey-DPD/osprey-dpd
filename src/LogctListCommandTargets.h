// LogctListCommandTargets.h: interface for the CLogctListCommandTargets class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTLISTCOMMANDTARGETS_H__E763C287_3417_4D40_B92E_1D917BA0D323__INCLUDED_)
#define AFX_LOGCTLISTCOMMANDTARGETS_H__E763C287_3417_4D40_B92E_1D917BA0D323__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctListCommandTargets : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctListCommandTargets(long time, long total, const StringSequence& targetList);

	virtual ~CLogctListCommandTargets();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctListCommandTargets& rMsg);

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

	CLogctListCommandTargets(const CLogctListCommandTargets& oldMessage);
	CLogctListCommandTargets& operator=(const CLogctListCommandTargets& rhs);


	// ****************************************
	// Data members
private:

    const long      m_TargetTotal; // Total number of targets
    StringSequence  m_TargetList;	// Set of command target properties

    long            m_PerLine;     // Number of properties printer per line

};

#endif // !defined(AFX_LOGCTLISTCOMMANDTARGETS_H__E763C287_3417_4D40_B92E_1D917BA0D323__INCLUDED_)
