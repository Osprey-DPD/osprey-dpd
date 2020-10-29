// LogacSetConstantFreeActiveBondConcentration.h: interface for the CLogacSetConstantFreeActiveBondConcentration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__8FED3C17_DCF0_4638_B770_0DBBB3BEE2B6__INCLUDED_)
#define AFX_LOGACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__8FED3C17_DCF0_4638_B770_0DBBB3BEE2B6__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetConstantFreeActiveBondConcentration : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetConstantFreeActiveBondConcentration(long time, const zString type, double fraction, long actives, long phantoms, long number);

	virtual ~CLogacSetConstantFreeActiveBondConcentration();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetConstantFreeActiveBondConcentration& rMsg);

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

	CLogacSetConstantFreeActiveBondConcentration(const CLogacSetConstantFreeActiveBondConcentration& oldMessage);
	CLogacSetConstantFreeActiveBondConcentration& operator=(const CLogacSetConstantFreeActiveBondConcentration& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		    // Type of active cell network modified
    const double    m_Fraction;         // Constant fraction of all active bonds
    const long		m_ActiveBondTotal;  // Constant number of active bonds
    const long		m_PhantomBondTotal; // Number of phantom bonds
	const long		m_PhantomsPerEvent;	// Number of phantom bonds created per event
};

#endif // !defined(AFX_LOGACSETCONSTANTFREEACTIVEBONDCONCENTRATION_H__8FED3C17_DCF0_4638_B770_0DBBB3BEE2B6__INCLUDED_)
