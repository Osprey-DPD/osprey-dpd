// LogacSetFixedHeadOffRate.h: interface for the CLogacSetFixedHeadOffRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFIXEDHEADOFFRATE_H__89D5CE95_0F48_4268_9875_A1C012F242D3__INCLUDED_)
#define AFX_LOGACSETFIXEDHEADOFFRATE_H__89D5CE95_0F48_4268_9875_A1C012F242D3__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetFixedHeadOffRate : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFixedHeadOffRate(long time, const zString type, double rate);

	virtual ~CLogacSetFixedHeadOffRate();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFixedHeadOffRate& rMsg);

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

	CLogacSetFixedHeadOffRate(const CLogacSetFixedHeadOffRate& oldMessage);
	CLogacSetFixedHeadOffRate& operator=(const CLogacSetFixedHeadOffRate& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	// Type of active cell network modified
	const double	m_Rate;	    // Probability per time-step for bond to detach
};

#endif // !defined(AFX_LOGACSETFIXEDHEADOFFRATE_H__89D5CE95_0F48_4268_9875_A1C012F242D3__INCLUDED_)
