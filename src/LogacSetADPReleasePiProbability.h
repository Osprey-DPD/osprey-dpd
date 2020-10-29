// LogacSetADPReleasePiProbability.h: interface for the CLogacSetADPReleasePiProbability class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETADPRELEASEPIPROBABILITY_H__B8B87D43_989C_44A6_9B3F_1FE8C8EE12F3__INCLUDED_)
#define AFX_LOGACSETADPRELEASEPIPROBABILITY_H__B8B87D43_989C_44A6_9B3F_1FE8C8EE12F3__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetADPReleasePiProbability : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetADPReleasePiProbability(long time, const zString type, double rate);

	virtual ~CLogacSetADPReleasePiProbability();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetADPReleasePiProbability& rMsg);

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

	CLogacSetADPReleasePiProbability(const CLogacSetADPReleasePiProbability& oldMessage);
	CLogacSetADPReleasePiProbability& operator=(const CLogacSetADPReleasePiProbability& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	// Type of active cell network modified
	const double	m_Rate;	    // Probability per time-step for transition
};

#endif // !defined(AFX_LOGACSETADPRELEASEPIPROBABILITY_H__B8B87D43_989C_44A6_9B3F_1FE8C8EE12F3__INCLUDED_)
