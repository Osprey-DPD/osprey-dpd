// CompositeBilayerData.h: interface for the CCompositeBilayerData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOSITEBILAYERDATA_H__8534D9F9_968F_4EF1_8323_C46D7B796800__INCLUDED_)
#define AFX_COMPOSITEBILAYERDATA_H__8534D9F9_968F_4EF1_8323_C46D7B796800__INCLUDED_


#include "AnalysisData.h"

class CCompositeBilayerData : public CAnalysisData  
{
	friend class CCompositeBilayer;		// Needed to copy data into CBilayer

	// ****************************************
	// Construction/Destruction
public:

	CCompositeBilayerData();

	virtual ~CCompositeBilayerData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static zString m_Type;	// Keyword used to identify aggregate analysis

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

	zString		   m_Polymer;		// Major polymer in bilayer
	StringSequence m_CoPolymers;	// Other polymers in bilayer

	long m_X;						// Normal to bilayer
	long m_Y;
	long m_Z;

	long m_SliceTotal;				// Number of slices in the SimBox
	long m_RowTotal;				// Number of rows to break a slice into
	long m_CellTotal;				// Number of cells to break a row into

	zString m_Solvent;				// Bead type representing the solvent (usually W)

	bool	m_bFRAP;				// Flag showing if FRAP tool is active
	zString m_FRAPPolymer;			// Polymer to be monitored using FRAP

	bool	m_bRafts;				// Flag showing if raft monitoring tool is active
	StringSequence	m_RaftPolymers;	// Names of raft polymers
	bool	m_bMonolayers[2];		// Flags showing which monolayers to monitor: Upper/lower

};

#endif // !defined(AFX_COMPOSITEBILAYERDATA_H__8534D9F9_968F_4EF1_8323_C46D7B796800__INCLUDED_)




	
