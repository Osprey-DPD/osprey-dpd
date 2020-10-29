// LogSaveBead1dDensityProfile.h: interface for the CLogSaveBead1dDensityProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEBEAD1DDENSITYPROFILE_H__767EA3E8_BB77_457C_9FC4_9FF0862A4075__INCLUDED_)
#define AFX_LOGSAVEBEAD1DDENSITYPROFILE_H__767EA3E8_BB77_457C_9FC4_9FF0862A4075__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveBead1dDensityProfile : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveBead1dDensityProfile(long time, const zString name, long type,
                                 long start, long end, long sample,
                                 long nx, long ny, long nz, long slices);

	virtual ~CLogSaveBead1dDensityProfile();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveBead1dDensityProfile& rMsg);

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

	CLogSaveBead1dDensityProfile(const CLogSaveBead1dDensityProfile& oldMessage);
	CLogSaveBead1dDensityProfile& operator=(const CLogSaveBead1dDensityProfile& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_BeadName;		// Bead name identifier
	const long		m_BeadType;		// Bead numeric type identifier
	const long	    m_Start;		// Time at which density field analysis starts
	const long	    m_End;			// Time at which density field analysis ends
	const long	    m_SamplePeriod;	// Number of time steps between data samples

	const long	    m_NX;
	const long	    m_NY;			// Direction of density profile
	const long	    m_NZ;
	const long	    m_SliceTotal;	// Number of slices in the density profile
};

#endif // !defined(AFX_LOGSAVEBEAD1DDENSITYPROFILE_H__767EA3E8_BB77_457C_9FC4_9FF0862A4075__INCLUDED_)
