// LogctChangeBeadType.h: interface for the CLogctChangeBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCHANGEBEADTYPE_H__9C061C89_E679_47C5_812D_08F762939768__INCLUDED_)
#define AFX_LOGCTCHANGEBEADTYPE_H__9C061C89_E679_47C5_812D_08F762939768__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctChangeBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctChangeBeadType(long time, const zString cmdTarget, 
					   const zString oldName, const zString newName, long oldType, long newType);

	virtual ~CLogctChangeBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctChangeBeadType& rMsg);

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

	CLogctChangeBeadType(const CLogctChangeBeadType& oldMessage);
	CLogctChangeBeadType& operator=(const CLogctChangeBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Bead type's original name
	const zString	m_NewName;		// Bead type's new name
	const long      m_OldType;		// Original bead type
	const long      m_NewType;		// New bead type
};

#endif // !defined(AFX_LOGCTCHANGEBEADTYPE_H__9C061C89_E679_47C5_812D_08F762939768__INCLUDED_)
