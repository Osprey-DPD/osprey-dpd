// LogctChangeBondType.h: interface for the CLogctChangeBondType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCHANGEBONDTYPE_H__D45DCD33_C3FF_4841_9767_7597A1CBCC73__INCLUDED_)
#define AFX_LOGCTCHANGEBONDTYPE_H__D45DCD33_C3FF_4841_9767_7597A1CBCC73__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctChangeBondType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctChangeBondType(long time, const zString cmdTarget, 
					   const zString oldName, long oldType, long newType);

	virtual ~CLogctChangeBondType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctChangeBondType& rMsg);

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

	CLogctChangeBondType(const CLogctChangeBondType& oldMessage);
	CLogctChangeBondType& operator=(const CLogctChangeBondType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;
	const long      m_OldType;
	const long      m_NewType;
};

#endif // !defined(AFX_LOGCTCHANGEBONDTYPE_H__D45DCD33_C3FF_4841_9767_7597A1CBCC73__INCLUDED_)
