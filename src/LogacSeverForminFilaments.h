// LogacSeverForminFilaments.h: interface for the CLogacSeverForminFilaments class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSEVERFORMINFILAMENTS_H__B746C5A1_F74B_49E5_A8C5_419998702CB6__INCLUDED_)
#define AFX_LOGACSEVERFORMINFILAMENTS_H__B746C5A1_F74B_49E5_A8C5_419998702CB6__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSeverForminFilaments : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSeverForminFilaments(long time, const zString type);

	virtual ~CLogacSeverForminFilaments();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSeverForminFilaments& rMsg);

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

	CLogacSeverForminFilaments(const CLogacSeverForminFilaments& oldMessage);
	CLogacSeverForminFilaments& operator=(const CLogacSeverForminFilaments& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSEVERFORMINFILAMENTS_H__B746C5A1_F74B_49E5_A8C5_419998702CB6__INCLUDED_)
