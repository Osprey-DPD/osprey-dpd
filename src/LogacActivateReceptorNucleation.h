// LogacActivateReceptorNucleation.h: interface for the CLogacActivateReceptorNucleation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACACTIVATERECEPTORNUCLEATION_H__8811751d_6452_4d7c_b51a_9edf53db6989__INCLUDED_)
#define AFX_LOGACACTIVATERECEPTORNUCLEATION_H__8811751d_6452_4d7c_b51a_9edf53db6989__INCLUDED_


#include "LogInfoMessage.h"

class CLogacActivateReceptorNucleation : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacActivateReceptorNucleation(long time, const zString type);

	virtual ~CLogacActivateReceptorNucleation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacActivateReceptorNucleation& rMsg);

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

	CLogacActivateReceptorNucleation(const CLogacActivateReceptorNucleation& oldMessage);
	CLogacActivateReceptorNucleation& operator=(const CLogacActivateReceptorNucleation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACACTIVATERECEPTORNUCLEATION_H__8811751d_6452_4d7c_b51a_9edf53db6989__INCLUDED_)
