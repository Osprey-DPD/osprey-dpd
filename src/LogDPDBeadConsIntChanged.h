// LogDPDBeadConsIntChanged.h: interface for the CLogDPDBeadConsIntChanged class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGDPDBEADCONSINTCHANGED_H__B038186D_A5EB_4CC6_AA2E_F0C206AE64C3__INCLUDED_)
#define AFX_LOGDPDBEADCONSINTCHANGED_H__B038186D_A5EB_4CC6_AA2E_F0C206AE64C3__INCLUDED_


#include "LogConstraintMessage.h"

class CLogDPDBeadConsIntChanged : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogDPDBeadConsIntChanged(long time, 
							  const zString firstName, const zString secondName, 
							  long firstType, long secondType, double consInt);

	virtual ~CLogDPDBeadConsIntChanged();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogDPDBeadConsIntChanged& rMsg);

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

	CLogDPDBeadConsIntChanged(const CLogDPDBeadConsIntChanged& oldMessage);
	CLogDPDBeadConsIntChanged& operator=(const CLogDPDBeadConsIntChanged& rhs);

	// ****************************************
	// Data members
private:

	const zString m_FirstName;				// Primary bead name, e.g., H
	const zString m_SecondName;				// Secondary bead name, e.g., H
	const long    m_FirstType;				// Primary bead type, e.g., 0
	const long    m_SecondType;				// Secondary bead type, e.g., 1
	const double  m_ConsInt;				// New value of conservative force parameter

};

#endif // !defined(AFX_LOGDPDBEADCONSINTCHANGED_H__B038186D_A5EB_4CC6_AA2E_F0C206AE64C3__INCLUDED_)
