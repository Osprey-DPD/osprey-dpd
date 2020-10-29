// LogctPolymerisePolymerTripleInTarget.h: interface for the CLogctPolymerisePolymerTripleInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTPOLYMERISEPOLYMERTRIPLEINTARGET_H__B74F5138_D8CD_49AF_A2C8_51768AA6A4E7__INCLUDED_)
#define AFX_LOGCTPOLYMERISEPOLYMERTRIPLEINTARGET_H__B74F5138_D8CD_49AF_A2C8_51768AA6A4E7__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctPolymerisePolymerTripleInTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctPolymerisePolymerTripleInTarget(long time, const zString cmdTarget, 
					   const zString polyName, long polyType, long bondTotal, 
					   double sprConst, double unStrLength);

	virtual ~CLogctPolymerisePolymerTripleInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctPolymerisePolymerTripleInTarget& rMsg);

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

	CLogctPolymerisePolymerTripleInTarget(const CLogctPolymerisePolymerTripleInTarget& oldMessage);
	CLogctPolymerisePolymerTripleInTarget& operator=(const CLogctPolymerisePolymerTripleInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CommandTarget;	// Name of command target holding polymers
	const zString	m_PolymerName;		// Name of polymerised polymers
	const long      m_PolymerType;		// Integer type of polymers
	const long      m_BondTotal;		// Number of bonds connecting polymers
	const double	m_SpringConstant;	// Hookean bond parameters
	const double	m_UnstretchedLength;

};

#endif // !defined(AFX_LOGCTPOLYMERISEPOLYMERTRIPLEINTARGET_H__B74F5138_D8CD_49AF_A2C8_51768AA6A4E7__INCLUDED_)
