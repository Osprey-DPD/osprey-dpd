// LogacSetProximityHeadOffSeparation.h: interface for the CLogacSetProximityHeadOffSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYHEADOFFSEPARATION_H__3E35EF89_7274_47C0_A78D_85E107F92ACC__INCLUDED_)
#define AFX_LOGACSETPROXIMITYHEADOFFSEPARATION_H__3E35EF89_7274_47C0_A78D_85E107F92ACC__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityHeadOffSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityHeadOffSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityHeadOffSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityHeadOffSeparation& rMsg);

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

	CLogacSetProximityHeadOffSeparation(const CLogacSetProximityHeadOffSeparation& oldMessage);
	CLogacSetProximityHeadOffSeparation& operator=(const CLogacSetProximityHeadOffSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double	m_Separation;	// Maximum separation at which bonds form
};

#endif // !defined(AFX_LOGACSETPROXIMITYHEADOFFSEPARATION_H__3E35EF89_7274_47C0_A78D_85E107F92ACC__INCLUDED_)
