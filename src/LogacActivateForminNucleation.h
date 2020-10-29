// LogacActivateForminNucleation.h: interface for the CLogacActivateForminNucleation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACACTIVATEFORMINNUCLEATION_H__16C956DB_6780_4CCB_9730_C084FDF1E594__INCLUDED_)
#define AFX_LOGACACTIVATEFORMINNUCLEATION_H__16C956DB_6780_4CCB_9730_C084FDF1E594__INCLUDED_


#include "LogInfoMessage.h"

class CLogacActivateForminNucleation : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacActivateForminNucleation(long time, const zString type);

	virtual ~CLogacActivateForminNucleation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacActivateForminNucleation& rMsg);

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

	CLogacActivateForminNucleation(const CLogacActivateForminNucleation& oldMessage);
	CLogacActivateForminNucleation& operator=(const CLogacActivateForminNucleation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACACTIVATEFORMINNUCLEATION_H__16C956DB_6780_4CCB_9730_C084FDF1E594__INCLUDED_)
