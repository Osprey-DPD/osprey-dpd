// LogSelectPolymerTypeInRegion.h: interface for the CLogSelectPolymerTypeInRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTPOLYMERTYPEINREGION_H__589EFD21_9831_42EE_972E_9DAE8EED488C__INCLUDED_)
#define AFX_LOGSELECTPOLYMERTYPEINREGION_H__589EFD21_9831_42EE_972E_9DAE8EED488C__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectPolymerTypeInRegion : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectPolymerTypeInRegion(long time, const zString cmdTarget, const zString region, 
							   const zString polyName, long polyType, long polyTotal);

	virtual ~CLogSelectPolymerTypeInRegion();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectPolymerTypeInRegion& rMsg);

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

	CLogSelectPolymerTypeInRegion(const CLogSelectPolymerTypeInRegion& oldMessage);
	CLogSelectPolymerTypeInRegion& operator=(const CLogSelectPolymerTypeInRegion& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Command target name
	const zString m_Region	;		// Geometric region of command target
	const zString m_PolymerName;	// Polymer name
	const long	  m_PolymerType;	// Polymer type held in command target
	const long	  m_PolymerTotal;	// Number of Polymer in command target
};

#endif // !defined(AFX_LOGSELECTPOLYMERTYPEINREGION_H__589EFD21_9831_42EE_972E_9DAE8EED488C__INCLUDED_)
