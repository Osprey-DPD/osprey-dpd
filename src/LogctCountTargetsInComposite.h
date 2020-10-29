// LogctCountTargetsInComposite.h: interface for the CLogctCountTargetsInComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCOUNTTARGETSINCOMPOSITE_H__01EC8843_26CD_48AA_88FD_793FE74600B6__INCLUDED_)
#define AFX_LOGCTCOUNTTARGETSINCOMPOSITE_H__01EC8843_26CD_48AA_88FD_793FE74600B6__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctCountTargetsInComposite : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctCountTargetsInComposite(long time, const zString label, long total);

	virtual ~CLogctCountTargetsInComposite();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctCountTargetsInComposite& rMsg);

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

	CLogctCountTargetsInComposite(const CLogctCountTargetsInComposite& oldMessage);
	CLogctCountTargetsInComposite& operator=(const CLogctCountTargetsInComposite& rhs);


	// ****************************************
	// Data members
private:

    const zString m_Label;    // Label of composite command target
	const long    m_Total;    // Number of targets in the composite target

};

#endif // !defined(AFX_LOGCTCOUNTTARGETSINCOMPOSITE_H__01EC8843_26CD_48AA_88FD_793FE74600B6__INCLUDED_)
