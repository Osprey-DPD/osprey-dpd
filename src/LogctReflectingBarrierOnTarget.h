// LogctReflectingBarrierOnTarget.h: interface for the CLogctReflectingBarrierOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTREFLECTINGBARRIERONTARGET_H__4622ad20_e5f3_41d2_ba97_07db543b7b59__INCLUDED_)
#define AFX_LOGCTREFLECTINGBARRIERONTARGET_H__4622ad20_e5f3_41d2_ba97_07db543b7b59__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctReflectingBarrierOnTarget : public CLogConstraintMessage
{
	// ****************************************
	// Construction/Destruction
public:

    CLogctReflectingBarrierOnTarget(long time, const zString targetlabel, const zString decLabel,
								long xn, long yn, long zn, double mag);

	virtual ~CLogctReflectingBarrierOnTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctReflectingBarrierOnTarget& rMsg);

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

    CLogctReflectingBarrierOnTarget(const CLogctReflectingBarrierOnTarget& oldMessage);
    CLogctReflectingBarrierOnTarget& operator=(const CLogctReflectingBarrierOnTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
	const zString	m_DecLabel;		// Command wrapper label 
	const long	    m_X;
	const long	    m_Y;			// Barrier normal vector
	const long	    m_Z;
	const double	m_Magnitude;	// Fractional position along normal axis

};

#endif // !defined(AFX_LOGCTREFLECTINGBARRIERONTARGET_H__4622ad20_e5f3_41d2_ba97_07db543b7b59__INCLUDED_)
