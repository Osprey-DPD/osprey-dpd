// Vesicle.h: interface for the CVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VESICLE_H__EC62FB38_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_VESICLE_H__EC62FB38_3C4F_11D4_BF3C_004095086186__INCLUDED_



// Forward declarations

class CVesicleData;
class CDiagonaliseMatrix;
class aaScalar;
class aaScalarProfile;
class aaVectorProfile;
class aaStressTensor1d;

#include "Analysis.h"

class CVesicle : public CAnalysis  
{
    typedef  xxBasevector<aaScalar*>::iterator xxBasevectorIterator;

	// ****************************************
	// Construction/Destruction
public:

	CVesicle(const zString runId, long sample, const CVesicleData* const pAD);
	virtual ~CVesicle();


	// ****************************************
	// PVFs inherited from CAnalysis base class
public:

	virtual void ConvertNames(const CAnalysisState& raState);
	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);


	// ****************************************
	// IEventXXX interface functions for all supported events
public:

	virtual bool VesicleBindsWallSupported() const;
	virtual bool VesicleUnbindsWallSupported() const;

	// ****************************************
	// IVesicleXXX interface functions for all supported processes
public:

	double GetInitialCMXPos() const;
	double GetInitialCMYPos() const;
	double GetInitialCMZPos() const;

	double GetCMXPos() const;
	double GetCMYPos() const;
	double GetCMZPos() const;

	double GetRadius() const;

	long GetMajorLipidType()	 const;
	long GetMajorLipidTailType() const;
	long GetMajorLipidHeadType() const;

	// ****************************************
	// Public access functions
public:

	inline double GetRg2()                            const {return m_Rg2;}
	inline double GetSurfaceArea()                    const {return m_SurfaceArea;}
	inline double GetSurfaceTensionTrapezoidal()      const {return m_SurfaceTensionTrap;}
	inline double GetSurfaceTensionSimpson()          const {return m_SurfaceTensionSimpson;}
	inline zDoubleVector GetRadialStressProfile()     const {return m_vRadialStress;}
	inline zDoubleVector GetTransverseStressProfile() const {return m_vTransverseStress;}
	inline zDoubleVector GetStressProfile()           const {return m_vStressProfile;}
	
	
	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

    void StressProfile(const ISimBox* const pISimBox);        // Stress profile in spherical polar coordinates
	void PlanarStressProfile(const ISimBox* const pISimBox);  // Stress tensor in Cartesian coordinates averaged over Z slices
	
	// Alternative ways of approximating the surface tension integral
	
	double TrapezoidalRule(const double h);
	double SimpsonsRule(const double h);
	
	// ****************************************
	// Implementation
private:

	// Data specified by user for vesicle analysis

	zString m_Polymer;		    // Polymer composing vesicle
	zString m_Solvent;		    // Polymer name representing surrounding solvent
	bool    m_bStress;          // Flag showing if the vesicle's stress profile should be calculated (this requires the compilation 
	                            // flag "EnableStressTensorSphere"  to be enabled in SimMiscellaneousFlags.h"
	long    m_ShellTotal;       // Number of shells in the stress profile calculation
	long    m_ShellMin;         // Lower limit in the integration of the stress profile
	long    m_ShellMax;         // Upper limit in the integration of the stress profile

	long    m_LipidType;	    // Id of vesicle's major component polymer
								// (referred to as lipids in this code)
	long    m_LipidHeadType;	// Id of lipid head bead
	long    m_LipidTailType;	// Id of lipid tail bead
	long    m_SolventType;		// Id of solvent polymer
	long    m_SolventHeadType;	// Id of solvent polymer head beads

	long    m_MaxVesicleSize;	// Maximum number of polymers possible in vesicle
	long    m_Size;				// Current number of polymers in vesicle
	long    m_InitialTime;		// SimTime of first call to this analysis object

	double  m_dSize;			// Double copy of vesicle size
	double  m_Radius;			// Average radius defined by polymer head beads
	double  m_Rg2;				// Average radius of gyration of vesicle
	double  m_SurfaceArea;		// Average area defined by polymer head beads
	double  m_SurfaceTensionTrap;           // Average surface tension across vesicle integrated using Trapezoidal rule
	double  m_SurfaceTensionSimpson;        // Average surface tension across vesicle integrated using Simpson's rule
	double  m_DiffCoeff;			        // Vesicle CM diffusion coefficient
	double  m_InitialCMXPos;		        // Initial centre-of-mass coordinates of vesicle head beads
	double  m_InitialCMYPos;
	double  m_InitialCMZPos;
	double  m_CMXPos;			            // Centre-of-mass coordinates of vesicle head beads
	double  m_CMYPos;
	double  m_CMZPos;
	double  m_CMXVel;			            // Centre-of-mass velocity of vesicle head beads
	double  m_CMYVel;
	double  m_CMZVel;
	double  m_Eccentricity;		            // Largest/smallest eigenvalue of inertia tensor
	double  m_StressCellRatio;              // Cube of the stress cell multiplier = number of stress cells per CNT cell
	double  m_StressCellVolume;             // Volume of each stress cell in units of the CNT side length
	double  m_StressOrigin[3];              // CM of vesicle for use as origin of stress tensor transformation

	CDiagonaliseMatrix*	m_pJacobi;		    // Matrix diagonalisation class

	zDoubleVector m_InertiaTensor;		    // Vesicle inertia tensor
	zDoubleVector m_InertiaEigenvalues;		// Eigenvalues of the inertia tensor

    zDoubleVector m_vStressCounter;         // Number of stress values per shell
	zDoubleVector m_vRadialStress;          // Radial component of stress in each spherical shell
	zDoubleVector m_vTransverseStress;      // Average transverse component of stress in each spherical shell
	zDoubleVector m_vStressProfile;         // Lateral stress profile across vesicle
	zDoubleVector m_vTensionProfile;        // Surface tension profile across vesicle
	
	aaScalarProfile*  m_pSrr;				// Stress profile for rr component
	aaScalarProfile*  m_pStt;			    // Stress profile for 0.5*(Stt + Spp) component
	aaScalarProfile*  m_pStressProfile;		// Srr - 0.5*(Stt + spp) 

	PolymerList m_lLipids;					// List of all lipids possible in vesicle
	PolymerList m_lCurrentLipids;			// List of lipids currently in vesicle

	LongPolymerMap m_mPolymerFromHeadId;	// Map taking head bead id to polymer
	LongPolymerMap m_mPolymerFromTailId;	// Map taking tail bead id to polymer

};

#endif // !defined(AFX_VESICLE_H__EC62FB38_3C4F_11D4_BF3C_004095086186__INCLUDED_)
