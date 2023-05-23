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

	virtual ~CTimeSeriesData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CTimeSeriesData& rTSD);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following virtual function must be provided by all derived classes
	// so that they can write their own data when directed by the operator<<(), which
    // cannot be treat polymorphically.

    virtual zOutStream& put(zOutStream& os) const;

	// ****************************************
	// Public access functions
public:

	void SetValue(long id, double value, zString label="data");

	inline long   Size()            const {return m_vDataSet.size();}
	inline long   GetTime()         const {return static_cast<long>(m_vDataSet.at(0));}	// Time must be 0th element
	inline double GetValue(long id) const {return m_vDataSet.at(id);}

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
protected:

    zDoubleVector    m_vDataSet;           // Set of stored values
    StringSequence   m_vDataLabels;        // Labels to identify stored values


};

#endif // !defined(AFX_TIMESERIESDATA_H__F63160F3_7D50_11D3_BF06_004095086186__INCLUDED_)
