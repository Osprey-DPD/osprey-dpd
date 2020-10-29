// LogacSetPolymerFormsEvent.h: interface for the CLogacSetPolymerFormsEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPOLYMERFORMSEVENT_H__9DB4576A_DA0A_4739_BE4D_96BE12972B1F__INCLUDED_)
#define AFX_LOGACSETPOLYMERFORMSEVENT_H__9DB4576A_DA0A_4739_BE4D_96BE12972B1F__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetPolymerFormsEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetPolymerFormsEvent(long time, const zString type, long duration,
		                       double range, double spring, double length);

	virtual ~CLogacSetPolymerFormsEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetPolymerFormsEvent& rMsg);

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

	CLogacSetPolymerFormsEvent(const CLogacSetPolymerFormsEvent& oldMessage);
	CLogacSetPolymerFormsEvent& operator=(const CLogacSetPolymerFormsEvent& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;			// Type of active cell network modified
	const long		m_Duration;			// Time steps allowed for polymer formation
	const double    m_Range;	        // Max separation for second bond selection
	const double    m_SpringConstant;	// Final value of transient bond strength
	const double    m_Length;	        // Final value of transient bond unstretched length
};

#endif // !defined(AFX_LOGACSETPOLYMERFORMSEVENT_H__9DB4576A_DA0A_4739_BE4D_96BE12972B1F__INCLUDED_)
