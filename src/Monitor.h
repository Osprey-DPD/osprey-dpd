// Monitor.h: interface for the CMonitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITOR_H__890FD6E0_3DE8_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_MONITOR_H__890FD6E0_3DE8_11D3_820E_0060088AD300__INCLUDED_

// Forward declarations

class CSimState;
class CCurrentStateFormat;


#include "ISimBoxBase.h"
#include "IMonitorCmd.h"

#if EnableMonitorCommand == SimCommandEnabled
    #include "mcCommentImpl.h"
    #include "mcGenerateRNGGaussianImpl.h"
    #include "mcGenerateRNGUniformImpl.h"
    #include "mcPanCurrentStateCameraImpl.h"
    #include "mcSaveCommandHistoryImpl.h"
    #include "mcSaveCommandHistoryToCurrentTimeImpl.h"
    #include "mcSaveProtocolFileImpl.h"
    #include "mcSetAllBeadsInvisibleImpl.h"
    #include "mcSetAllBeadsVisibleImpl.h"
    #include "mcShowAllProcessesImpl.h"
    #include "mcShowModifiableProcessesImpl.h"
    #include "mcToggleCurrentStateAnalysisImpl.h"
    #include "mcToggleEnergyOutputImpl.h"
    #include "mcToggleSliceEnergyOutputImpl.h"
    #include "mcZoomCurrentStateCameraImpl.h"
#endif

#include "mcGetBuildDateImpl.h"
#include "mcGetBuildIdImpl.h"
#include "mcIssueRunCompleteFractionImpl.h"
#include "mcSaveAmiraCurrentStateImpl.h"
#include "mcSaveBead1dDensityProfileImpl.h"
#include "mcSaveBeadDensityFluctuationsImpl.h"
#include "mcSaveBeadDensityFluctuationsByTypeImpl.h"
#include "mcSaveBeadKineticTemperaturesImpl.h"
#include "mcSaveBeadVelocityScalarProduct1dProfileImpl.h"
#include "mcSaveCurrentStateImpl.h"
#include "mcSaveNonBeadInclusiveRestartStateImpl.h"
#include "mcSavePairCorrelationFunctionImpl.h"
#include "mcSaveParaviewCurrentStateImpl.h"
#include "mcSavePolymerBeadRDFImpl.h"
#include "mcSavePovrayCurrentStateImpl.h"
#include "mcSaveRestartStateImpl.h"
#include "mcSaveSAXSImpl.h"
#include "mcSetAnalysisPeriodImpl.h"
#include "mcSetBeadDisplayIdImpl.h"
#include "mcSetBeadTypeDisplayIdImpl.h"
#include "mcSetCurrentStateCameraImpl.h"
#include "mcSetCurrentStateDefaultFormatImpl.h"
#include "mcSetCurrentStateLightImpl.h"
#include "mcSetDensityPeriodImpl.h"
#include "mcSetDisplayBeadRangeImpl.h"
#include "mcSetDisplayPeriodImpl.h"
#include "mcSetPolymerDisplayIdImpl.h"
#include "mcSetPolymerTypeDisplayIdImpl.h"
#include "mcSetRestartPeriodImpl.h"
#include "mcSetRestartStateDefaultBeadCoordinatesImpl.h"
#include "mcSetRestartStateDefaultInclusiveImpl.h"
#include "mcSetRunCompleteIntervalImpl.h"
#include "mcSetSamplePeriodImpl.h"
#include "mcSetSAXSProcessBeadElectronNoImpl.h"
#include "mcToggleBeadDisplayImpl.h"
#include "mcToggleCurrentStateBoxImpl.h"
#include "mcToggleDensityFieldOutputImpl.h"
#include "mcTogglePolymerDisplayImpl.h"
#include "mcToggleRestartWarningMessagesImpl.h"
#include "mcWriteLogMessageImpl.h"

class CMonitor : public ISimBoxBase, public virtual IMonitorCmd, 
#if EnableMonitorCommand == SimCommandEnabled
				public mcCommentImpl,
				public mcGenerateRNGGaussianImpl,
				public mcGenerateRNGUniformImpl,
				public mcPanCurrentStateCameraImpl,
				public mcSaveCommandHistoryImpl,
				public mcSaveCommandHistoryToCurrentTimeImpl,
				public mcSaveProtocolFileImpl,
				public mcSetAllBeadsInvisibleImpl,
				public mcSetAllBeadsVisibleImpl,
				public mcShowAllProcessesImpl,
				public mcShowModifiableProcessesImpl,
				public mcToggleCurrentStateAnalysisImpl,
				public mcToggleEnergyOutputImpl,
				public mcToggleSliceEnergyOutputImpl,
				public mcZoomCurrentStateCameraImpl,
