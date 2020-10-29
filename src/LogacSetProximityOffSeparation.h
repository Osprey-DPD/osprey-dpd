// LogacSetProximityOffSeparation.h: interface for the CLogacSetProximityOffSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPROXIMITYOFFSEPARATION_H__A3A2C9F8_7389_4B74_AAC0_76C9DBFE27A8__INCLUDED_)
#define AFX_LOGACSETPROXIMITYOFFSEPARATION_H__A3A2C9F8_7389_4B74_AAC0_76C9DBFE27A8__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetProximityOffSeparation : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetProximityOffSeparation(long time, const zString type, double max);

	virtual ~CLogacSetProximityOffSeparation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetProximityOffSeparation& rMsg);

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

	CLogacSetProximityOffSeparation(const CLogacSetProximityOffSeparation& oldMessage);
	CLogacSetProximityOffSeparation& operator=(const CLogacSetProximityOffSeparation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const double m_Separation;	// Maximum separation at which bonds dissociate
};

#endif // !defined(AFX_LOGACSETPROXIMITYOFFSEPARATION_H__A3A2C9F8_7389_4B74_AAC0_76C9DBFE27A8__INCLUDED_)
