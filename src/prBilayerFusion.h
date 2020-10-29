// prBilayerFusion.h: interface for the prBilayerFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBILAYERFUSION_H__787FE77B_044C_4F9C_AE46_BDCC9C786DC9__INCLUDED_)
#define AFX_PRBILAYERFUSION_H__787FE77B_044C_4F9C_AE46_BDCC9C786DC9__INCLUDED_


// Include files and forward declarations for template adaptor classes

#include "IBilayerFusion.h"

class CBilayer;



#include "xxProcess.h"

class prBilayerFusion : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prBilayerFusion();
	virtual ~prBilayerFusion();

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

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess() const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const;	// Return the process name

	// Function to allow the CInputData object to check all processes' data
	
	virtual bool ValidateData(const CInputData &riData);

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

	// ****************************************
	// Data members

private:							// Data specific to the process

	zString m_AggregateName1;		// Name of CAnalysis-derived class to monitor
	zString m_AggregateName2;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId1;			// Unique id of aggregate to monitor
	long	m_AggregateId2;			// Unique id of aggregate to monitor

	double	m_MaxApproach;			// Distance at which bilayers are apposite
	double	m_MinCuspCurvature;		// Minimum curvature for a cusp to register
	double	m_MaxDistalTouch;		// Distance at which bilayers outer leaflets "touch"
	long    m_MinPoreSize;			// Minimum size (grid points) of single-membrane pore
	long    m_MinFusionPoreSize;	// Minimum size (grid points) of fusion pore


	// Local data needed by the process

	// Pointers to wrapper classes for the two CBilayer aggregates to give
	// access their data, and the events supported by the aggregates

	const IBilayerFusion<CBilayer>*  m_pBilayer1;
	const IBilayerFusion<CBilayer>*  m_pBilayer2;

	// Local data created by the process from its aggregates and events

	// Flags showing which events on the path to fusion have occurred

	bool	m_bBilayerApproach;		// Indicates bilayers are closer than m_MaxApproach
	bool	m_bBilayerCusp1;		// Indicates a cusp has formed in bilayer 1
	bool	m_bBilayerCusp2;		// Indicates a cusp has formed in bilayer 2
	bool	m_bBilayerDistalMix;	// Indicates outer leaflets have mixed
	bool	m_bBilayerDistalTouch;	// Indicates that bilayer outer leaflets are closer than m_MaxDistalTouch
	bool    m_bBilayerFusionPore;	// Indicates a fusion pore connecting both bilayers
	bool	m_bBilayerPore1;		// Indicates that bilayer 1 has a pore
	bool	m_bBilayerPore2;		// Indicates that bilayer 2 has a pore
	bool	m_bBilayerProximalMix;	// Indicates inner leaflets have mixed


	double	m_CMSeparation;			// Distance between vesicle centres of mass
};

#endif // !defined(AFX_PRBILAYERFUSION_H__787FE77B_044C_4F9C_AE46_BDCC9C786DC9__INCLUDED_)
