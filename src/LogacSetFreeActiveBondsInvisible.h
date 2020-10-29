// LogacSetFreeActiveBondsInvisible.h: interface for the CLogacSetFreeActiveBondsInvisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFREEACTIVEBONDSINVISIBLE_H__E4A89419_4C23_416B_8801_F5EB1696970F__INCLUDED_)
#define AFX_LOGACSETFREEACTIVEBONDSINVISIBLE_H__E4A89419_4C23_416B_8801_F5EB1696970F__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSetFreeActiveBondsInvisible : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFreeActiveBondsInvisible(long time, const zString type);

	virtual ~CLogacSetFreeActiveBondsInvisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFreeActiveBondsInvisible& rMsg);

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

	CLogacSetFreeActiveBondsInvisible(const CLogacSetFreeActiveBondsInvisible& oldMessage);
	CLogacSetFreeActiveBondsInvisible& operator=(const CLogacSetFreeActiveBondsInvisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSETFREEACTIVEBONDSINVISIBLE_H__E4A89419_4C23_416B_8801_F5EB1696970F__INCLUDED_)
