// InterfaceData.h: interface for the CInterfaceData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACEDATA_H__ae227539_82f9_4027_a98e_f5ffafa0695c__INCLUDED_)
#define AFX_INTERFACEDATA_H__ae227539_82f9_4027_a98e_f5ffafa0695c__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CInterfaceData : public CAnalysisData
{
	friend class CInterface;		// Needed to pass data to CInterface

	// ****************************************
	// Construction/Destruction
public:

	CInterfaceData();

	virtual ~CInterfaceData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static zString m_Type;		// Keyword used to identify aggregate analysis

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual const zString GetAnalysisType() const;

	// Function to copy the data into a CAnalysis-derived object

	virtual CAnalysis* CreateAnalysis(const zString runId, long samplePeriod);

	virtual bool ValidateData(const CInputData& riData) const;

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
private:

	// ****************************************
	// Data members
private:

	zString m_PolymerMajor;		// Polymer on one side of interface
	zString m_PolymerMinor;		// Polymer on other side of interface
	double m_LowDensity;		// Thresholds for determining interface boundaries
	double m_HighDensity;
	long m_X;					// Interface normal: must be rational hence we use integers
	long m_Y;							
	long m_Z;						

};

#endif // !defined(AFX_INTERFACEDATA_H__ae227539_82f9_4027_a98e_f5ffafa0695c__INCLUDED_)
