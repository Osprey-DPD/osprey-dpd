// IRegionAnalysis.h: interface for the IRegionAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IREGIONANALYSIS_H__C0CC8984_9EBF_4D13_8E5B_33F235B572AF__INCLUDED_)
#define AFX_IREGIONANALYSIS_H__C0CC8984_9EBF_4D13_8E5B_33F235B572AF__INCLUDED_


// Forward declarations

class CSimState;
class ISimBox;
class xxRegionState;

// Include files



#include "xxBase.h"

class IRegionAnalysis : public xxBase
{
	// ****************************************
	// Construction/Destruction
public:

	IRegionAnalysis(const ISimBox* pISimBox);
	virtual ~IRegionAnalysis();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static long GetRegionAnalysisTotal();

	static void ZeroRegionAnalysisTotal();

private:

	static long m_RegionAnalysisTotal;


	// ****************************************
	// Public access functions
public:

	inline long GetId() const {return m_id;}

	// Function to write out the analysis data to file

	bool Serialize();

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Function to analyse the selected region of the SimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox) = 0;



	// ****************************************
	// Protected local functions
protected:

	void AddTimeSeriesData(CTimeSeriesData* pTSD);



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const long		m_id;			// Unique id for aggregate

	xxRegionState* m_pState;		// Time series data for the region


};

#endif // !defined(AFX_IREGIONANALYSIS_H__C0CC8984_9EBF_4D13_8E5B_33F235B572AF__INCLUDED_)
