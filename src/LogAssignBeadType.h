// LogAssignBeadType.h: interface for the CLogAssignBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGASSIGNBEADTYPE_H__EB576021_DC1B_41F9_8832_F9259AE5E3E6__INCLUDED_)
#define AFX_LOGASSIGNBEADTYPE_H__EB576021_DC1B_41F9_8832_F9259AE5E3E6__INCLUDED_


#include "LogConstraintMessage.h"

class CLogAssignBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogAssignBeadType(long time, const zString cmdTarget, 
					   const zString oldName, long oldType, 
					   const zString newName, long newType);

	virtual ~CLogAssignBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogAssignBeadType& rMsg);

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

	CLogAssignBeadType(const CLogAssignBeadType& oldMessage);
	CLogAssignBeadType& operator=(const CLogAssignBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Original bead name
	const long      m_OldType;		// Original bead type
	const zString	m_NewName;		// Bead type's new name
	const long      m_NewType;		// New bead type
};

#endif // !defined(AFX_LOGASSIGNBEADTYPE_H__EB576021_DC1B_41F9_8832_F9259AE5E3E6__INCLUDED_)
