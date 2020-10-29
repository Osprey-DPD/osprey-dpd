// LogSelectBeadTypeInRegion.h: interface for the CLogSelectBeadTypeInRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTBEADTYPEINREGION_H__E15F829E_61F4_4AE8_9FDD_C3D2D2A60C72__INCLUDED_)
#define AFX_LOGSELECTBEADTYPEINREGION_H__E15F829E_61F4_4AE8_9FDD_C3D2D2A60C72__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectBeadTypeInRegion : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectBeadTypeInRegion(long time, const zString cmdTarget, const zString region, 
							   const zString beadName, long beadType, long beadTotal);

	virtual ~CLogSelectBeadTypeInRegion();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectBeadTypeInRegion& rMsg);

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

	CLogSelectBeadTypeInRegion(const CLogSelectBeadTypeInRegion& oldMessage);
	CLogSelectBeadTypeInRegion& operator=(const CLogSelectBeadTypeInRegion& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Command target name
	const zString m_Region	;		// Geometric region of command target
	const zString m_BeadName;		// Bead name
	const long	  m_BeadType;		// Bead type held in command target
	const long	  m_BeadTotal;		// Number of beads in command target
};

#endif // !defined(AFX_LOGSELECTBEADTYPEINREGION_H__E15F829E_61F4_4AE8_9FDD_C3D2D2A60C72__INCLUDED_)
