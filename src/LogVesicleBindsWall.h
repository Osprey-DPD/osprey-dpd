// LogVesicleBindsWall.h: interface for the CLogVesicleBindsWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEBINDSWALL_H__EA443197_43D8_423E_B45E_862469277407__INCLUDED_)
#define AFX_LOGVESICLEBINDSWALL_H__EA443197_43D8_423E_B45E_862469277407__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleBindsWall : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleBindsWall(long time, long id, double d1, double d2);

	virtual ~CLogVesicleBindsWall();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleBindsWall& rMsg);

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

	CLogVesicleBindsWall(const CLogVesicleBindsWall& oldMessage);
	CLogVesicleBindsWall& operator=(const CLogVesicleBindsWall& rhs);


	// ****************************************
	// Data members
private:

	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2
	const double  m_CentreSeparation;	// Distance of vesicle centre from wall
	const double  m_EdgeSeparation;		// Distance of vesicle membrane from wall

};

#endif // !defined(AFX_LOGVESICLEBINDSWALL_H__EA443197_43D8_423E_B45E_862469277407__INCLUDED_)
