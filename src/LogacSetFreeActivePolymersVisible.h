// LogacSetFreeActivePolymersVisible.h: interface for the CLogacSetFreeActivePolymersVisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLE_H__2A90E26E_71CF_4579_888E_6F0424ABE09B__INCLUDED_)
#define AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLE_H__2A90E26E_71CF_4579_888E_6F0424ABE09B__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSetFreeActivePolymersVisible : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFreeActivePolymersVisible(long time, const zString type);

	virtual ~CLogacSetFreeActivePolymersVisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFreeActivePolymersVisible& rMsg);

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

	CLogacSetFreeActivePolymersVisible(const CLogacSetFreeActivePolymersVisible& oldMessage);
	CLogacSetFreeActivePolymersVisible& operator=(const CLogacSetFreeActivePolymersVisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLE_H__2A90E26E_71CF_4579_888E_6F0424ABE09B__INCLUDED_)
