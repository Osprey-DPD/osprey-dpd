// LogVesicleDistalTouch.h: interface for the CLogVesicleDistalTouch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEDISTALTOUCH_H__94F0D31B_E9FD_4B4D_B7CE_88E885CA3040__INCLUDED_)
#define AFX_LOGVESICLEDISTALTOUCH_H__94F0D31B_E9FD_4B4D_B7CE_88E885CA3040__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleDistalTouch : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleDistalTouch(long time, const zString name1, long type1, 
								      const zString name2, long type2, double distance);
	virtual ~CLogVesicleDistalTouch();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleDistalTouch& rMsg);

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

	CLogVesicleDistalTouch(const CLogVesicleDistalTouch& oldMessage);
	CLogVesicleDistalTouch& operator=(const CLogVesicleDistalTouch& rhs);


	// ****************************************
	// Data members
private:

	const zString   m_Name1;				// Aggregate name, e.g., bilayer
	const long      m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString   m_Name2;				// Aggregate name, e.g., bilayer
	const long      m_Type2;				// Aggregate id, e.g., 0, 1, 2
	const double	m_Distance;				// Separation of vesicle centres of mass


};

#endif // !defined(AFX_LOGVESICLEDISTALTOUCH_H__94F0D31B_E9FD_4B4D_B7CE_88E885CA3040__INCLUDED_)
