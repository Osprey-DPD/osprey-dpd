// LogCurrentStateCamera.h: interface for the CLogCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCURRENTSTATECAMERA_H__9F6159E2_BE71_40F7_8C79_A92F7551F446__INCLUDED_)
#define AFX_LOGCURRENTSTATECAMERA_H__9F6159E2_BE71_40F7_8C79_A92F7551F446__INCLUDED_


#include "LogInfoMessage.h"

class CLogCurrentStateCamera : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCurrentStateCamera(long time, const double cx, const double cy, const double dz,
									  const double tx, const double ty, const double tz);

	virtual ~CLogCurrentStateCamera();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCurrentStateCamera& rMsg);

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

	CLogCurrentStateCamera(const CLogCurrentStateCamera& oldMessage);
	CLogCurrentStateCamera& operator=(const CLogCurrentStateCamera& rhs);

	// ****************************************
	// Data members
private:
	const double m_CameraX;
	const double m_CameraY;
	const double m_CameraZ;

	const double m_TargetX;
	const double m_TargetY;
	const double m_TargetZ;

};

#endif // !defined(AFX_LOGCURRENTSTATECAMERA_H__9F6159E2_BE71_40F7_8C79_A92F7551F446__INCLUDED_)

