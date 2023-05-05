// TimeSeriesData.h: interface for the CTimeSeriesData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESERIESDATA_H__F63160F3_7D50_11D3_BF06_004095086186__INCLUDED_)
#define AFX_TIMESERIESDATA_H__F63160F3_7D50_11D3_BF06_004095086186__INCLUDED_


// Include file to gain access to the typedefs 

#include "xxBase.h"


class CTimeSeriesData  
{
	// ****************************************
	// Construction/Destruction
public:

	CTimeSeriesData(long dataSetSize);

	~CTimeSeriesData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CTimeSeriesData& rTSD);

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

    // Overloaded functions to allow either just the data or data/sdev to be stored
    
    void SetValue(long id, double value, zString label="data");
    void SetValue(long id, double value, double sdev, zString label="data");

	inline long   Size()            const {return m_vDataSet.size();}
	inline long   GetTime()         const {return static_cast<long>(m_vDataSet.at(0));}	// Time must be 0th element
    inline double GetValue(long id) const {return m_vDataSet.at(id);}
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

    zDoubleVector    m_vDataSet;            // Set of stored values
    zDoubleVector    m_vSDevSet;            // Set of standard deviations of the stored values
	StringSequence	 m_vDataLabels;		    // Labels to identify stored values


};

#endif // !defined(AFX_TIMESERIESDATA_H__F63160F3_7D50_11D3_BF06_004095086186__INCLUDED_)
