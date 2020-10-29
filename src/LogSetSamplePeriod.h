// LogSetSamplePeriod.h: interface for the CLogSetSamplePeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETSAMPLEPERIOD_H__55493A1C_3CD0_4AAD_8FC1_ACCEA53C4CD4__INCLUDED_)
#define AFX_LOGSETSAMPLEPERIOD_H__55493A1C_3CD0_4AAD_8FC1_ACCEA53C4CD4__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetSamplePeriod : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetSamplePeriod(long time, long newPeriod);

	virtual ~CLogSetSamplePeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetSamplePeriod& rMsg);

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

	CLogSetSamplePeriod(const CLogSetSamplePeriod& oldMessage);
	CLogSetSamplePeriod& operator=(const CLogSetSamplePeriod& rhs);


	// ****************************************
	// Data members
private:

	const long m_Period; // Number of timesteps between data samples
};

#endif // !defined(AFX_LOGSETSAMPLEPERIOD_H__55493A1C_3CD0_4AAD_8FC1_ACCEA53C4CD4__INCLUDED_)
