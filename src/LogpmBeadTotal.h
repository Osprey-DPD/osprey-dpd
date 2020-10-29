// LogpmBeadTotal.h: interface for the CLogpmBeadTotal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPMBEADTOTAL_H__87BE1FDE_FA02_4095_8A76_AA67458540DB__INCLUDED_)
#define AFX_LOGPMBEADTOTAL_H__87BE1FDE_FA02_4095_8A76_AA67458540DB__INCLUDED_


#include "LogInfoMessage.h"

class CLogpmBeadTotal : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpmBeadTotal(long time, long beadTotal);

	virtual ~CLogpmBeadTotal();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpmBeadTotal& rMsg);

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

	CLogpmBeadTotal(const CLogpmBeadTotal& oldMessage);
	CLogpmBeadTotal& operator=(const CLogpmBeadTotal& rhs);

	// ****************************************
	// Data members
private:

	const long m_BeadTotal;  // Number of beads in processor World
};

#endif // !defined(AFX_LOGPMBEADTOTAL_H__87BE1FDE_FA02_4095_8A76_AA67458540DB__INCLUDED_)
