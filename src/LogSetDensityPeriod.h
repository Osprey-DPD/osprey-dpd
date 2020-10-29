// LogSetDensityPeriod.h: interface for the CLogSetDensityPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETDENSITYPERIOD_H__148A8F72_6A0A_461B_A59E_9D976185325B__INCLUDED_)
#define AFX_LOGSETDENSITYPERIOD_H__148A8F72_6A0A_461B_A59E_9D976185325B__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetDensityPeriod : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetDensityPeriod(long time, long newPeriod);

	virtual ~CLogSetDensityPeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetDensityPeriod& rMsg);

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

	CLogSetDensityPeriod(const CLogSetDensityPeriod& oldMessage);
	CLogSetDensityPeriod& operator=(const CLogSetDensityPeriod& rhs);


	// ****************************************
	// Data members
private:

	const long m_Period; // Number of timesteps between density state output
};

#endif // !defined(AFX_LOGSETDENSITYPERIOD_H__148A8F72_6A0A_461B_A59E_9D976185325B__INCLUDED_)
