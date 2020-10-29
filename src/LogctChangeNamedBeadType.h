// LogctChangeNamedBeadType.h: interface for the CLogctChangeNamedBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCHANGENAMEDBEADTYPE_H__073605E9_399C_4DF5_9FB0_D0D52CE2DD2D__INCLUDED_)
#define AFX_LOGCTCHANGENAMEDBEADTYPE_H__073605E9_399C_4DF5_9FB0_D0D52CE2DD2D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctChangeNamedBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctChangeNamedBeadType(long time, const zString cmdTarget, 
					   const zString oldName, const zString newName, 
					   long oldType, long newType);

	virtual ~CLogctChangeNamedBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctChangeNamedBeadType& rMsg);

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

	CLogctChangeNamedBeadType(const CLogctChangeNamedBeadType& oldMessage);
	CLogctChangeNamedBeadType& operator=(const CLogctChangeNamedBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Bead type's original name
	const zString	m_NewName;		// Bead type's new name
	const long      m_OldType;		// Original bead type
	const long      m_NewType;		// New bead type
};

#endif // !defined(AFX_LOGCTCHANGENAMEDBEADTYPE_H__073605E9_399C_4DF5_9FB0_D0D52CE2DD2D__INCLUDED_)
