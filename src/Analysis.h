// Analysis.h: interface for the CAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSIS_H__A4AEA916_9D0D_11D3_BF15_004095086186__INCLUDED_)
#define AFX_ANALYSIS_H__A4AEA916_9D0D_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CSimState;
class ISimBox;
class CAnalysisData;
class CAnalysisState;
class CAnalysisTool;
class aaStressTensor;
class CSliceProfile;
class CSlice;

// Include files

#include "xxAggregateState.h"		// Needed by all derived classes

// Events that can be supported by a derived class must have their IEventXXX
// interface class specified as a public base class here. A derived class that 
// wants to implement the evXXX event overrides the IEventXXX class virtual member 
// functions. Derived classes that don't support an event inherit the virtual
// functions provided in the IEventXXX class.

#include "xxBase.h"
#include "SimProcessFlags.h"

#if EnableAnalysisEvents == SimProcessEnabled
  #include "IEventLamellaBilayerDomain.h"
  #include "IEventLamellaBilayerPhaseSeparation.h"
  #include "IEventLamellaCusp.h"
  #include "IEventLamellaFlipflop.h"
  #include "IEventLamellaMonolayerDomain.h"
  #include "IEventLamellaMonolayerPhaseSeparation.h"
  #include "IEventLamellaPore.h"
  #include "IEventLamellaPoreClosed.h"
  #include "IEventMotorATPase.h"
  #include "IEventVesicleBindsWall.h"
  #include "IEventVesicleUnbindsWall.h"
#endif

#if EnableAnalysisEvents == SimProcessEnabled
class CAnalysis : public xxBase, public IEventLamellaBilayerDomain,
								 public IEventLamellaBilayerPhaseSeparation,
								 public IEventLamellaCusp,
								 public IEventLamellaFlipflop,
								 public IEventLamellaMonolayerDomain,
								 public IEventLamellaMonolayerPhaseSeparation,
								 public IEventLamellaPore,
								 public IEventLamellaPoreClosed,
								 public IEventMotorATPase,
								 public IEventVesicleBindsWall,
								 public IEventVesicleUnbindsWall
#else
class CAnalysis : public xxBase
#endif
{
public:
	// ****************************************
	// Construction/Destruction

	CAnalysis(const zString runId, long sample, const CAnalysisData* const pAD);
	virtual ~CAnalysis();

	// ****************************************
	// Static member functions

	static long GetAggregateTotal();
	static void ZeroAggregateTotal();

	// ****************************************
	// Public access functions

	inline long				GetId()						const {return m_id;}
	inline const zString	GetType()					const {return m_Type;}
	bool					TimeToSample(long simTime)	const;
	bool					TimeToAnalyse(long simTime, long analysisPeriod) const;

	// ****************************************
	// Pure virtual functions that must be implemented by all derived classes

	// Function to pass the current simulation state to the analysis object for it to
	// search for its aggregate

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox) = 0;

	// Function to convert bead, bond or polymer names from strings to integers.

	virtual void ConvertNames(const CAnalysisState& raState) = 0;

	// ****************************************
	// Function to tell the enclosed aggregate state object to write its data to file

	bool Serialize() const;

	// Function returning a reference to the vector of CObservables in 
	// an aggregate analysis object

	inline ObservableSequence& GetObservables() {return m_vObservables;}

	// ****************************************
	// Functions used by derived classes to calculate the stress tensor averaged
	// over various profiles through the SimBox

protected:

	aaStressTensor* StressTensorProfile(const ISimBox* const pISimBox,
										CSliceProfile* const pProfile, 
										long xn, long yn, long zn);

	aaStressTensor* StressTensorProfile(const ISimBox* const pISimBox,
										CSlice* const pSlice, 
										long xn, long yn, long zn);


	// Static member variables

private:
	static long m_AggregateTotal;		// Number of aggregates created


	// Local member variables

protected:

	const long		m_id;			// Unique id for aggregate
	const zString	m_Type;			// Identifier for aggregate being analysed
	long			m_Start;		// Simulation time during which to perform analysis
	long			m_End;
	long			m_SamplePeriod;	// Sampling period for analysis

	xxAggregateState* m_pState;	// Time series data for the aggregates

	// Set of observables used by derived class objects in their analysis

	ObservableSequence m_vObservables;

	// Set of CAnalysisTool-derived classes used by aggregates to simulate
	// various measurements of aggregate properties such as a FRAP experiment.
	// Because not all tools may be active, we store the location in the
	// m_vTools vector of each active tool so that it is easily accessible
	// without knowing which preceding tools are active.

	ToolSequence		m_vTools;				// Set of tools for aggregate
	zBoolVector			m_vActiveTools;			// Flags showing which tools are used
	zLongVector			m_vActiveToolsElement;	// Element in m_vTools corresponding to each active tool

};

#endif // !defined(AFX_ANALYSIS_H__A4AEA916_9D0D_11D3_BF15_004095086186__INCLUDED_)
