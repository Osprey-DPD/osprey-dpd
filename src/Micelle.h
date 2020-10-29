// Micelle.h: interface for the CMicelle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICELLE_H__898EF6F3_8A24_11D4_BF49_004095086186__INCLUDED_)
#define AFX_MICELLE_H__898EF6F3_8A24_11D4_BF49_004095086186__INCLUDED_


// Forward declarations

class CMicelleData;
class CAutoCorr;



#include "Polymer.h"
#include "Analysis.h"

class CMicelle : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:
	CMicelle(const zString runId, long samplePeriod, const CMicelleData* const pAD);
	virtual ~CMicelle();


	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:


	inline const zString GetPolymer() const {return m_Polymer;}


	// ****************************************
	// PVS inherited from CAnalysis base class
public:

	// Function to get information about the micelle from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);


private:
	// Data specified by user for micelle analysis

	zString m_Polymer;		// Polymer composing micelle
	zString m_Solvent;		// Polymer name representing surrounding solvent
	long	m_VACTimeLag;	// Max time lag for CM velocity autocorrelation function

	// Local data used in the analysis

	CTimeSeriesData* m_pTSD;	// Store time-dependent data here

	long m_LipidType;			// Id of micelle's major component polymer
								// (referred to as lipids in this code)
	long m_LipidHeadType;		// Id of lipid head bead
	long m_LipidTailType;		// Id of lipid tail bead
	long m_SolventType;			// Id of solvent polymer
	long m_SolventHeadType;		// Id of solvent polymer head beads

	long   m_MaxMicelleSize;	// Maximum number of polymers possible in micelle
	long   m_Size;				// Current number of polymers in micelle
	long   m_PolymerSize;		// Number of beads per micelle polymer
	long   m_InitialTime;		// SimTime of first call to this analysis object
	double m_dSize;				// Number of lipids in micelle
	double m_dBeadSize;			// Number of beads in micelle
	double m_Radius;			// Average radius defined by polymer head beads
	double m_Rg2;				// Average radius of gyration of micelle
	double m_SurfaceArea;		// Average area defined by polymer head beads
	double m_DiffCoeff;			// Micelle CM diffusion coefficient
	double m_CMInitialPos[3];	// Initial CM position used to normalize diffusion coefficient
	double m_CMPos[3];			// CM position
	double m_CMVel[3];			// CM Velocity
	double m_CMVelMag;			// Magnitude of CM velocity
	double m_Inertia[9];		// Inertia tensor


	PolymerList m_lLipids;				// List of all lipids possible in micelle
	PolymerList m_lCurrentLipids;		// List of lipids currently in micelle

	LongPolymerMap m_mPolymerFromHeadId;		// Map taking head bead id to polymer
	LongPolymerMap m_mPolymerFromTailId;		// Map taking tail bead id to polymer

	CAutoCorr*		  m_pVAC;			// Pointer to the velocity autocorrelation object
	aaScalarProfile*  m_pVACProfile;	// Velocity profile


};

#endif // !defined(AFX_MICELLE_H__898EF6F3_8A24_11D4_BF49_004095086186__INCLUDED_)
