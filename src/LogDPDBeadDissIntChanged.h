// LogDPDBeadDissIntChanged.h: interface for the CLogDPDBeadDissIntChanged class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGDPDBEADDISSINTCHANGED_H__639B46C2_ED68_4064_AE35_D1F9116C1A43__INCLUDED_)
#define AFX_LOGDPDBEADDISSINTCHANGED_H__639B46C2_ED68_4064_AE35_D1F9116C1A43__INCLUDED_


#include "LogConstraintMessage.h"

class CLogDPDBeadDissIntChanged : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogDPDBeadDissIntChanged(long time, const zString firstName, const zString secondName, 
							  long firstType, long secondType, double dissInt);

	virtual ~CLogDPDBeadDissIntChanged();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogDPDBeadDissIntChanged& rMsg);

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

	CLogDPDBeadDissIntChanged(const CLogDPDBeadDissIntChanged& oldMessage);
	CLogDPDBeadDissIntChanged& operator=(const CLogDPDBeadDissIntChanged& rhs);

	// ****************************************
	// Data members
private:

	const zString m_FirstName;				// Primary bead name, e.g., H
	const zString m_SecondName;				// Secondary bead name, e.g., H
	const long    m_FirstType;				// Primary bead type, e.g., 0
	const long    m_SecondType;				// Secondary bead type, e.g., 1
	const double  m_DissInt;				// New value of dissipative force parameter

};

#endif // !defined(AFX_LOGDPDBEADDISSINTCHANGED_H__639B46C2_ED68_4064_AE35_D1F9116C1A43__INCLUDED_)
