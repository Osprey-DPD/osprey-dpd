// ISimBoxBase.h: interface for the ISimBoxBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMBOXBASE_H__02D40F1A_D951_4ABE_BA32_F2807D303438__INCLUDED_)
#define AFX_ISIMBOXBASE_H__02D40F1A_D951_4ABE_BA32_F2807D303438__INCLUDED_


// Forward declarations

class ISimBox;
class CBond;
class CBondPair;
	
#include "xxParallelBase.h"

class ISimBoxBase : public xxParallelBase
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ISimBoxBase();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	// Access functions for derived classes. All are const.

	const zString GetRunId() const;

	long	GetCurrentTime() const;
	long	GetTotalTime()	 const;

	long	GetSimBoxBeadTypeTotal()	const;
	long	GetSimBoxBondTypeTotal()	const;
	long	GetSimBoxBondPairTypeTotal()			const;

	const   AbstractBeadVector GetSimBoxBeads()		const;
	const   BondVector&		   GetSimBoxBonds()		const;
	const   BondPairVector&    GetSimBoxBondPairs()	const;
	const   CNTCellVector&	   GetSimBoxCells()		const;

	long	GetSimBoxBeadTotal()		const;
	long	GetSimBoxBondTotal()		const;
	long	GetSimBoxBondPairTotal()	const;
	long	GetSimBoxXCellNo()			const;
	long	GetSimBoxYCellNo()			const;
	long	GetSimBoxZCellNo()			const;
	long	GetSimBoxCellTotal()		const;

	double	GetSimBoxXCellWidth()		const;
	double	GetSimBoxYCellWidth()		const;
	double	GetSimBoxZCellWidth()		const;
	double	GetSimBoxXLength()			const;
	double	GetSimBoxYLength()			const;
	double	GetSimBoxZLength()			const;
	double	GetSimBoxHalfXLength()		const;
	double	GetSimBoxHalfYLength()		const;
	double	GetSimBoxHalfZLength()		const;
	double	GetSimBoxVolume()			const;
	double  GetSimSpaceXLength()        const;
	double  GetSimSpaceYLength()        const;
	double  GetSimSpaceZLength()        const;

