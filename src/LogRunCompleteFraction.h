// LogRunCompleteFraction.h: interface for the CLogRunCompleteFraction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRUNCOMPLETEFRACTION_H__8BBB8486_60FB_4F9E_86A8_94C63BBC9DB3__INCLUDED_)
#define AFX_LOGRUNCOMPLETEFRACTION_H__8BBB8486_60FB_4F9E_86A8_94C63BBC9DB3__INCLUDED_


#include "LogInfoMessage.h"

class CLogRunCompleteFraction : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogRunCompleteFraction(long time, const long percent);

	virtual ~CLogRunCompleteFraction();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogRunCompleteFraction& rMsg);

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

	CLogRunCompleteFraction(const CLogRunCompleteFraction& oldMessage);
	CLogRunCompleteFraction& operator=(const CLogRunCompleteFraction& rhs);


	// ****************************************
	// Data members
private:

	const long m_Percentage;	// Percentage of run complete at time of message

};

#endif // !defined(AFX_LOGRUNCOMPLETEFRACTION_H__8BBB8486_60FB_4F9E_86A8_94C63BBC9DB3__INCLUDED_)
