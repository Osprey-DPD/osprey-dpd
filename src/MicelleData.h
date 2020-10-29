// MicelleData.h: interface for the CMicelleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICELLEDATA_H__898EF6F4_8A24_11D4_BF49_004095086186__INCLUDED_)
#define AFX_MICELLEDATA_H__898EF6F4_8A24_11D4_BF49_004095086186__INCLUDED_


#include "AnalysisData.h"

class CMicelleData : public CAnalysisData  
{
	friend class CMicelle;		// Needed to copy data into CMicelle

	// ****************************************
	// Construction/Destruction
public:

	CMicelleData();

	virtual ~CMicelleData();

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

	zString m_Polymer;				// Polymer composing micelle
	zString m_Solvent;				// Polymer representing the solvent
	long    m_VACTimeLag;			// Max time lag for velocity autocorrelation function

};

#endif // !defined(AFX_MICELLEDATA_H__898EF6F4_8A24_11D4_BF49_004095086186__INCLUDED_)
