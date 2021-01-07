// LogVesiclePore.h: interface for the CLogVesiclePore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEPORE_H__C75841D6_D9D7_409A_AED6_FF434BBC30D4__INCLUDED_)
#define AFX_LOGVESICLEPORE_H__C75841D6_D9D7_409A_AED6_FF434BBC30D4__INCLUDED_


#include "LogEventMessage.h"

class CLogVesiclePore : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesiclePore(long time, const zString name, long type, long poreSize);

	virtual ~CLogVesiclePore();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesiclePore& rMsg);

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

	CLogVesiclePore(const CLogVesiclePore& oldMessage);
	CLogVesiclePore& operator=(const CLogVesiclePore& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Name;				// Aggregate name, e.g., bilayer
	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2
	const long	  m_PoreSize;			// Size of pore in grid points

};

#endif // !defined(AFX_LOGVESICLEPORE_H__C75841D6_D9D7_409A_AED6_FF434BBC30D4__INCLUDED_)
