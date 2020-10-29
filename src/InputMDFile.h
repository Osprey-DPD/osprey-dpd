// InputMDFile.h: interface for the CInputMDFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTMDFILE_H__B5F919C4_5A24_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INPUTMDFILE_H__B5F919C4_5A24_11D3_820E_0060088AD300__INCLUDED_


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

class CInputMDFile : public xxFile  
{
public:
	CInputMDFile(zString inFileName, bool IOFlag, const CInitialStateData& rISD);
	virtual ~CInputMDFile();

	bool Serialize();

	void AddNewBeadType(zString name, zDoubleVector ljDepth, zDoubleVector ljRange,
									  zDoubleVector scDepth, zDoubleVector scRange);
	void AddNewBondType(zString name1, zString name2, double SprConst, double UnStrLen);
	void AddNewBondPairType(zString name1, zString name2, zString name3, double strength, double phi0);
	void AddNewPolymerType(zString name, zString shape, double fraction);
	void AddNewPolymerType(zString name, zString shape, double fraction, zString head, zString tail);

// Member variables accessed by the CInputData object to pass the data in 
// control data file into the main code.

public:									
	const CInitialStateData* pISD;		// Initial configuration data
	CGravityData* pGD;					// Data needed for a body force in the SimBox
	CWallData* pWD;						// Data needed for walls around the SimBox

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

	LongBeadTypeMap    mNewBeadTypes;
	LongBondTypeMap    mNewBondTypes;
	LongBondPairTypeMap	mNewBondPairTypes;
	LongPolymerTypeMap mNewPolymerTypes;

	double CNTZCellWidth;
	double CNTYCellWidth;
	double CNTXCellWidth;
	double AverageBeadDensity;
	double kT;
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
	long TotalMCTime;	// MC parameters needed for pre-MD relaxation 
	double MCStepSize;
	double RCutOff;		// MD cut-off radius for LJ and SC potentials
	

// Local data to wrap certain classes. These are not accessible by the
// CInputData object.

private:
	const CInitialStateData& m_rISD;	// Default initial state data object
	CInitialStateObject* m_pISO;		// Wrapper class for pISD

};

#endif // !defined(AFX_INPUTMDFILE_H__B5F919C4_5A24_11D3_820E_0060088AD300__INCLUDED_)
