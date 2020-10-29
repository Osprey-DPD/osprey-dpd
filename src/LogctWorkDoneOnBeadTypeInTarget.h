// LogctWorkDoneOnBeadTypeInTarget.h: interface for the CLogctWorkDoneOnBeadTypeInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTWORKDONEONBEADTYPEINTARGET_H__F65A0995_FBE4_4EA0_924B_3CFFB7274D60__INCLUDED_)
#define AFX_LOGCTWORKDONEONBEADTYPEINTARGET_H__F65A0995_FBE4_4EA0_924B_3CFFB7274D60__INCLUDED_

#include "LogAnalysisMessage.h"

class CLogctWorkDoneOnBeadTypeInTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctWorkDoneOnBeadTypeInTarget(long time, const zString cmdTarget,
									 const zString name, long type, long total, double work);

	virtual ~CLogctWorkDoneOnBeadTypeInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctWorkDoneOnBeadTypeInTarget& rMsg);

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

	CLogctWorkDoneOnBeadTypeInTarget(const CLogctWorkDoneOnBeadTypeInTarget& oldMessage);
	CLogctWorkDoneOnBeadTypeInTarget& operator=(const CLogctWorkDoneOnBeadTypeInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Name of command target
	const zString m_BeadName;		// Name of specified bead type in target
	const long	  m_BeadType;		// Specified bead type
	const long    m_BeadTotal;		// Number of beads of specified type in target
	const double  m_Work;			// Sum of F.dx over all beads of specified type in target

};

#endif // !defined(AFX_LOGCTWORKDONEONBEADTYPEINTARGET_H__F65A0995_FBE4_4EA0_924B_3CFFB7274D60__INCLUDED_)