#endif
				public mcGetBuildDateImpl,
				public mcGetBuildIdImpl,
				public mcIssueRunCompleteFractionImpl,
				public mcSaveAmiraCurrentStateImpl,
				public mcSaveBead1dDensityProfileImpl,
				public mcSaveBeadDensityFluctuationsImpl,
				public mcSaveBeadDensityFluctuationsByTypeImpl,
				public mcSaveBeadKineticTemperaturesImpl,
				public mcSaveBeadVelocityScalarProduct1dProfileImpl,
				public mcSaveCurrentStateImpl,
				public mcSaveNonBeadInclusiveRestartStateImpl,
                public mcSavePairCorrelationFunctionImpl,
				public mcSaveParaviewCurrentStateImpl,
				public mcSavePolymerBeadRDFImpl,
				public mcSavePovrayCurrentStateImpl,
				public mcSaveRestartStateImpl,
                public mcSaveSAXSImpl,
				public mcSetAnalysisPeriodImpl,
				public mcSetBeadDisplayIdImpl,
				public mcSetBeadTypeDisplayIdImpl,
				public mcSetCurrentStateCameraImpl,
				public mcSetCurrentStateDefaultFormatImpl,
				public mcSetCurrentStateLightImpl,
				public mcSetDensityPeriodImpl,
				public mcSetDisplayBeadRangeImpl,
				public mcSetDisplayPeriodImpl,
				public mcSetPolymerDisplayIdImpl,
				public mcSetPolymerTypeDisplayIdImpl,
				public mcSetRestartPeriodImpl,
				public mcSetRestartStateDefaultBeadCoordinatesImpl,
				public mcSetRestartStateDefaultInclusiveImpl,
				public mcSetRunCompleteIntervalImpl,
				public mcSetSamplePeriodImpl,
                public mcSetSAXSProcessBeadElectronNoImpl,
				public mcToggleBeadDisplayImpl,
				public mcToggleCurrentStateBoxImpl,
				public mcToggleDensityFieldOutputImpl,
				public mcTogglePolymerDisplayImpl,
				public mcToggleRestartWarningMessagesImpl,
				public mcWriteLogMessageImpl

