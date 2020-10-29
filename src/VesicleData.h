// VesicleData.h: interface for the CVesicleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VESICLEDATA_H__EC62FB36_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_VESICLEDATA_H__EC62FB36_3C4F_11D4_BF3C_004095086186__INCLUDED_



// Forward declarations

class CAnalysis;



#include "AnalysisData.h"

class CVesicleData : public CAnalysisData
{
	friend class CVesicle;		// Needed to copy data into CVesicle

	// ****************************************
	// Construction/Destruction
public:

	CVesicleData();

	virtual ~CVesicleData();

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

	zString m_Polymer;	// Polymer composing vesicle
	zString m_Solvent;	// Bead type representing the solvent (usually W)
	
	bool  m_bStress;     // Flag showing if the stress profile should be calculated
	long  m_ShellTotal;  // Number of shells in the stress profile calculation
	long  m_ShellMin;    // Lower limit in the integration of the stress profile
	long  m_ShellMax;    // Upper limit in the integration of the stress profile

};

#endif // !defined(AFX_VESICLEDATA_H__EC62FB36_3C4F_11D4_BF3C_004095086186__INCLUDED_)
