// LogctTargetVisible.h: interface for the CLogctTargetVisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETVISIBLE_H__627DBD9C_A1C5_4D65_B656_4BB6EFD5EF81__INCLUDED_)
#define AFX_LOGCTTARGETVISIBLE_H__627DBD9C_A1C5_4D65_B656_4BB6EFD5EF81__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetVisible : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetVisible(long time, const zString cmdTarget);

	virtual ~CLogctTargetVisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetVisible& rMsg);

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

	CLogctTargetVisible(const CLogctTargetVisible& oldMessage);
	CLogctTargetVisible& operator=(const CLogctTargetVisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
};

#endif // !defined(AFX_LOGCTTARGETVISIBLE_H__627DBD9C_A1C5_4D65_B656_4BB6EFD5EF81__INCLUDED_)
