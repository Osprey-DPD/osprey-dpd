// LogacSetFreeActivePolymersInvisible.h: interface for the CLogacSetFreeActivePolymersInvisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFREEACTIVEPOLYMERSINVISIBLE_H__4B7D5A01_EB9D_42CA_95C5_467616A340D7__INCLUDED_)
#define AFX_LOGACSETFREEACTIVEPOLYMERSINVISIBLE_H__4B7D5A01_EB9D_42CA_95C5_467616A340D7__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSetFreeActivePolymersInvisible : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFreeActivePolymersInvisible(long time, const zString type);

	virtual ~CLogacSetFreeActivePolymersInvisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFreeActivePolymersInvisible& rMsg);

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

	CLogacSetFreeActivePolymersInvisible(const CLogacSetFreeActivePolymersInvisible& oldMessage);
	CLogacSetFreeActivePolymersInvisible& operator=(const CLogacSetFreeActivePolymersInvisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSETFREEACTIVEPOLYMERSINVISIBLE_H__4B7D5A01_EB9D_42CA_95C5_467616A340D7__INCLUDED_)
