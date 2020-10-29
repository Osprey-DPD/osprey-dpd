// LogBondStrengthChanged.h: interface for the CLogBondStrengthChanged class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBONDSTRENGTHCHANGED_H__CE0252E6_DFFC_46A4_B814_CBF16851ECBE__INCLUDED_)
#define AFX_LOGBONDSTRENGTHCHANGED_H__CE0252E6_DFFC_46A4_B814_CBF16851ECBE__INCLUDED_


#include "LogConstraintMessage.h"

class CLogBondStrengthChanged : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogBondStrengthChanged(long time, const zString name, 
							long type, double sprConst, double unStrLength);

	virtual ~CLogBondStrengthChanged();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogBondStrengthChanged& rMsg);

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

	CLogBondStrengthChanged(const CLogBondStrengthChanged& oldMessage);
	CLogBondStrengthChanged& operator=(const CLogBondStrengthChanged& rhs);

	// ****************************************
	// Data members
private:
	const zString m_Name;				// Bond name, e.g., HB
	const long    m_Type;				// Bond type, e.g., 0
	const double  m_SpringConstant;
	const double  m_UnstretchedLength;
};

#endif // !defined(AFX_LOGBONDSTRENGTHCHANGED_H__CE0252E6_DFFC_46A4_B814_CBF16851ECBE__INCLUDED_)
