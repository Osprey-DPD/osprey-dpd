// LogctChangeNamedBeadTypeInCompositeTarget.h: interface for the CLogctChangeNamedBeadTypeInCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__CC05B2AF_A655_4E29_9BA7_5DE4033A4E54__INCLUDED_)
#define AFX_LOGCTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__CC05B2AF_A655_4E29_9BA7_5DE4033A4E54__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctChangeNamedBeadTypeInCompositeTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctChangeNamedBeadTypeInCompositeTarget(long time, const zString cmdTarget, 
					   const zString oldName, const zString newName, 
					   long oldType, long newType);

	virtual ~CLogctChangeNamedBeadTypeInCompositeTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctChangeNamedBeadTypeInCompositeTarget& rMsg);

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

	CLogctChangeNamedBeadTypeInCompositeTarget(const CLogctChangeNamedBeadTypeInCompositeTarget& oldMessage);
	CLogctChangeNamedBeadTypeInCompositeTarget& operator=(const CLogctChangeNamedBeadTypeInCompositeTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Bead type's original name
	const zString	m_NewName;		// Bead type's new name
	const long      m_OldType;		// Original bead type
	const long      m_NewType;		// New bead type
};

#endif // !defined(AFX_LOGCTCHANGENAMEDBEADTYPEINCOMPOSITETARGET_H__CC05B2AF_A655_4E29_9BA7_5DE4033A4E54__INCLUDED_)