// Analysis commands forwarded to the Monitor for handling

	void                                    Comment(const xxCommand* const pCommand) const;
	void                        GenerateRNGGaussian(const xxCommand* const pCommand) const;
	void                         GenerateRNGUniform(const xxCommand* const pCommand) const;
	void                               GetBuildDate(const xxCommand* const pCommand) const;
	void                                 GetBuildId(const xxCommand* const pCommand) const;
	void	               IssueRunCompleteFraction(const xxCommand* const pCommand) const;
	void                      PanCurrentStateCamera(const xxCommand* const pCommand) const;
	void                                     RunGud(const xxCommand* const pCommand) const;
	void		              SaveAmiraCurrentState(const xxCommand* const pCommand) const;
	void	               SaveBead1dDensityProfile(const xxCommand* const pCommand) const;
	void                SaveBeadKineticTemperatures(const xxCommand* const pCommand) const;
	void     SaveBeadVelocityScalarProduct1dProfile(const xxCommand* const pCommand) const;
	void                SaveBeadDensityFluctuations(const xxCommand* const pCommand) const;
	void          SaveBeadDensityFluctuationsByType(const xxCommand* const pCommand) const;
    void                         SaveCommandHistory(const xxCommand* const pCommand) const;
	void            SaveCommandHistoryToCurrentTime(const xxCommand* const pCommand) const;
	void		                SaveCurrentStateCmd(const xxCommand* const pCommand) const;
	void        SaveNonBeadInclusiveRestartStateCmd(const xxCommand* const pCommand) const;
    void                SavePairCorrelationFunction(const xxCommand* const pCommand) const;
	void	               SaveParaviewCurrentState(const xxCommand* const pCommand) const;
    void                         SavePolymerBeadRDF(const xxCommand* const pCommand) const;
	void	                 SavePovrayCurrentState(const xxCommand* const pCommand) const;
	void			               SaveProtocolFile(const xxCommand* const pCommand) const;
	void		                SaveRestartStateCmd(const xxCommand* const pCommand) const;
    void                                   SaveSAXS(const xxCommand* const pCommand) const;
	void		               SetAllBeadsInvisible(const xxCommand* const pCommand) const;
	void		                 SetAllBeadsVisible(const xxCommand* const pCommand) const;
	void                          SetAnalysisPeriod(const xxCommand* const pCommand) const;
	void                           SetBeadDisplayId(const xxCommand* const pCommand) const;
	void                       SetBeadTypeDisplayId(const xxCommand* const pCommand) const;
	void		              SetCurrentStateCamera(const xxCommand* const pCommand) const;
	void               SetCurrentStateDefaultFormat(const xxCommand* const pCommand) const;
	void		               SetCurrentStateLight(const xxCommand* const pCommand) const;
	void			               SetDensityPeriod(const xxCommand* const pCommand) const;
	void		                SetDisplayBeadRange(const xxCommand* const pCommand) const;
	void		                   SetDisplayPeriod(const xxCommand* const pCommand) const;
	void                        SetPolymerDisplayId(const xxCommand* const pCommand) const;
	void                    SetPolymerTypeDisplayId(const xxCommand* const pCommand) const;
	void		                   SetRestartPeriod(const xxCommand* const pCommand) const;
	void      SetRestartStateDefaultBeadCoordinates(const xxCommand* const pCommand) const;
	void            SetRestartStateDefaultInclusive(const xxCommand* const pCommand) const;
	void	                 SetRunCompleteInterval(const xxCommand* const pCommand) const;
	void                            SetSamplePeriod(const xxCommand* const pCommand) const;
    void               SetSAXSProcessBeadElectronNo(const xxCommand* const pCommand) const;
	void			               ShowAllProcesses(const xxCommand* const pCommand) const;
	void	                ShowModifiableProcesses(const xxCommand* const pCommand) const;
	void			              ToggleBeadDisplay(const xxCommand* const pCommand) const;
	void                 ToggleCurrentStateAnalysis(const xxCommand* const pCommand) const;
	void		              ToggleCurrentStateBox(const xxCommand* const pCommand) const;
	void	               ToggleDensityFieldOutput(const xxCommand* const pCommand) const;
	void		                 ToggleEnergyOutput(const xxCommand* const pCommand) const;
	void	                ToggleSliceEnergyOutput(const xxCommand* const pCommand) const;
	void		               TogglePolymerDisplay(const xxCommand* const pCommand) const;
	void               ToggleRestartWarningMessages(const xxCommand* const pCommand) const;
	void	                 ZoomCurrentStateCamera(const xxCommand* const pCommand) const;
	void                            WriteLogMessage(const xxCommand* const pCommand) const;

// Other functions forwarded to the Monitor

	void Sample()			const;
	void SampleProcess()	const;
	void SaveCurrentState() const;
	void SaveProcessState() const;
	void SaveRestartState() const;
	void UpdateBeadTypes()  const;
	void UpdateBondTypes()  const;

	void ZeroSliceStress()	const ;
	void AddBondStress(const CBond* const pBond) const;
	void AddBondPairStress(const CBondPair* const pBondPair) const;

	void ZeroTotalEnergy() const;

	// Function to allow derived classes access to the ISimBox interface.
	// This is used extensively in aggregate analysis.

	inline const ISimBox*  GetISimBox() const {return m_pISimBox;}


	// ****************************************
	// Protected local functions
protected:

	ISimBoxBase(const ISimBox* const pISimBox);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	const ISimBox* const m_pISimBox;		// Interface class between derived classes
};

#endif // !defined(AFX_ISIMBOXBASE_H__02D40F1A_D951_4ABE_BA32_F2807D303438__INCLUDED_)

