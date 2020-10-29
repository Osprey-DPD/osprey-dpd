// InputDPDLGFile.h: interface for the CInputDPDLGFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTDPDLGFILE_H__486E130B_32F8_48EC_808A_51B7A05806EC__INCLUDED_)
#define AFX_INPUTDPDLGFILE_H__486E130B_32F8_48EC_808A_51B7A05806EC__INCLUDED_


// Forward declarations

class CBeadType;
class CBondType;
class CBondPairType;
class CPolymerType;
class CInitialStateObject;
class CInitialStateData;
class CGravityData;
class CWallData;


#include "xxFile.h"

class CInputDPDLGFile : public xxFile  
{
	// ****************************************
	// Construction/Destruction
public:

    CInputDPDLGFile(zString inFileName, bool IOFlag, const CInitialStateData& rISD);
    CInputDPDLGFile(zString inFileName, bool IOFlag, bool IsLG, const CInitialStateData& rISD);

    virtual ~CInputDPDLGFile();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool Serialize();

	// ****************************************
	// Public access functions
public:

    bool IsLG() const {return m_bLG;}

	void AddNewBeadType(zString name, double radius, zDoubleVector consInt, zDoubleVector dissInt);
	void AddNewBeadType(zString name, double radius, double lgRadius, zDoubleVector consInt, zDoubleVector dissInt, zDoubleVector lgInt);
	void AddNewBondType(zString name1, zString name2, double SprConst, double UnStrLen);
	void AddNewBondPairType(zString name1, zString name2, zString name3, double strength, double phi0);
	void AddNewPolymerType(zString name, zString shape, double fraction);
	void AddNewPolymerType(zString name, zString shape, double fraction, zString head, zString tail);

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

    // Member variables accessed by the CInputData object to pass the data in the
    // control data file into the main code.

public:									
	const CInitialStateData* pISD;				// Initial configuration data
	CGravityData* pGD;							// Body force specification in the SimBox
	CWallData* pWD;								// Wall specification around the SimBox

	ProcessObjectSequence  vProcessObjects;		// Set of process objects
	AnalysisObjectSequence vAnalysisObjects;	// Set of analysis objects
	EventObjectSequence    vEventObjects;		// Set of event objects
	CommandObjectSequence  vCommandObjects;		// Set of command objects

// String used to identify the run

	zString sSimType;
	zString Title;
	zString Date;
	zString Comment;

// Types of bead, bond and polymer used to construct the CBead, etc, representations
// needed to copy-construct the many copies of beads, bonds and polymers used
// in the actual simulation.

	LongBeadTypeMap		mNewBeadTypes;
	LongBondTypeMap		mNewBondTypes;
	LongBondPairTypeMap	mNewBondPairTypes;
	LongPolymerTypeMap	mNewPolymerTypes;

	double CNTZCellWidth;
	double CNTYCellWidth;
	double CNTXCellWidth;
	double AverageBeadDensity;
	double kT;
	double Lambda;
	double StepSize;

    long ProcXNo;  // Number of processors to use in each dimension for parallel code
    long ProcYNo;
    long ProcZNo;
	long CNTZCellNo;
	long CNTYCellNo;
	long CNTXCellNo;
	long GridZCellNo;
	long GridYCellNo;
	long GridXCellNo;
	long AnalysisPeriod;
	long DensityPeriod;
	long DisplayPeriod;
	long RestartPeriod;
	long SamplePeriod;
	long RNGSeed;
	long TotalTime;

// Local data used to wrap certain classes. These are not accessible by
// the CInputData object.

private:
	const CInitialStateData& m_rISD;	// Default initial state data object
	CInitialStateObject* m_pISO;		// Wrapper class for pISD;

    bool  m_bLG;            // Flag showing if density-dependent LG force is included

};

#endif // !defined(AFX_INPUTDPDLGFILE_H__486E130B_32F8_48EC_808A_51B7A05806EC__INCLUDED_)
