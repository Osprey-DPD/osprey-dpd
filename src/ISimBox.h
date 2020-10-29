// ISimBox.h: interface for the ISimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMBOX_H__CA81BE73_AD75_11D3_BF16_004095086186__INCLUDED_)
#define AFX_ISIMBOX_H__CA81BE73_AD75_11D3_BF16_004095086186__INCLUDED_

#include "xxBase.h"	// Added by ClassView


// Forward declarations

class ISimCmd;
class ISimEvent;
class IActiveSimBox;
class IMonitorCmd;
class CMonitor;

// Include files

#include "SimACNFlags.h"
#include "SimState.h"
#include "SimBox.h"

#include "IGlobalSimBox.h"

class ISimBox : public IGlobalSimBox
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ISimBox();

	// ****************************************
	// Global functions, static member functions and variables
public:
	// Public Instance() function and protected constructor ensure that
	// only a single ISimBox interface object can be created. 
	// We pass it the CSimState object so that the SimBox and Monitor 
	// objects can initialise themselves.

	static ISimBox* Instance(CSimState& simState);

private:

	static ISimBox* m_pInstance;	// Pointer to single instance of ISimBox class

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    // Functions to return pointers to the SimBox and Monitor command interfaces.
	// The pointers cannot be const because of errors on the unix compilers.

	ISimCmd*		IISimCmd()		 const;
	ISimEvent*		IISimEvent()	 const;
	ISimState*		IISimState()	 const;
	IMonitorCmd*	IIMonitorCmd()   const;

    // Functions to return pointers to the SimBox and Monitor: note that they
    // cannot be altered via these pointer. Explicit functions are provided below
    // for modifying them.

	inline const CSimBox* GetSimBox()   const {return m_pSimBox;}
	inline const CMonitor* GetMonitor() const {return m_pMonitor;}


#if EnableShadowSimBox == SimACNEnabled
	IActiveSimBox*	IIActiveSimBox() const;
