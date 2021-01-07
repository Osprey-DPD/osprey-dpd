// LogVesicleApproach.h: interface for the CLogVesicleApproach class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLEAPPROACH_H__2EF31FCE_98B0_4474_B0B8_58825BFAF5A3__INCLUDED_)
#define AFX_LOGVESICLEAPPROACH_H__2EF31FCE_98B0_4474_B0B8_58825BFAF5A3__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleApproach : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleApproach(long time, const zString name1, long type1, 
								   const zString name2, long type2, double distance);
	virtual ~CLogVesicleApproach();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleApproach& rMsg);

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

	CLogVesicleApproach(const CLogVesicleApproach& oldMessage);
	CLogVesicleApproach& operator=(const CLogVesicleApproach& rhs);

	// ****************************************
	// Data members
private:

	const zString   m_Name1;				// Aggregate name, e.g., bilayer
	const long      m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString   m_Name2;				// Aggregate name, e.g., bilayer
	const long      m_Type2;				// Aggregate id, e.g., 0, 1, 2
	const double	m_Distance;			// Separation of vesicle centres of mass

};

#endif // !defined(AFX_LOGVESICLEAPPROACH_H__2EF31FCE_98B0_4474_B0B8_58825BFAF5A3__INCLUDED_)

