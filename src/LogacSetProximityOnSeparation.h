// LogacSetProximityOnSeparation.h: interface for the CLogacSetProximityOnSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYONSEPARATION_H__C41A26A8_393A_417F_BEA6_54B828C108A4__INCLUDED_)
#define AFX_LOGACSETPROXIMITYONSEPARATION_H__C41A26A8_393A_417F_BEA6_54B828C108A4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityOnSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityOnSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityOnSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityOnSeparation& rMsg);

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

	CLogacSetProximityOnSeparation(const CLogacSetProximityOnSeparation& oldMessage);
	CLogacSetProximityOnSeparation& operator=(const CLogacSetProximityOnSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double	m_Separation;	// Maximum separation at which bonds form
};

#endif // !defined(AFX_LOGACSETPROXIMITYONSEPARATION_H__C41A26A8_393A_417F_BEA6_54B828C108A4__INCLUDED_)
