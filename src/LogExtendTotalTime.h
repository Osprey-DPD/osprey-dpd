// LogExtendTotalTime.h: interface for the CLogExtendTotalTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGEXTENDTOTALTIME_H__E823D8B2_4D9E_4EEB_B4E3_AE7A60A0B716__INCLUDED_)
#define AFX_LOGEXTENDTOTALTIME_H__E823D8B2_4D9E_4EEB_B4E3_AE7A60A0B716__INCLUDED_


#include "LogConstraintMessage.h"

class CLogExtendTotalTime : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogExtendTotalTime(long time, long extraTime);

	virtual ~CLogExtendTotalTime();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogExtendTotalTime& rMsg);

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

	CLogExtendTotalTime(const CLogExtendTotalTime& oldMessage);
	CLogExtendTotalTime& operator=(const CLogExtendTotalTime& rhs);


	// ****************************************
	// Data members
private:

	const long m_ExtraTime;	// Number of time steps to extend simulation
};

#endif // !defined(AFX_LOGEXTENDTOTALTIME_H__E823D8B2_4D9E_4EEB_B4E3_AE7A60A0B716__INCLUDED_)
