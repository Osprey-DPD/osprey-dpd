// LogCurrentStateLight.h: interface for the CLogCurrentStateLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCURRENTSTATELIGHT_H__BBDA0D95_9382_4B29_BDD0_2D0033D93765__INCLUDED_)
#define AFX_LOGCURRENTSTATELIGHT_H__BBDA0D95_9382_4B29_BDD0_2D0033D93765__INCLUDED_


#include "LogInfoMessage.h"

class CLogCurrentStateLight : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCurrentStateLight(long time, const double dx, const double dy, const double dz);

	virtual ~CLogCurrentStateLight();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCurrentStateLight& rMsg);

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

	CLogCurrentStateLight(const CLogCurrentStateLight& oldMessage);
	CLogCurrentStateLight& operator=(const CLogCurrentStateLight& rhs);

	// ****************************************
	// Data members
private:

	const double m_LightX;
	const double m_LightY;
	const double m_LightZ;

};

#endif // !defined(AFX_LOGCURRENTSTATELIGHT_H__BBDA0D95_9382_4B29_BDD0_2D0033D93765__INCLUDED_)

