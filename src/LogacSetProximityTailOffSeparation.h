// LogacSetProximityTailOffSeparation.h: interface for the CLogacSetProximityTailOffSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYTAILOFFSEPARATION_H__BF3954A3_829D_43D0_8853_21AA4404FC0A__INCLUDED_)
#define AFX_LOGACSETPROXIMITYTAILOFFSEPARATION_H__BF3954A3_829D_43D0_8853_21AA4404FC0A__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityTailOffSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityTailOffSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityTailOffSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityTailOffSeparation& rMsg);

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

	CLogacSetProximityTailOffSeparation(const CLogacSetProximityTailOffSeparation& oldMessage);
	CLogacSetProximityTailOffSeparation& operator=(const CLogacSetProximityTailOffSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double	m_Separation;	// Maximum separation at which bonds form
};

#endif // !defined(AFX_LOGACSETPROXIMITYTAILOFFSEPARATION_H__BF3954A3_829D_43D0_8853_21AA4404FC0A__INCLUDED_)
