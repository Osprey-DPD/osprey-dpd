// IMonitorCmd.h: interface for the IMonitorCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMONITORCMD_H__7DEE8083_57D7_11D4_BF42_004095086186__INCLUDED_)
#define AFX_IMONITORCMD_H__7DEE8083_57D7_11D4_BF42_004095086186__INCLUDED_


// Forward declarations

class xxCommand;

// Conditional compilation flags
#include "SimCommandFlags.h"


class IMonitorCmd
{
public:
	virtual ~IMonitorCmd();

	// Command interface functions to change the display and analysis of
	// data during a simulation. These must be implemented by the CMonitor
	// class or passed on to the next link via the ISimBox interface
	// class.

#if EnableMonitorCommand == SimCommandEnabled
    virtual void		                         Comment(const xxCommand* const pCommand) = 0;
    virtual void		             GenerateRNGGaussian(const xxCommand* const pCommand) = 0;
    virtual void		              GenerateRNGUniform(const xxCommand* const pCommand) = 0;
	virtual void		           PanCurrentStateCamera(const xxCommand* const pCommand) = 0;
	virtual void			          SaveCommandHistory(const xxCommand* const pCommand) = 0;
	virtual void         SaveCommandHistoryToCurrentTime(const xxCommand* const pCommand) = 0;
	virtual void				        SaveProtocolFile(const xxCommand* const pCommand) = 0;
	virtual void			        SetAllBeadsInvisible(const xxCommand* const pCommand) = 0;
	virtual void			          SetAllBeadsVisible(const xxCommand* const pCommand) = 0;
	virtual void				        ShowAllProcesses(const xxCommand* const pCommand) = 0;
	virtual void		         ShowModifiableProcesses(const xxCommand* const pCommand) = 0;
	virtual void	          ToggleCurrentStateAnalysis(const xxCommand* const pCommand) = 0;
	virtual void			          ToggleEnergyOutput(const xxCommand* const pCommand) = 0;
	virtual void		         ToggleSliceEnergyOutput(const xxCommand* const pCommand) = 0;
	virtual void		          ZoomCurrentStateCamera(const xxCommand* const pCommand) = 0;
#endif
    virtual void		                    GetBuildDate(const xxCommand* const pCommand) = 0;
	virtual void		                      GetBuildId(const xxCommand* const pCommand) = 0;
	virtual void		        IssueRunCompleteFraction(const xxCommand* const pCommand) = 0;
	virtual void		           SaveAmiraCurrentState(const xxCommand* const pCommand) = 0;
	virtual void                SaveBead1dDensityProfile(const xxCommand* const pCommand) = 0;
	virtual void             SaveBeadDensityFluctuations(const xxCommand* const pCommand) = 0;
	virtual void       SaveBeadDensityFluctuationsByType(const xxCommand* const pCommand) = 0;
	virtual void             SaveBeadKineticTemperatures(const xxCommand* const pCommand) = 0;
	virtual void  SaveBeadVelocityScalarProduct1dProfile(const xxCommand* const pCommand) = 0;
	virtual void			         SaveCurrentStateCmd(const xxCommand* const pCommand) = 0;
	virtual void     SaveNonBeadInclusiveRestartStateCmd(const xxCommand* const pCommand) = 0;
    virtual void             SavePairCorrelationFunction(const xxCommand* const pCommand) = 0;
	virtual void		        SaveParaviewCurrentState(const xxCommand* const pCommand) = 0;
	virtual void                      SavePolymerBeadRDF(const xxCommand* const pCommand) = 0;
	virtual void		          SavePovrayCurrentState(const xxCommand* const pCommand) = 0;
	virtual void			         SaveRestartStateCmd(const xxCommand* const pCommand) = 0;
    virtual void                                SaveSAXS(const xxCommand* const pCommand) = 0;
	virtual void			           SetAnalysisPeriod(const xxCommand* const pCommand) = 0;
	virtual void		                SetBeadDisplayId(const xxCommand* const pCommand) = 0;
	virtual void		            SetBeadTypeDisplayId(const xxCommand* const pCommand) = 0;
	virtual void		           SetCurrentStateCamera(const xxCommand* const pCommand) = 0;
	virtual void	        SetCurrentStateDefaultFormat(const xxCommand* const pCommand) = 0;
	virtual void			        SetCurrentStateLight(const xxCommand* const pCommand) = 0;
	virtual void				        SetDensityPeriod(const xxCommand* const pCommand) = 0;
	virtual void			         SetDisplayBeadRange(const xxCommand* const pCommand) = 0;
	virtual void				        SetDisplayPeriod(const xxCommand* const pCommand) = 0;
	virtual void		             SetPolymerDisplayId(const xxCommand* const pCommand) = 0;
	virtual void		         SetPolymerTypeDisplayId(const xxCommand* const pCommand) = 0;
	virtual void				        SetRestartPeriod(const xxCommand* const pCommand) = 0;
	virtual void   SetRestartStateDefaultBeadCoordinates(const xxCommand* const pCommand) = 0;
	virtual void	     SetRestartStateDefaultInclusive(const xxCommand* const pCommand) = 0;
	virtual void		          SetRunCompleteInterval(const xxCommand* const pCommand) = 0;
	virtual void				         SetSamplePeriod(const xxCommand* const pCommand) = 0;
    virtual void            SetSAXSProcessBeadElectronNo(const xxCommand* const pCommand) = 0;
	virtual void			           ToggleBeadDisplay(const xxCommand* const pCommand) = 0;
	virtual void		           ToggleCurrentStateBox(const xxCommand* const pCommand) = 0;
	virtual void		        ToggleDensityFieldOutput(const xxCommand* const pCommand) = 0;
	virtual void			        TogglePolymerDisplay(const xxCommand* const pCommand) = 0;
	virtual void	        ToggleRestartWarningMessages(const xxCommand* const pCommand) = 0;
	virtual void			             WriteLogMessage(const xxCommand* const pCommand) = 0;

protected:

	IMonitorCmd();

};

#endif // !defined(AFX_IMONITORCMD_H__7DEE8083_57D7_11D4_BF42_004095086186__INCLUDED_)
