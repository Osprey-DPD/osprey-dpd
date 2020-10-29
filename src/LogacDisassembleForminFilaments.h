// LogacDisassembleForminFilaments.h: interface for the CLogacDisassembleForminFilaments class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACDISASSEMBLEFORMINFILAMENTS_H__F865AA1A_EA94_453F_B90D_B452E4427258__INCLUDED_)
#define AFX_LOGACDISASSEMBLEFORMINFILAMENTS_H__F865AA1A_EA94_453F_B90D_B452E4427258__INCLUDED_


#include "LogInfoMessage.h"

class CLogacDisassembleForminFilaments : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacDisassembleForminFilaments(long time, const zString type);

	virtual ~CLogacDisassembleForminFilaments();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacDisassembleForminFilaments& rMsg);

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

	CLogacDisassembleForminFilaments(const CLogacDisassembleForminFilaments& oldMessage);
	CLogacDisassembleForminFilaments& operator=(const CLogacDisassembleForminFilaments& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACDISASSEMBLEFORMINFILAMENTS_H__F865AA1A_EA94_453F_B90D_B452E4427258__INCLUDED_)
