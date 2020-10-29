// LogacSetProximityHeadOnSeparation.h: interface for the CLogacSetProximityHeadOnSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYHEADONSEPARATION_H__AFE0258D_FB58_4BC9_AE95_B599A4336B0D__INCLUDED_)
#define AFX_LOGACSETPROXIMITYHEADONSEPARATION_H__AFE0258D_FB58_4BC9_AE95_B599A4336B0D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityHeadOnSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityHeadOnSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityHeadOnSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityHeadOnSeparation& rMsg);

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

	CLogacSetProximityHeadOnSeparation(const CLogacSetProximityHeadOnSeparation& oldMessage);
	CLogacSetProximityHeadOnSeparation& operator=(const CLogacSetProximityHeadOnSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double	m_Separation;	// Maximum separation at which bonds form
};

#endif // !defined(AFX_LOGACSETPROXIMITYHEADONSEPARATION_H__AFE0258D_FB58_4BC9_AE95_B599A4336B0D__INCLUDED_)
