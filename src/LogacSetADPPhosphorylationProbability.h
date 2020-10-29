// LogacSetADPPhosphorylationProbability.h: interface for the CLogacSetADPPhosphorylationProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETADPPHOSPHORYLATIONPROBABILITY_H__81C6F7B7_4DEE_4979_986B_60584264778F__INCLUDED_)
#define AFX_LOGACSETADPPHOSPHORYLATIONPROBABILITY_H__81C6F7B7_4DEE_4979_986B_60584264778F__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetADPPhosphorylationProbability : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetADPPhosphorylationProbability(long time, const zString type, double rate);

	virtual ~CLogacSetADPPhosphorylationProbability();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetADPPhosphorylationProbability& rMsg);

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

	CLogacSetADPPhosphorylationProbability(const CLogacSetADPPhosphorylationProbability& oldMessage);
	CLogacSetADPPhosphorylationProbability& operator=(const CLogacSetADPPhosphorylationProbability& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	// Type of active cell network modified
	const double	m_Rate;	    // Probability per time-step for transition
};

#endif // !defined(AFX_LOGACSETADPPHOSPHORYLATIONPROBABILITY_H__81C6F7B7_4DEE_4979_986B_60584264778F__INCLUDED_)
