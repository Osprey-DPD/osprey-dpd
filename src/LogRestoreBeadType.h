// LogRestoreBeadType.h: interface for the CLogRestoreBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRESTOREBEADTYPE_H__1E22A218_26FE_4AAA_B011_BF3442EA2ABC__INCLUDED_)
#define AFX_LOGRESTOREBEADTYPE_H__1E22A218_26FE_4AAA_B011_BF3442EA2ABC__INCLUDED_


#include "LogConstraintMessage.h"

class CLogRestoreBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogRestoreBeadType(long time, const zString cmdTarget, 
					   const zString beadName, long oldType, long newType);

	virtual ~CLogRestoreBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogRestoreBeadType& rMsg);

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

	CLogRestoreBeadType(const CLogRestoreBeadType& oldMessage);
	CLogRestoreBeadType& operator=(const CLogRestoreBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Command target's name
	const zString	m_BeadName;		// Bead type's original name
	const long      m_OldType;		// Previous bead type
	const long      m_NewType;		// New bead type

};

#endif // !defined(AFX_LOGRESTOREBEADTYPE_H__1E22A218_26FE_4AAA_B011_BF3442EA2ABC__INCLUDED_)
