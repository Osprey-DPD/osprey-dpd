// LogSelectACNAsTarget.h: interface for the CLogSelectACNAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTACNASTARGET_H__21FDC9FB_DF87_4CE3_BE2C_0154A0F0148B__INCLUDED_)
#define AFX_LOGSELECTACNASTARGET_H__21FDC9FB_DF87_4CE3_BE2C_0154A0F0148B__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectACNAsTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectACNAsTarget(long time, const zString cmdTarget, const zString acnType);

	virtual ~CLogSelectACNAsTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectACNAsTarget& rMsg);

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

	CLogSelectACNAsTarget(const CLogSelectACNAsTarget& oldMessage);
	CLogSelectACNAsTarget& operator=(const CLogSelectACNAsTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;			// Command target name
	const zString m_ACNType;			// ACN name
};

#endif // !defined(AFX_LOGSELECTACNASTARGET_H__21FDC9FB_DF87_4CE3_BE2C_0154A0F0148B__INCLUDED_)
