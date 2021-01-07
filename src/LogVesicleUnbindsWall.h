// LogVesicleUnbindsWall.h: interface for the CLogVesicleUnbindsWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEUNBINDSWALL_H__8DB36B74_1693_4765_93AE_A91FEDBD30FD__INCLUDED_)
#define AFX_LOGVESICLEUNBINDSWALL_H__8DB36B74_1693_4765_93AE_A91FEDBD30FD__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleUnbindsWall : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleUnbindsWall(long time, long id, double d1, double d2);

	virtual ~CLogVesicleUnbindsWall();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleUnbindsWall& rMsg);

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

	CLogVesicleUnbindsWall(const CLogVesicleUnbindsWall& oldMessage);
	CLogVesicleUnbindsWall& operator=(const CLogVesicleUnbindsWall& rhs);

	// ****************************************
	// Data members
private:

	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2
	const double  m_CentreSeparation;	// Distance of vesicle centre from wall
	const double  m_EdgeSeparation;		// Distance of vesicle membrane from wall
};

#endif // !defined(AFX_LOGVESICLEUNBINDSWALL_H__8DB36B74_1693_4765_93AE_A91FEDBD30FD__INCLUDED_)
