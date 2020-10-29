// LogSaveBeadKineticTemperatures.h: interface for the CLogSaveBeadKineticTemperatures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEBEADKINETICTEMPERATURES_H__3D0D8CB6_7AC9_45C7_A982_DBB3FC758942__INCLUDED_)
#define AFX_LOGSAVEBEADKINETICTEMPERATURES_H__3D0D8CB6_7AC9_45C7_A982_DBB3FC758942__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveBeadKineticTemperatures : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveBeadKineticTemperatures(long time, long analysisPeriods, 
                                    long dataPoints, long start, long end, 
                                    long samplePeriod, long samplesPerPoint,
                                    long beadTypes);

	virtual ~CLogSaveBeadKineticTemperatures();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveBeadKineticTemperatures& rMsg);

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

	CLogSaveBeadKineticTemperatures(const CLogSaveBeadKineticTemperatures& oldMessage);
	CLogSaveBeadKineticTemperatures& operator=(const CLogSaveBeadKineticTemperatures& rhs);


	// ****************************************
	// Data members
private:

    const long  m_TotalAnalysisPeriods; // No of analysis periods to sample over
    const long  m_TotalDataPoints;      // No of data points to output

	const long	m_Start;		        // Time at which analysis starts
	const long	m_End;			        // Time at which analysis ends
	const long	m_SamplePeriod;	        // Number of time steps between data samples
    const long  m_SamplesPerDataPoint;  // No of samples per output data point

    const long  m_BeadTypeTotal;        // Number of bead types
};

#endif // !defined(AFX_LOGSAVEBEADKINETICTEMPERATURES_H__3D0D8CB6_7AC9_45C7_A982_DBB3FC758942__INCLUDED_)
