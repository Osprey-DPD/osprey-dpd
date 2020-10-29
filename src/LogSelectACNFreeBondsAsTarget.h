// LogSelectACNFreeBondsAsTarget.h: interface for the CLogSelectACNFreeBondsAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTACNFREEBONDSASTARGET_H__5EB4EF04_330D_433B_8876_0785850AD58D__INCLUDED_)
#define AFX_LOGSELECTACNFREEBONDSASTARGET_H__5EB4EF04_330D_433B_8876_0785850AD58D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectACNFreeBondsAsTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectACNFreeBondsAsTarget(long time, const zString cmdTarget, const zString acnType);

	virtual ~CLogSelectACNFreeBondsAsTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectACNFreeBondsAsTarget& rMsg);

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

	CLogSelectACNFreeBondsAsTarget(const CLogSelectACNFreeBondsAsTarget& oldMessage);
	CLogSelectACNFreeBondsAsTarget& operator=(const CLogSelectACNFreeBondsAsTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;			// Command target name
	const zString m_ACNType;			// ACN name
};

#endif // !defined(AFX_LOGSELECTACNFREEBONDSASTARGET_H__5EB4EF04_330D_433B_8876_0785850AD58D__INCLUDED_)
