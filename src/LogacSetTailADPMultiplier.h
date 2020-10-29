// LogacSetTailADPMultiplier.h: interface for the CLogacSetTailADPMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETTAILADPMULTIPLIER_H__D6835EC4_5FDB_4427_A9CA_EFCFD67A6E84__INCLUDED_)
#define AFX_LOGACSETTAILADPMULTIPLIER_H__D6835EC4_5FDB_4427_A9CA_EFCFD67A6E84__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetTailADPMultiplier : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetTailADPMultiplier(long time, const zString type, double factor);

	virtual ~CLogacSetTailADPMultiplier();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetTailADPMultiplier& rMsg);

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

	CLogacSetTailADPMultiplier(const CLogacSetTailADPMultiplier& oldMessage);
	CLogacSetTailADPMultiplier& operator=(const CLogacSetTailADPMultiplier& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	    // Type of active cell network modified
	const double	m_Multiplier;	// Multiplier of the transition's basal probability
};

#endif // !defined(AFX_LOGACSETTAILADPMULTIPLIER_H__D6835EC4_5FDB_4427_A9CA_EFCFD67A6E84__INCLUDED_)
