// LogctPolymerisePolymersInTarget.h: interface for the CLogctPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTPOLYMERISEPOLYMERSINTARGET_H__5D1336E8_B27E_47A0_853E_F95CF546AC8A__INCLUDED_)
#define AFX_LOGCTPOLYMERISEPOLYMERSINTARGET_H__5D1336E8_B27E_47A0_853E_F95CF546AC8A__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctPolymerisePolymersInTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctPolymerisePolymersInTarget(long time, const zString cmdTarget, long polymerTotal, long bondTotal, 
									 long maxBonds, double range, double fraction, double sprConst, double unStrLength);

	CLogctPolymerisePolymersInTarget(long time, const zString cmdTarget, const zString bondName, long bondType,
                                     long polymerTotal, long bondTotal, 
									 long maxBonds, double range, double fraction, double sprConst, double unStrLength);

	virtual ~CLogctPolymerisePolymersInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctPolymerisePolymersInTarget& rMsg);

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

	CLogctPolymerisePolymersInTarget(const CLogctPolymerisePolymersInTarget& oldMessage);
	CLogctPolymerisePolymersInTarget& operator=(const CLogctPolymerisePolymersInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CommandTarget;	// Name of command target holding polymers
	const zString	m_BondName;	        // Name of bonds created to bind polymers
    const long      m_BondType;         // Numeric type of bonds 
	const long		m_PolymerTotal;		// Number of polymers bound
	const long      m_BondTotal;		// Number of bonds created
	const long		m_MaxBonds;			// Maximum number of bonds per polymer
	const double    m_Range;			// Fraction of neighbours that are bound
	const double    m_Fraction;			// Fraction of neighbours that are bound
	const double	m_SpringConstant;	// Hookean bond parameters
	const double	m_UnstretchedLength;

};

#endif // !defined(AFX_LOGCTPOLYMERISEPOLYMERSINTARGET_H__5D1336E8_B27E_47A0_853E_F95CF546AC8A__INCLUDED_)
