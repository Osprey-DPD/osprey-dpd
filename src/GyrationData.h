// GyrationData.h: interface for the CGyrationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GYRATIONDATA_H__af893864_67e6_4e03_99e9_f81af2b00f9c__INCLUDED_)
#define AFX_GYRATIONDATA_H__af893864_67e6_4e03_99e9_f81af2b00f9c__INCLUDED_


// Forward declarations

class CAnalysis;


#include "AnalysisData.h"

class CGyrationData : public CAnalysisData
{
	friend class CGyration;		// Needed to pass data to CGyration

	// ****************************************
	// Construction/Destruction
public:

	CGyrationData();

	virtual ~CGyrationData();

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
    
    CCommandTargetNode*   m_pTarget;  // Command target containing beads/polymers whose Rg is being calculated

    StringSequence    m_Polymers;
    bool m_bByType;                 // Flag showing whether to output Rg**2 by polymer type as well as summed

};

#endif // !defined(AFX_GYRATIONDATA_H__af893864_67e6_4e03_99e9_f81af2b00f9c__INCLUDED_)
