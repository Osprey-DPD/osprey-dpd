// LogSelectACNCurrentBondsAsTarget.h: interface for the CLogSelectACNCurrentBondsAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTACNCURRENTBONDSASTARGET_H__3D6C9389_2E88_4FF2_93B4_05763133F37A__INCLUDED_)
#define AFX_LOGSELECTACNCURRENTBONDSASTARGET_H__3D6C9389_2E88_4FF2_93B4_05763133F37A__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectACNCurrentBondsAsTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectACNCurrentBondsAsTarget(long time, const zString cmdTarget, const zString acnType, 
							   const zString polyName, long polyType, long freeBondTotal, long polyTotal);

	virtual ~CLogSelectACNCurrentBondsAsTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectACNCurrentBondsAsTarget& rMsg);

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

	CLogSelectACNCurrentBondsAsTarget(const CLogSelectACNCurrentBondsAsTarget& oldMessage);
	CLogSelectACNCurrentBondsAsTarget& operator=(const CLogSelectACNCurrentBondsAsTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;		// Command target name
	const zString m_ACNType;		// ACN name
	const zString m_PolymerName;	// Name of polymer type held in command target
	const long	  m_PolymerType;	// Polymer type held in command target
	const long	  m_FreeBondTotal;	// Number of free bonds in command target
	const long	  m_PolymerTotal;	// Number of CPolymers in command target
};

#endif // !defined(AFX_LOGSELECTACNCURRENTBONDSASTARGET_H__3D6C9389_2E88_4FF2_93B4_05763133F37A__INCLUDED_)
