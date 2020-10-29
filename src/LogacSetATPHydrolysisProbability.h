// LogacSetATPHydrolysisProbability.h: interface for the CLogacSetATPHydrolysisProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETATPHYDROLYSISPROBABILITY_H__12E57340_4AE6_4CB0_84C9_0288E9ECDEB1__INCLUDED_)
#define AFX_LOGACSETATPHYDROLYSISPROBABILITY_H__12E57340_4AE6_4CB0_84C9_0288E9ECDEB1__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetATPHydrolysisProbability : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetATPHydrolysisProbability(long time, const zString type, double rate);

	virtual ~CLogacSetATPHydrolysisProbability();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetATPHydrolysisProbability& rMsg);

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

	CLogacSetATPHydrolysisProbability(const CLogacSetATPHydrolysisProbability& oldMessage);
	CLogacSetATPHydrolysisProbability& operator=(const CLogacSetATPHydrolysisProbability& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	// Type of active cell network modified
	const double	m_Rate;	    // Probability per time-step for transition
};

#endif // !defined(AFX_LOGACSETATPHYDROLYSISPROBABILITY_H__12E57340_4AE6_4CB0_84C9_0288E9ECDEB1__INCLUDED_)
