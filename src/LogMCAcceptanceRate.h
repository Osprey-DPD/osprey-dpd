// LogMCAcceptanceRate.h: interface for the CLogMCAcceptanceRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMCACCEPTANCERATE_H__D794240E_1EF0_4C96_9B04_5C4D5BEA8240__INCLUDED_)
#define AFX_LOGMCACCEPTANCERATE_H__D794240E_1EF0_4C96_9B04_5C4D5BEA8240__INCLUDED_


#include "LogInfoMessage.h"

class CLogMCAcceptanceRate : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogMCAcceptanceRate(long time, double rate);

	virtual ~CLogMCAcceptanceRate();


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogMCAcceptanceRate& rMsg);

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

	CLogMCAcceptanceRate(const CLogMCAcceptanceRate& oldMessage);
	CLogMCAcceptanceRate& operator=(const CLogMCAcceptanceRate& rhs);

	// ****************************************
	// Data members
private:

	const double m_AcceptRate;
};

#endif // !defined(AFX_LOGMCACCEPTANCERATE_H__D794240E_1EF0_4C96_9B04_5C4D5BEA8240__INCLUDED_)
