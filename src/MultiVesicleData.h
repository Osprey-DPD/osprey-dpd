// MultiVesicleData.h: interface for the CMultiVesicleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIVESICLEDATA_H__A9C239F9_9C13_4ECA_9109_80B19436BF36__INCLUDED_)
#define AFX_MULTIVESICLEDATA_H__A9C239F9_9C13_4ECA_9109_80B19436BF36__INCLUDED_


// Forward declarations

class CAnalysis;



#include "AnalysisData.h"

class CMultiVesicleData : public CAnalysisData  
{
	friend class CMultiVesicle;		// Needed to copy data into CVesicle

	// ****************************************
	// Construction/Destruction
public:

	CMultiVesicleData();

	virtual ~CMultiVesicleData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// the aggregate type to be accessed at class level

private:

	static zString m_Type;			// Keyword used to identify structure for analysis


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual const zString GetAnalysisType() const;

	virtual CAnalysis* CreateAnalysis(const zString runId, long samplePeriod);	// Function to copy the data into a CAnalysis-derived object

	// The ValidateData() function is not yet a PVF because it has not been
	// implemented by all derived classes. Eventually it should become
	// one.

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

	zString m_Polymer;		// Polymer composing vesicle
	zString m_Solvent;		// Bead type representing the solvent (usually W)
};

#endif // !defined(AFX_MULTIVESICLEDATA_H__A9C239F9_9C13_4ECA_9109_80B19436BF36__INCLUDED_)
