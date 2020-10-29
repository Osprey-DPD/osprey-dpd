// LogLamellaDomain.h: interface for the CLogLamellaDomain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLAMELLADOMAIN_H__92251060_F955_4F85_A35C_CFA1F69F1905__INCLUDED_)
#define AFX_LOGLAMELLADOMAIN_H__92251060_F955_4F85_A35C_CFA1F69F1905__INCLUDED_


#include "LogEventMessage.h"

class CLogLamellaDomain : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogLamellaDomain(long time, const zString name, long type, bool bOuter, long domainTotal);
	
	virtual ~CLogLamellaDomain();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogLamellaDomain& rMsg);

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

	CLogLamellaDomain(const CLogLamellaDomain& oldMessage);
	CLogLamellaDomain& operator=(const CLogLamellaDomain& rhs);

	// ****************************************
	// Data members
private:


	const zString m_Name;				// Aggregate name, e.g., bilayer
	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2
	const bool	  m_bOuterMonolayer;	// Flags showing which monolayer has domains
	const long	  m_DomainTotal;		// Number of domains in monolayer
};

#endif // !defined(AFX_LOGLAMELLADOMAIN_H__92251060_F955_4F85_A35C_CFA1F69F1905__INCLUDED_)
