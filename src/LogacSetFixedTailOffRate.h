// LogacSetFixedTailOffRate.h: interface for the CLogacSetFixedTailOffRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFIXEDTAILOFFRATE_H__830C5CCA_92F4_4018_9998_920EEFEE4A1F__INCLUDED_)
#define AFX_LOGACSETFIXEDTAILOFFRATE_H__830C5CCA_92F4_4018_9998_920EEFEE4A1F__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetFixedTailOffRate : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFixedTailOffRate(long time, const zString type, double rate);

	virtual ~CLogacSetFixedTailOffRate();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFixedTailOffRate& rMsg);

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

	CLogacSetFixedTailOffRate(const CLogacSetFixedTailOffRate& oldMessage);
	CLogacSetFixedTailOffRate& operator=(const CLogacSetFixedTailOffRate& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	// Type of active cell network modified
	const double	m_Rate;	    // Probability per time-step for bond to detach
};

#endif // !defined(AFX_LOGACSETFIXEDTAILOFFRATE_H__830C5CCA_92F4_4018_9998_920EEFEE4A1F__INCLUDED_)
