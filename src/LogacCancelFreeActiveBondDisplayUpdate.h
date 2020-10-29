// LogCLogacCancelFreeActiveBondDisplayUpdate.h: interface for the CLogCLogacCancelFreeActiveBondDisplayUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCLogacCancelFreeActiveBondDisplayUpdate_H__9ED8FA0B_5E58_4E6F_9FA3_59E8C36A4486__INCLUDED_)
#define AFX_LOGCLogacCancelFreeActiveBondDisplayUpdate_H__9ED8FA0B_5E58_4E6F_9FA3_59E8C36A4486__INCLUDED_


#include "LogInfoMessage.h"

class CLogacCancelFreeActiveBondDisplayUpdate : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacCancelFreeActiveBondDisplayUpdate(long time, const zString type);

	virtual ~CLogacCancelFreeActiveBondDisplayUpdate();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacCancelFreeActiveBondDisplayUpdate& rMsg);

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

	CLogacCancelFreeActiveBondDisplayUpdate(const CLogacCancelFreeActiveBondDisplayUpdate& oldMessage);
	CLogacCancelFreeActiveBondDisplayUpdate& operator=(const CLogacCancelFreeActiveBondDisplayUpdate& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGCLogacCancelFreeActiveBondDisplayUpdate_H__9ED8FA0B_5E58_4E6F_9FA3_59E8C36A4486__INCLUDED_)
