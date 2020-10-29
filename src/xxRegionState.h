// xxRegionState.h: interface for the xxRegionState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXREGIONSTATE_H__0A9FB271_329B_4835_973A_ADE9A2258579__INCLUDED_)
#define AFX_XXREGIONSTATE_H__0A9FB271_329B_4835_973A_ADE9A2258579__INCLUDED_

// Forward declarations

class CTimeSeriesData;


#include "xxState.h"

class xxRegionState : public xxState  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	xxRegionState(const zString runId, long id, long totalTime, long samplePeriod);

	virtual ~xxRegionState();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.


	// ****************************************
	// Public access functions
public:

	bool Serialize();

	void AddTimeSeriesData(CTimeSeriesData* pTSD);

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	long m_SamplesSerialized;

	TimeSeriesSequence m_vTimeSeries;

};

#endif // !defined(AFX_XXREGIONSTATE_H__0A9FB271_329B_4835_973A_ADE9A2258579__INCLUDED_)
