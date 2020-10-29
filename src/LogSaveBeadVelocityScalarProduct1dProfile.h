// LogSaveBeadVelocityScalarProduct1dProfile.h: interface for the CLogSaveBeadVelocityScalarProduct1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__775CF7D7_A332_4957_A54A_70523E46A10D__INCLUDED_)
#define AFX_LOGSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__775CF7D7_A332_4957_A54A_70523E46A10D__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveBeadVelocityScalarProduct1dProfile : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveBeadVelocityScalarProduct1dProfile(long time, long analysisPeriods, 
                                    long dataPoints, long start, long end, 
                                    long samplePeriod, long samplesPerPoint);

	virtual ~CLogSaveBeadVelocityScalarProduct1dProfile();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveBeadVelocityScalarProduct1dProfile& rMsg);

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

	CLogSaveBeadVelocityScalarProduct1dProfile(const CLogSaveBeadVelocityScalarProduct1dProfile& oldMessage);
	CLogSaveBeadVelocityScalarProduct1dProfile& operator=(const CLogSaveBeadVelocityScalarProduct1dProfile& rhs);


	// ****************************************
	// Data members
private:

    const long  m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long  m_TotalDataPoints;      // No of data points to output

	const long	m_Start;		        // Time at which analysis starts
	const long	m_End;			        // Time at which analysis ends
	const long	m_SamplePeriod;	        // Number of time steps between data samples
    const long  m_SamplesPerDataPoint;  // No of samples per output data point

};

#endif // !defined(AFX_LOGSAVEBEADVELOCITYSCALARPRODUCT1DPROFILE_H__775CF7D7_A332_4957_A54A_70523E46A10D__INCLUDED_)
