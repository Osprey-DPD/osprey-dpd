// LogctAddTargetToComposite.h: interface for the CLogctAddTargetToComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTADDTARGETTOCOMPOSITE_H__2580CDA1_2E8B_42C1_A9D5_3BB245F9597C__INCLUDED_)
#define AFX_LOGCTADDTARGETTOCOMPOSITE_H__2580CDA1_2E8B_42C1_A9D5_3BB245F9597C__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctAddTargetToComposite : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctAddTargetToComposite(long time, const zString destLabel, 
							   const zString sourceLabel, long total);

	virtual ~CLogctAddTargetToComposite();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctAddTargetToComposite& rMsg);

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

	CLogctAddTargetToComposite(const CLogctAddTargetToComposite& oldMessage);
	CLogctAddTargetToComposite& operator=(const CLogctAddTargetToComposite& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Destination;	// Composite command target's name
	const zString m_Source;			// Name of target to add to composite
	const long    m_Total;			// Targets held in composite target
};

#endif // !defined(AFX_LOGCTADDTARGETTOCOMPOSITE_H__2580CDA1_2E8B_42C1_A9D5_3BB245F9597C__INCLUDED_)
