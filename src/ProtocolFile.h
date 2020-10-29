// ProtocolFile.h: interface for the CProtocolFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLFILE_H__32F7A271_8475_459E_A2FF_D5AC8FC7682E__INCLUDED_)
#define AFX_PROTOCOLFILE_H__32F7A271_8475_459E_A2FF_D5AC8FC7682E__INCLUDED_


// Forward declarations

class ISimBox;

// Include files

#include "PolymerParser.h"


#include "xxFile.h"

class CProtocolFile : public xxFile
{
	friend class mcSaveProtocolFileImpl;

	// ****************************************
	// Construction/Destruction
public:

	CProtocolFile(const ISimBox* const pISimBox,
				  const zString simType, const zString protocol, const zString runId, 
				  const zString title, const zString date, const zString comment);

	virtual ~CProtocolFile();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual bool Serialize();


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation

protected:



	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

	const ISimBox* const m_pISimBox;	// ISimBox pointer for access to polymer data

	CPolymerParser m_Parser;			// Parser to convert polymer shapes to tags

	// Experiment Data
	// ***************

	// User
	const zString	m_UserName;			// Name of experiment user
	const zString	m_UserID;			// Authorisation code for user
	const zString	m_UserEmail;		// Email address of user

	// Project
	const zString	m_ProjectTitle;			// Title of project
	const zString	m_StartDate;			// Date project commenced
	const zString	m_Duration;				// Duration of project
	const zString	m_DurationUnits;		// Units of duation, "Weeks", "Months", "Years"
	const zString	m_PrincipalInvestigator;// PI on project
	const zString	m_ProjectDescription;	// Description of project


	// Security


	// Deployment



	// Simulation Data
	// ***************

	const zString m_SimType;		// dpd or md, Note. Case sensitive
	const zString m_Protocol;		// Protocol identifier
	const zString m_RunId;			// Control data file suffix
	const zString m_Title;
	const zString m_Date;
	const zString m_Comment;
									//	<SimBox>

	long	m_CNTXCellNo;			//	<BoxSize>
	long	m_CNTYCellNo;
	long	m_CNTZCellNo;

	double	m_CNTXCellWidth;		//	<CNTCellSize>
	double	m_CNTYCellWidth;
	double	m_CNTZCellWidth;

	long	m_GridXCellNo;			//	<GridSize>
	long	m_GridYCellNo;
	long	m_GridZCellNo;

	double	m_Density;				//	<Density>
	double	m_Temperature;			//	<Temperature>
	long	m_RNGSeed;				//	<RNGSeed>
	double	m_Lambda;				//	<Lambda>

									//	<SimTimes>

	double	m_StepSize;				//	<StepSize>
	long	m_StepTotal;			//	<StepTotal>
	long	m_SamplePeriod;			//	<SamplePeriod>
	long	m_AnalysisPeriod;		//	<AnalysisPeriod>
	long	m_DensityPeriod;		//	<DensityPeriod>
	long	m_DisplayPeriod;		//	<DisplayPeriod>
	long	m_RestartPeriod;		//	<RestartPeriod>


	// Substrate Data
	// ***************

	BeadVector		m_BeadTypes;		// Set of polymer types
	BondVector		m_BondTypes;		// Set of polymer types
	BondPairVector	m_BondPairTypes;	// Set of polymer types
	PolymerVector	m_PolymerTypes;		// Set of polymer types

	// Sculpture Data
	// ***************


	// ProbeField Data
	// ***************


	// ExternalField Data
	// ***************


	// Processes Data
	// ***************

	ProcessSequence		m_Processes;
	AggregateSequence	m_Aggregates;
	EventSequence		m_Events;
	CommandSequence		m_Commands;



};

#endif // !defined(AFX_PROTOCOLFILE_H__32F7A271_8475_459E_A2FF_D5AC8FC7682E__INCLUDED_)