#endif

    // Function to check if the parallel SimBox instance was created successfully
    // on all processors.

    bool GetParallelAssemblyFailed() const;

	// Forwarding functions to the CSimState

	inline const zString GetRunId()				const {return m_rSimState.GetRunId();}
	inline const zString GetTitle()				const {return m_rSimState.GetTitle();}
	inline const zString GetDate()				const {return m_rSimState.GetDate();}
	inline const zString GetComment()			const {return m_rSimState.GetComment();}
	inline const zString GetInitialStateType()	const {return m_rSimState.GetInitialStateType();}

	inline long	GetSamplePeriod()				const {return m_rSimState.GetSamplePeriod();}
	inline long	GetAnalysisPeriod()				const {return m_rSimState.GetAnalysisPeriod();}
	inline long GetDensityPeriod()				const {return m_rSimState.GetDensityPeriod();}
	inline long	GetDisplayPeriod()				const {return m_rSimState.GetDisplayPeriod();}
	inline long	GetRestartPeriod()				const {return m_rSimState.GetRestartPeriod();}

	inline long	GetCNTXCellNo()					const {return m_rSimState.GetCNTXCellNo();}
	inline long	GetCNTYCellNo()					const {return m_rSimState.GetCNTYCellNo();}
	inline long	GetCNTZCellNo()					const {return m_rSimState.GetCNTZCellNo();}
												
	inline double GetCNTXCellWidth()			const {return m_rSimState.GetCNTXCellWidth();}
	inline double GetCNTYCellWidth()			const {return m_rSimState.GetCNTYCellWidth();}
	inline double GetCNTZCellWidth()			const {return m_rSimState.GetCNTZCellWidth();}


	// Functions to replace the above references to the CSimState in the parallel codebase.
	// As the number of entities in each processor's Space varies in time, we replace
	// functions that use CSimState by new ones that directly access the CSimBox.
	// But we still keep track of the numbers of bead types, etc and update them
	// in the CSimState.

	long GetBeadTotal() const;
	AbstractBeadVector GetBeads() const;

	inline long GetBondTotal()					const {return m_rSimState.GetBondTotal();}
	inline long GetBondPairTotal()				const {return m_rSimState.GetBondPairTotal();}
	inline long GetPolymerTotal()				const {return m_rSimState.GetPolymerTotal();}

	inline const BondVector&     GetBonds()		const {return m_rSimState.GetBonds();}
	inline const BondPairVector& GetBondPairs()	const {return m_rSimState.GetBondPairs();}
    inline const BondVector&     GetPolymerisedBonds()	const {return m_rSimState.GetPolymerisedBonds();}
	inline const PolymerVector&  GetPolymers()	const {return m_rSimState.GetPolymers();}

	inline long	GetBeadTypeTotal()				const {return m_rSimState.GetBeadTypeTotal();}
	inline long GetBondTypeTotal()				const {return m_rSimState.GetBondTypeTotal();}
	inline long GetBondPairTypeTotal()			const {return m_rSimState.GetBondPairTypeTotal();}
	inline long GetPolymerTypeTotal()			const {return m_rSimState.GetPolymerTypeTotal();}

	const BeadVector&		GetBeadTypes() const;
	const BondVector&		GetBondTypes() const;
	const BondPairVector&	GetBondPairTypes() const;
	const PolymerVector&	GetPolymerTypes() const;

	const zString GetBeadNameFromType(long type) const;
	const zString GetBondNameFromType(long type) const;
	const zString GetBondHeadNameFromType(long type) const;
	const zString GetBondTailNameFromType(long type) const;
	const zString GetBondPairNameFromType(long type) const;
	const zString GetBondPairHeadNameFromType(long type) const;
	const zString GetBondPairMiddleNameFromType(long type) const;
	const zString GetBondPairTailNameFromType(long type) const;
	const zString GetPolymerNameFromType(long type) const;

    long GetBeadTypeFromName(const zString name) const;
	
	long GetBeadTotalForType(long type) const;
	long GetBondTotalForType(long type) const;
	long GetBondPairTotalForType(long type) const;
	long GetPolymerTotalForType(long type) const;

	long   GetPolymerSizeFromType(long type) const;
	double GetPolymerFractionFromType(long type) const;
	const zString GetPolymerShapeFromType(long polyType) const;
	const zString GetSpaceWrappedPolymerShapeFromType(long polyType) const;

	IModifyProcess* GetModifiableProcessFromName(const zString name) const;
	IModifyProcess* GetModifiableProcessFromPid(const zString pid) const;

	double GetDPDBeadConsInt(long first, long second) const;
	double GetDPDBeadDissInt(long first, long second) const;

	inline double GetDensity()					const {return m_rSimState.GetDensity();}
	inline double GetTemperature()				const {return m_rSimState.GetkT();}
	inline long   GetRNGSeed()					const {return m_rSimState.GetRNGSeed();}
	inline double GetLambda()					const {return m_rSimState.GetLambda();}
	inline double GetStepSize()					const {return m_rSimState.GetIntegrationStep();}

	inline ProcessSequence GetProcesses()		const {return m_rSimState.GetProcesses();}
	inline EventSequence   GetEvents()			const {return m_rSimState.GetEvents();}
	inline CommandSequence GetCommands()		const {return m_rSimState.GetCommands();}

	// Forwarding functions to the CSimBox

	inline void AddCommand(const xxCommand* const pCommand) const {m_pSimBox->AddCommand(pCommand);}
	inline void AddProcess(xxProcess* const pProcess)       const {m_pSimBox->AddProcess(pProcess);}

	inline void Run()							const {m_pSimBox->Run();}
	inline void RunMCRelaxation()				const {m_pSimBox->RunMCRelaxation();}
												
	inline long GetCurrentTime()				const {return m_pSimBox->GetCurrentTime();}
	inline long GetTotalTime()					const {return m_pSimBox->GetTotalTime();}

	inline long	 GetCNTCellTotal()				const {return m_pSimBox->GetCNTCellTotal();}
	inline const CNTCellVector&  GetCNTCells()	const {return m_pSimBox->GetCNTCells();}
												
	inline double GetSimSpaceXLength()			const {return m_rSimState.GetSimSpaceXLength();}
	inline double GetSimSpaceYLength()			const {return m_rSimState.GetSimSpaceYLength();}
	inline double GetSimSpaceZLength()			const {return m_rSimState.GetSimSpaceZLength();}
	inline double GetSimBoxXLength()			const {return m_pSimBox->GetXLength();}
	inline double GetSimBoxYLength()			const {return m_pSimBox->GetYLength();}
	inline double GetSimBoxZLength()			const {return m_pSimBox->GetZLength();}
	inline double GetHalfSimBoxXLength()		const {return m_pSimBox->GetHalfXLength();}
	inline double GetHalfSimBoxYLength()		const {return m_pSimBox->GetHalfYLength();}
	inline double GetHalfSimBoxZLength()		const {return m_pSimBox->GetHalfZLength();}
	inline double GetVolume()					const {return m_pSimBox->GetVolume();}
									
	inline long GetStressCellMultiplier()               const {return m_pSimBox->GetStressCellMultiplier();}
	inline long GetStressCellRatio()                    const {return m_pSimBox->GetStressCellRatio();}
	inline long GetStressCellXNo()                      const {return m_pSimBox->GetStressCellXNo();}
	inline long GetStressCellYNo()                      const {return m_pSimBox->GetStressCellYNo();}
	inline long GetStressCellZNo()                      const {return m_pSimBox->GetStressCellZNo();}
	inline long	 GetStressCellTotal()	                const {return m_pSimBox->GetStressCellTotal();}
	inline StressCellVector&  GetStressCells()	        const {return m_pSimBox->GetStressCells();}
	void    SetStressSphereOrigin(double rcm[3])        const;
	void    TransformStressSphere(double stress[9])     const;


	// Forwarding functions to the Monitor: not inline to separate the Monitor
	// implementation from this interface class

	void    InternalTogglePolymerDisplay(const zString polymerName)  const;

	void	Sample()				 const;
	void	SampleProcess()			 const;
	void	SaveCurrentState()		 const;
	void    SavePrevisCurrentState() const;
	void	SaveProcessState()		 const;
	void	SaveRestartState()		 const;
	void    Saveud()                 const;
	void	UpdateBeadTypes()		 const;
	void	UpdateBondTypes()		 const;

	void	ZeroSliceStress()		 const;
	void    AddBeadStress(double rp[3], double rq[3], double stress[9]) const;
	void	AddBondStress(const CBond* const pBond) const;
	void	AddBondPairStress(const CBondPair* const pBondPair) const;

	void	ZeroTotalEnergy()		 const;

	long	GetGridXCellNo()		 const;
	long	GetGridYCellNo()		 const;
	long	GetGridZCellNo()		 const;
	double	GetGridXCellWidth()		 const;
	double	GetGridYCellWidth()		 const;
	double	GetGridZCellWidth()		 const;



	// ****************************************
	// Protected local functions
protected:

    ISimBox(CSimState& simState);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	CSimBox* m_pSimBox;				// Single SimBox object doing simulation

	CMonitor* m_pMonitor;			// Single Monitor object

	const CSimState& m_rSimState;	// CSimState object
};

#endif // !defined(AFX_ISIMBOX_H__CA81BE73_AD75_11D3_BF16_004095086186__INCLUDED_)
