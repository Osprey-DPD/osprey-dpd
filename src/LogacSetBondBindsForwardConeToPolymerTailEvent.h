// LogacSetBondBindsForwardConeToPolymerTailEvent.h: interface for the CLogacSetBondBindsForwardConeToPolymerTailEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__9DC99435_F219_4822_BCC3_67E1AD6725E2__INCLUDED_)
#define AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__9DC99435_F219_4822_BCC3_67E1AD6725E2__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetBondBindsForwardConeToPolymerTailEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetBondBindsForwardConeToPolymerTailEvent(long time, const zString type, long duration,
		                       double range, double halfAngle, double spring, double length);

	virtual ~CLogacSetBondBindsForwardConeToPolymerTailEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetBondBindsForwardConeToPolymerTailEvent& rMsg);

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

	CLogacSetBondBindsForwardConeToPolymerTailEvent(const CLogacSetBondBindsForwardConeToPolymerTailEvent& oldMessage);
	CLogacSetBondBindsForwardConeToPolymerTailEvent& operator=(const CLogacSetBondBindsForwardConeToPolymerTailEvent& rhs);


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

#endif // !defined(AFX_LOGACSETBONDBINDSFORWARDCONETOPOLYMERTAILEVENT_H__9DC99435_F219_4822_BCC3_67E1AD6725E2__INCLUDED_)
