// LogacSetProximityTailOnSeparation.h: interface for the CLogacSetProximityTailOnSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYTAILONSEPARATION_H__8FDA7737_1433_4D2E_B283_A91906D0FC79__INCLUDED_)
#define AFX_LOGACSETPROXIMITYTAILONSEPARATION_H__8FDA7737_1433_4D2E_B283_A91906D0FC79__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityTailOnSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityTailOnSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityTailOnSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityTailOnSeparation& rMsg);

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

	CLogacSetProximityTailOnSeparation(const CLogacSetProximityTailOnSeparation& oldMessage);
	CLogacSetProximityTailOnSeparation& operator=(const CLogacSetProximityTailOnSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double	m_Separation;	// Maximum separation at which bonds form
};

#endif // !defined(AFX_LOGACSETPROXIMITYTAILONSEPARATION_H__8FDA7737_1433_4D2E_B283_A91906D0FC79__INCLUDED_)
