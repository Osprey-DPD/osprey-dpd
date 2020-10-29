// HistoryState.h: interface for the CHistoryState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISTORYSTATE_H__D70C3D00_3D5E_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_HISTORYSTATE_H__D70C3D00_3D5E_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CInputData;
class CTimeSeriesData;


#include "xxState.h"

class CHistoryState : public xxState  
{
	friend class CMonitor;

	// ****************************************
	// Construction/Destruction
public:

	CHistoryState(const CInputData& rData);

	virtual ~CHistoryState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool Serialize();

	// ****************************************
	// Public access functions
public:

	void AddTimeSeriesData(CTimeSeriesData* pTSD);
	void AddProbeBeadData(const double x, const double y, const double z);

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
private:								// Data arrays

	zDoubleVector m_ProbeBeadXPos;
	zDoubleVector m_ProbeBeadYPos;
	zDoubleVector m_ProbeBeadZPos;

	TimeSeriesSequence m_vTimeSeries;

										// Local data
	const long m_SamplePeriod;
	long m_LastPeriodSerialized;

};

#endif // !defined(AFX_HISTORYSTATE_H__D70C3D00_3D5E_11D3_820E_0060088AD300__INCLUDED_)
