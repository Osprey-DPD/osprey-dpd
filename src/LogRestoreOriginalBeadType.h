// LogRestoreOriginalBeadType.h: interface for the CLogRestoreOriginalBeadType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRESTOREORIGINALBEADTYPE_H__B02D84B1_72E8_4949_8FBF_0A4807E19640__INCLUDED_)
#define AFX_LOGRESTOREORIGINALBEADTYPE_H__B02D84B1_72E8_4949_8FBF_0A4807E19640__INCLUDED_


#include "LogConstraintMessage.h"

class CLogRestoreOriginalBeadType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogRestoreOriginalBeadType(long time, const zString cmdTarget, 
					   const zString beadName, long oldType, long newType);

	virtual ~CLogRestoreOriginalBeadType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogRestoreOriginalBeadType& rMsg);

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

	CLogRestoreOriginalBeadType(const CLogRestoreOriginalBeadType& oldMessage);
	CLogRestoreOriginalBeadType& operator=(const CLogRestoreOriginalBeadType& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Command target's name
	const zString	m_BeadName;		// Bead type's original name
	const long      m_OldType;		// Original bead type
	const long      m_NewType;		// New bead type

};
#endif // !defined(AFX_LOGRESTOREORIGINALBEADTYPE_H__B02D84B1_72E8_4949_8FBF_0A4807E19640__INCLUDED_)
