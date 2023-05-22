// TimeSeriesMeanData.h: interface for the CTimeSeriesMeanData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESERIESMEANDATA_H__140ab0af_b2c5_4126_aafb_7ff9981cee87__INCLUDED_)
#define AFX_TIMESERIESMEANDATA_H__140ab0af_b2c5_4126_aafb_7ff9981cee87__INCLUDED_


#include "TimeSeriesData.h"


class CTimeSeriesMeanData : public CTimeSeriesData
{
	// ****************************************
	// Construction/Destruction
public:

	CTimeSeriesMeanData(long dataSetSize);

	virtual ~CTimeSeriesMeanData();

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

    virtual zOutStream& put(zOutStream& os) const;

	// ****************************************
	// Public access functions
public:

    // New functions to allow data/sdev to be stored in this class
    
    void SetValue(long id, double value, double sdev, zString label="data");

    inline double GetSDev(long id)  const {return m_vSDevSet.at(id);}

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

    zDoubleVector m_vSDevSet;         // Set of standard deviations of the stored values


};

#endif // !defined(AFX_TimeSeriesMeanDataDATA_H__140ab0af_b2c5_4126_aafb_7ff9981cee87__INCLUDED_)
