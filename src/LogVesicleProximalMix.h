// LogVesicleProximalMix.h: interface for the CLogVesicleProximalMix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEPROXIMALMIX_H__F46E4CE9_901E_4A6B_A33D_C9B5DCFC1FE6__INCLUDED_)
#define AFX_LOGVESICLEPROXIMALMIX_H__F46E4CE9_901E_4A6B_A33D_C9B5DCFC1FE6__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleProximalMix : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleProximalMix(long time, const zString name1, long type1, 
									  const zString name2, long type2);
	virtual ~CLogVesicleProximalMix();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleProximalMix& rMsg);

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

	CLogVesicleProximalMix(const CLogVesicleProximalMix& oldMessage);
	CLogVesicleProximalMix& operator=(const CLogVesicleProximalMix& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name1;				// Aggregate name, e.g., bilayer
	const long    m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString m_Name2;				// Aggregate name, e.g., bilayer
	const long    m_Type2;				// Aggregate id, e.g., 0, 1, 2

};

#endif // !defined(AFX_LOGVESICLEPROXIMALMIX_H__F46E4CE9_901E_4A6B_A33D_C9B5DCFC1FE6__INCLUDED_)
