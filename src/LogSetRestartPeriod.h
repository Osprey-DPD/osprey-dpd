// LogSetRestartPeriod.h: interface for the CLogSetRestartPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETRESTARTPERIOD_H__7BC7AAF6_B57C_4E52_9BF3_8E776E49293E__INCLUDED_)
#define AFX_LOGSETRESTARTPERIOD_H__7BC7AAF6_B57C_4E52_9BF3_8E776E49293E__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetRestartPeriod : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetRestartPeriod(long time, long newPeriod);

	virtual ~CLogSetRestartPeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetRestartPeriod& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Pure virtual function to force each derived class to serialize its data

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

	CLogSetRestartPeriod(const CLogSetRestartPeriod& oldMessage);
	CLogSetRestartPeriod& operator=(const CLogSetRestartPeriod& rhs);


	// ****************************************
	// Data members
private:

	const long m_Period; // Number of timesteps between current state output
};

#endif // !defined(AFX_LOGSETRESTARTPERIOD_H__7BC7AAF6_B57C_4E52_9BF3_8E776E49293E__INCLUDED_)
