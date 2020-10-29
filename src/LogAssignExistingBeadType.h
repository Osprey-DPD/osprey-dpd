// LogAssignExistingBeadType.h: interface for the CLogAssignExistingBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGASSIGNEXISTINGBEADTYPE_H__D6DF59C0_FDAE_4A8B_B23C_5E684AA314BC__INCLUDED_)
#define AFX_LOGASSIGNEXISTINGBEADTYPE_H__D6DF59C0_FDAE_4A8B_B23C_5E684AA314BC__INCLUDED_


#include "LogConstraintMessage.h"

class CLogAssignExistingBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogAssignExistingBeadType(long time, const zString cmdTarget, 
					   const zString oldName, long oldType, 
					   const zString newName, long newType);

	virtual ~CLogAssignExistingBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogAssignExistingBeadType& rMsg);

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

	CLogAssignExistingBeadType(const CLogAssignExistingBeadType& oldMessage);
	CLogAssignExistingBeadType& operator=(const CLogAssignExistingBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Original bead name
	const long      m_OldType;		// Original bead type
	const zString	m_NewName;		// Bead type's new name
	const long      m_NewType;		// New bead type
};

#endif // !defined(AFX_LOGASSIGNEXISTINGBEADTYPE_H__D6DF59C0_FDAE_4A8B_B23C_5E684AA314BC__INCLUDED_)
