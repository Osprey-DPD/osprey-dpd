// LogacSetBondBindsForwardConeToPolymerHeadEvent.h: interface for the CLogacSetBondBindsForwardConeToPolymerHeadEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERHEADEVENT_H__3491C67D_1854_4E25_9C4A_4C2FA5C4BE85__INCLUDED_)
#define AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERHEADEVENT_H__3491C67D_1854_4E25_9C4A_4C2FA5C4BE85__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetBondBindsForwardConeToPolymerHeadEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetBondBindsForwardConeToPolymerHeadEvent(long time, const zString type, long duration,
		                       double range, double halfAngle, double spring, double length);

	virtual ~CLogacSetBondBindsForwardConeToPolymerHeadEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetBondBindsForwardConeToPolymerHeadEvent& rMsg);

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

	CLogacSetBondBindsForwardConeToPolymerHeadEvent(const CLogacSetBondBindsForwardConeToPolymerHeadEvent& oldMessage);
	CLogacSetBondBindsForwardConeToPolymerHeadEvent& operator=(const CLogacSetBondBindsForwardConeToPolymerHeadEvent& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;			// Type of active cell network modified
	const long		m_Duration;			// Time steps allowed for polymer formation
	const double    m_Range;	        // Height of conical capture zone
	const double    m_ConeAngle;	    // Half-angle of conical capture zone (radians)
	const double    m_SpringConstant;	// Final value of transient bond strength
	const double    m_Length;	        // Final value of transient bond unstretched length
};

#endif // !defined(AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERHEADEVENT_H__3491C67D_1854_4E25_9C4A_4C2FA5C4BE85__INCLUDED_)
