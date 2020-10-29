// BilayerData.h: interface for the CBilayerData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BILAYERDATA_H__A4AEA91A_9D0D_11D3_BF15_004095086186__INCLUDED_)
#define AFX_BILAYERDATA_H__A4AEA91A_9D0D_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CAnalysis;



#include "AnalysisData.h"

class CBilayerData : public CAnalysisData
{
	friend class CBilayer;		// Needed to copy data into CBilayer

	// ****************************************
	// Construction/Destruction
public:

	CBilayerData();

	virtual ~CBilayerData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static zString m_Type;			// Keyword used to identify structure for analysis


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

	zString m_Polymer;				// Polymer composing bilayer

	long m_X;						// Normal to bilayer
	long m_Y;
	long m_Z;

	long m_SliceTotal;				// Number of slices in the SimBox
	long m_RowTotal;				// Number of rows to break a slice into
	long m_CellTotal;				// Number of cells to break a row into

	zString m_Solvent;				// Bead type representing the solvent (usually W)
};

#endif // !defined(AFX_BILAYERDATA_H__A4AEA91A_9D0D_11D3_BF15_004095086186__INCLUDED_)

