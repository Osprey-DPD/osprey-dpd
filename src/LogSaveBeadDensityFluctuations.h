// LogSaveBeadDensityFluctuations.h: interface for the CLogSaveBeadDensityFluctuations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEBEADDENSITYFLUCTUATIONS_H__9C437C2F_D690_42DD_90E0_28E01F64F1A7__INCLUDED_)
#define AFX_LOGSAVEBEADDENSITYFLUCTUATIONS_H__9C437C2F_D690_42DD_90E0_28E01F64F1A7__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveBeadDensityFluctuations : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveBeadDensityFluctuations(long time, const zString beadName, long beadType,
                                    long totalDataPoints, long totalDensityPeriods, 
                                    long densityPeriod, 
                                    long start, long end, 
                                    long xCell, long yCell, long zCell,
                                    bool bConjugate);

	virtual ~CLogSaveBeadDensityFluctuations();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveBeadDensityFluctuations& rMsg);

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

	CLogSaveBeadDensityFluctuations(const CLogSaveBeadDensityFluctuations& oldMessage);
	CLogSaveBeadDensityFluctuations& operator=(const CLogSaveBeadDensityFluctuations& rhs);


	// ****************************************
	// Data members
private:

	const zString m_BeadName;	          // String identifier for bead type
	const long    m_BeadType;	          // Numeric identifier for bead type

    const long    m_TotalDataPoints;      // No of data points to output
    const long    m_TotalDensityPeriods;  // No of density periods to average over
    const long    m_DensityPeriod;        // Density period
	const long	  m_Start;		          // Time at which analysis starts
	const long	  m_End;			      // Time at which analysis ends
    const long    m_XCellNo;
    const long    m_YCellNo;              // Indices for cell being analysed
    const long    m_ZCellNo;
    const bool    m_bConjugate;           // Flag showing if results are output for
                                          // the cell and its complementary volume

};

#endif // !defined(AFX_LOGSAVEBEADDENSITYFLUCTUATIONS_H__9C437C2F_D690_42DD_90E0_28E01F64F1A7__INCLUDED_)
