// LogBondStiffnessChanged.h: interface for the CLogBondStiffnessChanged class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBONDSTIFFNESSCHANGED_H__49142103_2A0F_4FB5_8A36_023793585DEC__INCLUDED_)
#define AFX_LOGBONDSTIFFNESSCHANGED_H__49142103_2A0F_4FB5_8A36_023793585DEC__INCLUDED_


#include "LogConstraintMessage.h"

class CLogBondStiffnessChanged : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogBondStiffnessChanged(long time, const zString name, 
							 long type, double modulus, double phi0);

	virtual ~CLogBondStiffnessChanged();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogBondStiffnessChanged& rMsg);

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

	CLogBondStiffnessChanged(const CLogBondStiffnessChanged& oldMessage);
	CLogBondStiffnessChanged& operator=(const CLogBondStiffnessChanged& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;				// Bond name, e.g., HBB
	const long    m_Type;				// Bond type, e.g., 0
	const double  m_BendingModulus;
	const double  m_Phi0;

};

#endif // !defined(AFX_LOGBONDSTIFFNESSCHANGED_H__49142103_2A0F_4FB5_8A36_023793585DEC__INCLUDED_)
