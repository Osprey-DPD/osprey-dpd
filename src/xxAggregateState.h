// xxAggregateState.h: interface for the xxAggregateState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXAGGREGATESTATE_H__EC136040_FD16_4287_8540_89A03952A8B5__INCLUDED_)
#define AFX_XXAGGREGATESTATE_H__EC136040_FD16_4287_8540_89A03952A8B5__INCLUDED_


// Forward declarations

class CAnalysisData;
class CTimeSeriesData;


#include "xxState.h"

class xxAggregateState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

	xxAggregateState(const zString runId, long id, const CAnalysisData * pAD, long sample);

	virtual ~xxAggregateState();

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

	void AddTimeSeriesData(CTimeSeriesData *pTSD);

	// ****************************************
	// Protected local functions
protected:

	
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:
	
	const long m_SamplePeriod;
	long m_SamplesSerialized;

	TimeSeriesSequence m_vTimeSeries;
};


#endif // !defined(AFX_XXAGGREGATESTATE_H__EC136040_FD16_4287_8540_89A03952A8B5__INCLUDED_)
