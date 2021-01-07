// LogVesicleDistalMix.h: interface for the CLogVesicleDistalMix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEDISTALMIX_H__A3C69077_17AB_49F5_9B3D_DC66AFCB0A95__INCLUDED_)
#define AFX_LOGVESICLEDISTALMIX_H__A3C69077_17AB_49F5_9B3D_DC66AFCB0A95__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleDistalMix : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleDistalMix(long time, const zString name1, long type1, 
								    const zString name2, long type2);
	virtual ~CLogVesicleDistalMix();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleDistalMix& rMsg);

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

	CLogVesicleDistalMix(const CLogVesicleDistalMix& oldMessage);
	CLogVesicleDistalMix& operator=(const CLogVesicleDistalMix& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Name1;				// Aggregate name, e.g., bilayer
	const long    m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString m_Name2;				// Aggregate name, e.g., bilayer
	const long    m_Type2;				// Aggregate id, e.g., 0, 1, 2

};

#endif // !defined(AFX_LOGVESICLEDISTALMIX_H__A3C69077_17AB_49F5_9B3D_DC66AFCB0A95__INCLUDED_)
