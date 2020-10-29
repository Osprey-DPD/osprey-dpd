// prBeadDensityFluctuations.h: interface for the prBeadDensityFluctuations class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBEADDENSITYFLUCTUATIONS_H__3BE39A6D_9A93_46EF_B3F4_1F506203612D__INCLUDED_)
#define AFX_PRBEADDENSITYFLUCTUATIONS_H__3BE39A6D_9A93_46EF_B3F4_1F506203612D__INCLUDED_


// Forward declarations

class CSimState;


#include "xxProcess.h"

class prBeadDensityFluctuations : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prBeadDensityFluctuations();

	// Constructor for use by the command that creates this process

	prBeadDensityFluctuations(const CSimState* const pSimState, long beadType, 
                              long dataPoints, long densityPeriods, 
                              long xCell, long yCell, long zCell, bool bConjugate);

	virtual ~prBeadDensityFluctuations();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// Function to allow process validation when it is created internally

	virtual bool InternalValidateData(const ISimState* const pISimState);


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:					        // Data specific to the process

    zString m_BeadName;             // String identifier for bead under examination
    long    m_BeadType;             // Numeric identifier for bead under examination
    long    m_TotalDataPoints;      // No of data points to output
    long    m_TotalDensityPeriods;  // No of density periods sampled for each data point
    long    m_XCellNo;
    long    m_YCellNo;              // Indices for cell being analysed
    long    m_ZCellNo;
    bool    m_bConjugate;           // Flag showing if results are output for
                                    // the cell and its complementary volume

    // Local data 

    long  m_SamplePeriod;           // No of time-steps between samples in SimBox
    long  m_DensityPeriod;          // Period of sampling bead density grid in SimBox
    long  m_SamplesTaken;           // Running total of samples in each average
    long  m_SamplesPerDataPoint;    // No of samples per output data point
    long  m_BeadTotal;              // Number of beads of the specified type in SimBox
    long  m_GridXCellNo;
    long  m_GridYCellNo;            // No of grid cells in each dimension
    long  m_GridZCellNo;

    double m_RunningMeanInCell;       // Running totals for bead averages
    double m_RunningSDevInCell;
    double m_RunningMeanOutsideCell;
    double m_RunningSDevOutsideCell;

};

#endif // !defined(AFX_PRBEADDENSITYFLUCTUATIONS_H__3BE39A6D_9A93_46EF_B3F4_1F506203612D__INCLUDED_)
