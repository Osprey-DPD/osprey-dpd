// prBLMVesicleFusion.h: interface for the prBLMVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBLMVESICLEFUSION_H__9FB39810_D35E_4979_B6FE_3FD41E9072DD__INCLUDED_)
#define AFX_PRBLMVESICLEFUSION_H__9FB39810_D35E_4979_B6FE_3FD41E9072DD__INCLUDED_


// Include files and forward declarations for template adaptor classes

#include "IBilayerFusion.h"
#include "IVesicleFusion.h"

class CBilayer;
class CVesicle;



#include "IModifyBLMVesicleFusion.h"

class prBLMVesicleFusion : public IModifyBLMVesicleFusion
{
	// ****************************************
	// Construction/Destruction
public:

	prBLMVesicleFusion();
	virtual ~prBLMVesicleFusion();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess()  const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

// ****************************************
// Implementation of the IModifyProcess interface

	bool IsModifiable() const;
	void SetModifiable(const xxCommand* const pCommand);
	void SetUnmodifiable(const xxCommand* const pCommand);
	void SetIrreversibleUnmodifiable(const xxCommand* const pCommand);

// ****************************************
	// Implementation of the IModifyBLMVesicleFusion interface

	virtual void SetMaxCMApproach(const xxCommand* const pCommand);
	virtual void SetMaxDistalTouch(const xxCommand* const pCommand);
	virtual void SetMinMixThreshold(const xxCommand* const pCommand);
	virtual void SetMinPoreSize(const xxCommand* const pCommand);
	virtual void SetMinFusionPoreSize(const xxCommand* const pCommand);	
	virtual void SetCondenseDelay(const xxCommand* const pCommand);
	virtual void SetCondenseDuration(const xxCommand* const pCommand);
	virtual void SetStretchDelay(const xxCommand* const pCommand);
	virtual void SetStretchDuration(const xxCommand* const pCommand);
	virtual void SetInnerRadius(const xxCommand* const pCommand);
	virtual void SetOuterRadius(const xxCommand* const pCommand);
	virtual void SetHalfLength(const xxCommand* const pCommand);
	virtual void SetRadialForceMagnitude(const xxCommand* const pCommand);

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.

	void SetAggregate(const CAnalysis* pAggregate1, const CAnalysis* pAggregate2);

	void SetEvent();


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void WriteDensityProfile(const ISimBox* const pISimBox);

	// ****************************************
	// Data members

private:							// Data specific to the process

	// Local data for the process

	zString m_AggregateName1;		// Name of CAnalysis-derived class to monitor
	zString m_AggregateName2;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId1;			// Unique id of aggregate to monitor
	long	m_AggregateId2;			// Unique id of aggregate to monitor

	double	m_MaxCMApproach;		// CM separation at which BLM/vesicle are "close"
	double	m_MaxDistalTouch;		// Distance at which vesicle outer leaflets "touch"
	long    m_MinMixThreshold;		// Minimum number of lipids to enter monolayer for mixing
	long    m_MinPoreSize;			// Minimum size (grid points) of single-membrane pore
	long    m_MinFusionPoreSize;	// Minimum size (grid points) of fusion pore
	long	m_CondenseDelay;		// Delay after vesicle nears bilayer of condensation of headgroups
	long	m_CondenseDuration;		// Duration of condensation of headgroups
	long	m_StretchDelay;			// Delay after first contact before applying radial stretching force
	long	m_StretchDuration;		// Duration of radial stretching force
	double	m_InnerRadius;			// Inner radius of force annulus
	double  m_OuterRadius;			// Outer radius of      "          "
	double  m_HalfLength;			// Half length of       "          "
	double  m_RadialForceMag;		// Magnitude of (constant) radial stretching force


	// Local data needed by the process

	// Pointers to the two aggregates to access their data

	const IBilayerFusion<CBilayer>*  m_pBLM;
	const IVesicleFusion<CVesicle>*  m_pVesicle;

	// Local data created by the process from its aggregates and events

	// Flags showing which events on the path to fusion have occurred

	bool	m_bVesicleApproach;		// Indicates BLM-vesicle are closer than m_MaxApproach
	bool	m_bVesicleDistalTouch;	// Indicates that outer leaflets are closer than m_MaxDistalTouch
	bool	m_bVesicleDistalMix;	// Indicates outer leaflets have mixed
	bool	m_bVesicleProximalMix;	// Indicates inner leaflets have mixed
	bool    m_bStalk;				// Indicates a stalk has formed
	bool    m_bVesicleFusionPore;	// Indicates a fusion pore connecting BLM-vesicle
	bool	m_bBLMPore;				// Indicates that the BLM has ruptured prematurely
	bool	m_bVesiclePore;			// Indicates that the vesicle has ruptured prematurely


	bool	m_bVesicleAboveBLM;		// Flag showing if vesicle CM is above BLM in bilayer normal direction
	double  m_VesicleNormalCMCoord;	// Vesicle CM coordinate in direction of BLM normal
	double	m_CMSeparation;			// Distance between BLM-vesicle centres of mass
	double	m_OuterSeparation;		// Distance between outer monolayers of BLM and vesicle

	long	m_BLMLipidType;			// Bilayer lipid type
	long	m_BLMLipidHeadType;		// Bilayer lipid head bead type
	long	m_BLMLipidTailType;		// Bilayer lipid tail bead type
	long	m_VesicleLipidType;		// Vesicle lipid type
	long	m_VesicleLipidHeadType;	// Vesicle lipid head bead type
	long	m_VesicleLipidTailType;	// Vesicle lipid tail bead type

	long	m_OuterLeafletMixTotal;	// No of vesicle lipids in BLM outer leaflet
	long	m_InnerLeafletMixTotal;	// No of vesicle lipids in BLM inner leaflet

	long    m_MergedProximalIndex;	// CNT cell index of merged proximal leaflets

	long	m_PoreSize;				// No of grid points in pore area
	double	m_PoreArea;				// Pore area in units of bead diameter squared


	// Data to allow lipid density profiles to be created

	xxProcessState*  m_pDensityFile;

};

#endif // !defined(AFX_PRBLMVESICLEFUSION_H__9FB39810_D35E_4979_B6FE_3FD41E9072DD__INCLUDED_)
