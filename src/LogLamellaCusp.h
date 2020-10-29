// LogLamellaCusp.h: interface for the CLogLamellaCusp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLAMELLACUSP_H__4742A03B_527B_4417_92B7_02DF7AE7522E__INCLUDED_)
#define AFX_LOGLAMELLACUSP_H__4742A03B_527B_4417_92B7_02DF7AE7522E__INCLUDED_


#include "LogEventMessage.h"

class CLogLamellaCusp : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogLamellaCusp(long time, const zString name, long type, double curvature);

	virtual ~CLogLamellaCusp();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogLamellaCusp& rMsg);

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

	CLogLamellaCusp(const CLogLamellaCusp& oldMessage);
	CLogLamellaCusp& operator=(const CLogLamellaCusp& rhs);

	// ****************************************
	// Data members
private:

	const zString	m_Name;				// Aggregate name, e.g., bilayer
	const long		m_Type;				// Aggregate id, e.g., 0, 1, 2
	const double	m_Curvature;		// Cusp curvature 
};

#endif // !defined(AFX_LOGLAMELLACUSP_H__4742A03B_527B_4417_92B7_02DF7AE7522E__INCLUDED_)