{
	// CNTCells need access to the stress tensor vectors and the ISimBox needs
	// to call private member functions. The command implementation classes
	// need access to the CMonitor's data.

	friend class CCNTCell;
	friend class CExternalCNTCell;
	friend class ISimBox;
#if EnableMonitorCommand == SimCommandEnabled
	friend class  mcCommentImpl;
	friend class  mcGenerateRNGGaussianImpl;
	friend class  mcGenerateRNGUniformImpl;
	friend class  mcPanCurrentStateCameraImpl;
	friend class  mcSaveCommandHistoryImpl;
	friend class  mcSaveCommandHistoryToCurrentTimeImpl;
	friend class  mcSaveProtocolFileImpl;
	friend class  mcSetAllBeadsInvisibleImpl;
	friend class  mcSetAllBeadsVisibleImpl;
	friend class  mcShowAllProcessesImpl;
	friend class  mcShowModifiableProcessesImpl;
	friend class  mcToggleCurrentStateAnalysisImpl;
	friend class  mcToggleEnergyOutputImpl;
	friend class  mcToggleSliceEnergyOutputImpl;
	friend class  mcZoomCurrentStateCameraImpl;
#endif
	friend class  mcGetBuildDateImpl;
	friend class  mcGetBuildIdImpl;
	friend class  mcIssueRunCompleteFractionImpl;
	friend class  mcSaveAmiraCurrentStateImpl;
	friend class  mcSaveBead1dDensityProfileImpl;
	friend class  mcSaveBeadDensityFluctuationsImpl;
	friend class  mcSaveBeadDensityFluctuationsByTypeImpl;
	friend class  mcSaveBeadKineticTemperaturesImpl;
	friend class  mcSaveBeadVelocityScalarProduct1dProfileImpl;
	friend class  mcSaveCurrentStateImpl;
	friend class  mcSaveNonBeadInclusiveRestartStateImpl;
    friend class  mcSavePairCorrelationFunctionImpl;
	friend class  mcSaveParaviewCurrentStateImpl;
	friend class  mcSavePolymerBeadRDFImpl;
	friend class  mcSavePovrayCurrentStateImpl;
	friend class  mcSaveRestartStateImpl;
    friend class  mcSaveSAXSImpl;
	friend class  mcSetAnalysisPeriodImpl;
	friend class  mcSetBeadDisplayIdImpl;
	friend class  mcSetBeadTypeDisplayIdImpl;
	friend class  mcSetCurrentStateCameraImpl;
	friend class  mcSetCurrentStateDefaultFormatImpl;
	friend class  mcSetCurrentStateLightImpl;
	friend class  mcSetDensityPeriodImpl;
	friend class  mcSetDisplayBeadRangeImpl;
	friend class  mcSetDisplayPeriodImpl;
	friend class  mcSetPolymerDisplayIdImpl;
	friend class  mcSetPolymerTypeDisplayIdImpl;
	friend class  mcSetRestartPeriodImpl;
	friend class  mcSetRestartStateDefaultBeadCoordinatesImpl;
	friend class  mcSetRestartStateDefaultInclusiveImpl;
	friend class  mcSetRunCompleteIntervalImpl;
	friend class  mcSetSamplePeriodImpl;
    friend class  mcSetSAXSProcessBeadElectronNoImpl;
	friend class  mcToggleBeadDisplayImpl;
	friend class  mcToggleCurrentStateBoxImpl;
	friend class  mcToggleDensityFieldOutputImpl;
	friend class  mcTogglePolymerDisplayImpl;
	friend class  mcToggleRestartWarningMessagesImpl;
	friend class  mcWriteLogMessageImpl;

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CMonitor();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// Protected constructor and the public Instance() functions ensure that
	// only a single CMonitor object can be created. We pass it CSimState and
	// CSimBox pointers to initialise it for a new simulation.

	static CMonitor* Instance(CSimState* const psState, const ISimBox* const pISimBox);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

    // Helper functions relating to bead properties

    bool IsBeadTypeVisible(long type) const;

    // Functions related to bead density grid analysis

    inline bool IsDensityGridAnalysisOn() const {return m_bDensityGridOutput;}
	const CDensityState* GetLastDensityState() const;
	const DensityStateSequence& GetDensityStates() const;

	// Functions related to density field analysis

	inline bool	IsDensityFieldAnalysisOn() const {return !m_DensityFields.empty();}
	inline long GetDensityFieldTotal()	   const {return m_DensityFields.size();}



	// Allow the aggregates to get the the stress tensor calculation results

	inline double GetSliceStress11(long slice, long beadPairType) const {return m_vvSliceStress11.at(slice).at(beadPairType);}
	inline double GetSliceStress12(long slice, long beadPairType) const {return m_vvSliceStress12.at(slice).at(beadPairType);}
	inline double GetSliceStress13(long slice, long beadPairType) const {return m_vvSliceStress13.at(slice).at(beadPairType);}
	inline double GetSliceStress21(long slice, long beadPairType) const {return m_vvSliceStress21.at(slice).at(beadPairType);}
	inline double GetSliceStress22(long slice, long beadPairType) const {return m_vvSliceStress22.at(slice).at(beadPairType);}
	inline double GetSliceStress23(long slice, long beadPairType) const {return m_vvSliceStress23.at(slice).at(beadPairType);}
	inline double GetSliceStress31(long slice, long beadPairType) const {return m_vvSliceStress31.at(slice).at(beadPairType);}
	inline double GetSliceStress32(long slice, long beadPairType) const {return m_vvSliceStress32.at(slice).at(beadPairType);}
	inline double GetSliceStress33(long slice, long beadPairType) const {return m_vvSliceStress33.at(slice).at(beadPairType);}

	inline double GetSliceStressBond11(long slice, long bondType) const {return m_vvSliceStressBond11.at(slice).at(bondType);}
	inline double GetSliceStressBond12(long slice, long bondType) const {return m_vvSliceStressBond12.at(slice).at(bondType);}
	inline double GetSliceStressBond13(long slice, long bondType) const {return m_vvSliceStressBond13.at(slice).at(bondType);}
	inline double GetSliceStressBond21(long slice, long bondType) const {return m_vvSliceStressBond21.at(slice).at(bondType);}
	inline double GetSliceStressBond22(long slice, long bondType) const {return m_vvSliceStressBond22.at(slice).at(bondType);}
	inline double GetSliceStressBond23(long slice, long bondType) const {return m_vvSliceStressBond23.at(slice).at(bondType);}
	inline double GetSliceStressBond31(long slice, long bondType) const {return m_vvSliceStressBond31.at(slice).at(bondType);}
	inline double GetSliceStressBond32(long slice, long bondType) const {return m_vvSliceStressBond32.at(slice).at(bondType);}
	inline double GetSliceStressBond33(long slice, long bondType) const {return m_vvSliceStressBond33.at(slice).at(bondType);}

	inline double GetSliceStressBondPair11(long slice, long bpType) const {return m_vvSliceStressBondPair11.at(slice).at(bpType);}
	inline double GetSliceStressBondPair12(long slice, long bpType) const {return m_vvSliceStressBondPair12.at(slice).at(bpType);}
	inline double GetSliceStressBondPair13(long slice, long bpType) const {return m_vvSliceStressBondPair13.at(slice).at(bpType);}
	inline double GetSliceStressBondPair21(long slice, long bpType) const {return m_vvSliceStressBondPair21.at(slice).at(bpType);}
	inline double GetSliceStressBondPair22(long slice, long bpType) const {return m_vvSliceStressBondPair22.at(slice).at(bpType);}
	inline double GetSliceStressBondPair23(long slice, long bpType) const {return m_vvSliceStressBondPair23.at(slice).at(bpType);}
	inline double GetSliceStressBondPair31(long slice, long bpType) const {return m_vvSliceStressBondPair31.at(slice).at(bpType);}
	inline double GetSliceStressBondPair32(long slice, long bpType) const {return m_vvSliceStressBondPair32.at(slice).at(bpType);}
	inline double GetSliceStressBondPair33(long slice, long bpType) const {return m_vvSliceStressBondPair33.at(slice).at(bpType);}


	void Sample();
	void SampleProcess();

	// Functions needed by the ISimBox interface

	inline long   GetGridXCellNo()		const {return m_GridXCellNo;}
	inline long   GetGridYCellNo()		const {return m_GridYCellNo;}
	inline long	  GetGridZCellNo()		const {return m_GridZCellNo;}
	inline double GetGridXCellWidth()	const {return m_GridXCellWidth;}
	inline double GetGridYCellWidth()	const {return m_GridYCellWidth;}
	inline double GetGridZCellWidth()	const {return m_GridZCellWidth;}

	// SaveCurrentState() cannot be const because it stores pointers to the 
	// CCurrentState objects

	void SaveCurrentState();
	void SavePrevisCurrentState();
	void SaveProcessState();
	void SaveNonBeadInclusiveRestartState() const;
	void SaveRestartState() const;

    // Parallel versions of the above functions.

    void SampleP();
	void SaveCurrentStateP();

	// ****************************************
	// Protected local functions
protected:

	CMonitor(CSimState* const psState, const ISimBox* const pISimBox);

    // Command handler function to save a current state to file

    bool InternalSaveCurrentState(CCurrentStateFormat* const pFormat);

    // Command handler functions to change the monitor's analysis periods

	bool InternalSetAnalysisPeriod(long period);
	bool InternalSetDensityPeriod(long period);
	bool InternalSetDisplayPeriod(long period);
    bool InternalSetRestartPeriod(long period);
    bool InternalSetSamplePeriod(long period);

    void InternalToggleEnergyOutput(bool bNormalizePerBead);
	void InternalTogglePolymerDisplay(const zString polymerName) const;

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void CalculateSingleBeadData();
	void CalculateSingleBondData();
	void CalculateSingleBondPairData();
	void CalculateSinglePolymerData();
	void CalculateDensityFields();

	void FindAggregates();				// Search for any mesoscopic aggregates
	void FindRegions();

										// Pass stress contributions from CNTCells
	void InitialiseSliceStress();
	void ZeroSliceStress();
	void AddBeadStress(const CAbstractBead* const pBead1, const CAbstractBead* const pBead2, const double force[3], const double dx[3]);
	void AddBondStress(const CBond* const pBond);
	void AddBondPairStress(const CBondPair* const pBondPair);


										// Pass total energy contributions from CNTCells
	void ZeroTotalEnergy();
	void AddBeadEnergy(double kinetic, double potential);

	void UpdateBeadTypes();
	void UpdateBondTypes();

				
	// SaveDensityStates() cannot be const because it stores pointers to the 
	// CDensityState objects

	void SaveDensityStates();
	void SaveAggregateState() const;
	void SaveAnalysisState()  const;
	void SaveHistoryState()   const;
	void SaveLogState()		  const;
	void SaveRegionState()    const;
	void Saveud() const;

    // Parallel versions of the above functions.

	void SaveLogStateP() const;

	// ****************************************
	// Data members

private:

	static CMonitor* m_pInstance;		// Pointer to single instance of CMonitor class

	CSimState* const m_pSimState;		// Pointer is const but CSimState can be changed

	double m_SimBoxXLength;				// SimBox size for PBC calculations
	double m_SimBoxYLength;				// Note. These are not const to allow for
	double m_SimBoxZLength;				// future constant pressure simulations

	const long m_GridXCellNo;			// No of analysis grid cells in X dimension
	const long m_GridYCellNo;			//           "                  Y     "
	const long m_GridZCellNo;			//           "                  Z     "

	long m_BeadTypeSize;				// No of bead types
	long m_BondTypeSize;				// No of bond types
	const long m_BondPairTypeSize;		// No of bondpair types
	const long m_PolymerTypeSize;		// No of polymer types

	// Local values of some observables

	double m_meanTemp;		
	double m_meanPress;
	double m_meanKineticEnergy;
	double m_meanPotentialEnergy;
	double m_cmPos[3];
	double m_cmMom[3];
	double m_totalSqMom[3];
	double m_totalAngMom[3];
	double m_totalStress[9];
	double m_totalStressSphere[9];
	double m_totalInertia[9];

	// Data used in the aggregate analysis to calculate the stress tensor averaged
	// over slices through the SimBox. Note that the slices are hardwired in the 
	// Z direction for now.

	long m_StressBeadPairTotal;	// Defined in constructor; mustn't change
	long m_StressSliceTotal;			// Number of slices in SimBox for stress calculation
	long m_LowerStressSliceIndex;		// Index of slice containing lower bead
	long m_UpperStressSliceIndex;		// Index of slice containing upper bead

	double m_LowerStressSliceCoord;		// Real version of slice index
	double m_UpperStressSliceCoord;		// Real version of slice index
	double m_LowerStressSliceRem;		// Remainder from lower bead to next-lowest slice border
	double m_UpperStressSliceRem;		// Remainder from upper bead to next-lowest slice border

	double m_StressSliceWidth;			// Stress slice width
	double m_invSliceWidth;				// Inverse of stress tensor slice width
	double m_fdx[9];					// Array to hold temporary force*dx terms

	// 2-dimensional arrays to hold the contributions to the stress tensor
	// averaged over slices through the SimBox from the bead-bead interactions,
	// the bond forces (Hookean springs) and bondpair forces (3-body forces).

	zArray2dDouble m_vvSliceStress11;
	zArray2dDouble m_vvSliceStress12;
	zArray2dDouble m_vvSliceStress13;
	zArray2dDouble m_vvSliceStress21;
	zArray2dDouble m_vvSliceStress22;
	zArray2dDouble m_vvSliceStress23;
	zArray2dDouble m_vvSliceStress31;
	zArray2dDouble m_vvSliceStress32;
	zArray2dDouble m_vvSliceStress33;

	zArray2dDouble m_vvSliceStressBond11;
	zArray2dDouble m_vvSliceStressBond12;
	zArray2dDouble m_vvSliceStressBond13;
	zArray2dDouble m_vvSliceStressBond21;
	zArray2dDouble m_vvSliceStressBond22;
	zArray2dDouble m_vvSliceStressBond23;
	zArray2dDouble m_vvSliceStressBond31;
	zArray2dDouble m_vvSliceStressBond32;
	zArray2dDouble m_vvSliceStressBond33;

	zArray2dDouble m_vvSliceStressBondPair11;
	zArray2dDouble m_vvSliceStressBondPair12;
	zArray2dDouble m_vvSliceStressBondPair13;
	zArray2dDouble m_vvSliceStressBondPair21;
	zArray2dDouble m_vvSliceStressBondPair22;
	zArray2dDouble m_vvSliceStressBondPair23;
	zArray2dDouble m_vvSliceStressBondPair31;
	zArray2dDouble m_vvSliceStressBondPair32;
	zArray2dDouble m_vvSliceStressBondPair33;

	// Sequence of CAnalysis-derived objects corresponding to mesoscopic aggregates
	// to monitor during the run

	AggregateSequence	m_vAggregates;

	// Sequences of analysis objects created during the run to hold the data collected
	// from the aggregates and the overall system

	ObservableSequence		m_vAllObservables;		// Copy of all other observables
	ObservableSequence		m_vObservables;			// Most types of data analysed
	ObservableSequence		m_vBondObservables;		// Data relating to Bonds
	ObservableSequence		m_vBondPairObservables;	// Data relating to BondPairs
	ObservableSequence		m_vPolymerObservables;	// Data relating to Polymers
	GridObservableSequence	m_vGridObservables;		// Bead density fields
	CurrentStateSequence	m_vCurrentStates;		// Snapshots of bead/bond coordinates
	DensityStateSequence	m_vDensityStates;		// xxState-derived class to package
													// the CGridObservable object for 
													// serialization

	RegionSequence			m_Regions;				// All IRegionAnalysis objects created
	RegionSequence			m_ActiveRegions;		// Currently active IRegionAnalysis objects

	// Vectors holding observables

	zDoubleVector m_vBeadMSD;						// Bead mean-square deviation (diffusion)
	zDoubleVector m_vBondLength;					// Bond length by type
	zDoubleVector m_vBondPairFirstLength;			// Bondpair first bond length by type
	zDoubleVector m_vBondPairSecondLength;			// Bondpair second bond length by type
	zDoubleVector m_vBondPairP2;					// Bondpair Lengendre polynomial by type
	zDoubleVector m_vBondPairPhiRatio;				// Bondpair <phi**2>/<phi>**2 by type
	zDoubleVector m_vBondPairPhiSq;					// Bondpair <phi**2> by type
	zDoubleVector m_vBondPairPhi;					// Bondpair <phi> by type
	zDoubleVector m_vPolymerEE;						// Polymer end-to-end distance by type

	// Local data derived from other simulation data 

	long   m_SamplesTaken;					// Current no of samples made
	long   m_SampleNo;						// No of samples per analysis period
	double m_SampleSize;					// No of samples per analysis period
    long   m_GridSampleNo;					// No of samples per density period


	double m_HalfSimBoxXLength;				// Half SimBox size for PBC calculations
	double m_HalfSimBoxYLength;
	double m_HalfSimBoxZLength;

	double m_invSimBoxVolume;				// Inverse of volume to avoid divisions in normalization

	double m_GridXCellWidth;				// Width of analysis grid cells
	double m_GridYCellWidth;
	double m_GridZCellWidth;

	double m_BeadTotal;						// Total no of beads of all types

	zLongVector m_vBeadTypeTotal;			// No of beads of each type	
	zLongVector m_vBondTypeTotal;			// No of bonds of each type
	zLongVector m_vBondPairTypeTotal;		// No of bondpairs of each type
	zLongVector m_vPolymerTypeTotal;		// No of polymers of each type

	long m_FixedObservableNo;				// No of fixed observables (not depending on bond or polymer types)

	// **********************************************************************
	// Data relating to commands inherited from the IMonitorCmd interface.


	DensityFieldSequence	m_DensityFields;	// Set of bead density fields being calculated

	
	long m_RunCompleteInterval;				// Interval to issue mcIssueRunCompleteFraction commands

	// Vector showing which bead types are to be excluded from being displayed 
	// in CCurrentState files

	zLongVector m_ExcludedBeadTypes;

	double m_MinXFraction;					// Coordinate ranges used to restrict
	double m_MinYFraction;					// the display of beads in CCurrentState
	double m_MinZFraction;					// snapshots. They are specified as
	double m_MaxXFraction;					// fractions of the SimBox side lengths
	double m_MaxYFraction;
	double m_MaxZFraction;

	double m_Camera[3];						// Coordinates of CCurrentState camera
	double m_Target[3];

	zDoubleVector m_vLightX;				// Coordinates of lights added to CCurrentState output
	zDoubleVector m_vLightY;
	zDoubleVector m_vLightZ;

	zString m_DefaultCurrentStateFormat;	// Format of CCurrentState output for visualisation
	bool m_bCurrentStateAnalysis;			// Save and analyse CCurrentState snapshots
	bool m_bDisplayBox;						// Display bounding box in CCurrentState snapshot
	bool m_bRestrictCurrentStateCoords;		// Restrict current state bead coordinates
	bool m_bDensityGridOutput;				// Save density grid data to file
	bool m_bEnergyOutput;					// Write bead KE, PE and total energy to file
    bool m_bLogRestartWarningMessages;      // Log warning messages when bead coordinates are shifted in restart states
    bool m_bNormalizePerBead;				// Normalize energy terms by bead total or not

    bool m_bInclusiveRestartStates;         // Save inclusive restart states by default

};

#endif // !defined(AFX_MONITOR_H__890FD6E0_3DE8_11D3_820E_0060088AD300__INCLUDED_)
