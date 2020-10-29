// LogSelectACNCurrentPolymersAsTarget.h: interface for the CLogSelectACNCurrentPolymersAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTACNCURRENTPOLYMERSASTARGET_H__D57CC132_937E_4A3E_94A8_EC6296711E13__INCLUDED_)
#define AFX_LOGSELECTACNCURRENTPOLYMERSASTARGET_H__D57CC132_937E_4A3E_94A8_EC6296711E13__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectACNCurrentPolymersAsTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectACNCurrentPolymersAsTarget(long time, const zString cmdTarget, const zString acnType, 
							   const zString polyName, long polyType, long freePolymerTotal, long polyTotal);

	virtual ~CLogSelectACNCurrentPolymersAsTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectACNCurrentPolymersAsTarget& rMsg);

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

	CLogSelectACNCurrentPolymersAsTarget(const CLogSelectACNCurrentPolymersAsTarget& oldMessage);
	CLogSelectACNCurrentPolymersAsTarget& operator=(const CLogSelectACNCurrentPolymersAsTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;			// Command target name
	const zString m_ACNType;			// ACN name
	const zString m_PolymerName;		// Name of polymer type held in command target
	const long	  m_PolymerType;		// Polymer type held in command target
	const long	  m_FreePolymerTotal;	// Number of free polymers in command target
	const long	  m_PolymerTotal;		// Number of CPolymers in command target
};

#endif // !defined(AFX_LOGSELECTACNCURRENTPOLYMERSASTARGET_H__D57CC132_937E_4A3E_94A8_EC6296711E13__INCLUDED_)
