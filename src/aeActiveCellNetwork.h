// aeActiveCellNetwork.h: interface for the aeActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEACTIVECELLNETWORK_H__429F97D4_9084_4B22_ABED_CA954B4C430C__INCLUDED_)
#define AFX_AEACTIVECELLNETWORK_H__429F97D4_9084_4B22_ABED_CA954B4C430C__INCLUDED_



// Forward declarations

class IACNAccessControl;
class aevActiveEvent;
class CDensityField1d;
class xxProcess;
class taEventSourceDecorator;


// Include header files used by derived classes

#include "acfACNFactory.h"	// Needed to create aeActiveCellNetworks
#include "IActiveSimBox.h"


#include "xxBase.h"

class aeActiveCellNetwork : public xxBase
{
	// Active events need to modify the bonds and polymers stored in the ACN's

	friend class aevActiveEvent;

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~aeActiveCellNetwork();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual void Evolve() = 0;

	// Function to connect the ACN to an associated process that can
	// serialise its data to file. It is not yet a PVF as not all ANCs
	// implement it.

	virtual void AttachProcess(xxProcess* pProcess);

	// Non-static member function to return the (static) ACN name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided in each derived class.

	virtual const zString GetACNType() const = 0;


	// ****************************************
	// Public access functions
public:

	// Inter-ACN access control functions

	bool IsAccessControlEnabled() const;

	inline IACNAccessControl* GetIACNAccessControl() const {return m_pIACNAccess;}


	// Function to allow other active classes to access the IActiveSimBox interface
	// and forwarding functions to the active SimBox

	IActiveSimBox* GetIActiveSimBox();

	inline const zString GetRunId()  const {return m_pShadow->GetRunId();}
	inline long   GetCurrentTime()	 const {return m_pShadow->GetCurrentTime();}
	inline double GetSimBoxXLength() const {return m_pShadow->GetSimBoxXLength();}
	inline double GetSimBoxYLength() const {return m_pShadow->GetSimBoxYLength();}
	inline double GetSimBoxZLength() const {return m_pShadow->GetSimBoxZLength();}

	// Active SimBox CNT cell functions

	inline long GetXCellNo()	  const {return m_CNTXCellNo;}
	inline long GetYCellNo()	  const {return m_CNTYCellNo;}
	inline long GetZCellNo()	  const {return m_CNTZCellNo;}
	inline long GetXYCellNo()	  const {return m_CNTXYCellTotal;}
	inline long GetCellTotal()	  const {return m_CNTCellTotal;}

	inline double GetXCellWidth() const {return m_CNTXCellWidth;}
	inline double GetYCellWidth() const {return m_CNTYCellWidth;}
	inline double GetZCellWidth() const {return m_CNTZCellWidth;}

	// Functions to obtain the cell containing a specified active bond.

	aeCNTCell* GetCellFromBond(const aeActiveBond* const pBond) const;
	aeCNTCell* GetCellFromIndex(double x, double y, double z) const;

	inline bool   GetFreeBondDisplayStatus()					const {return m_bFreeBondVisible;}
	inline bool   GetFreePolymerDisplayStatus()					const {return m_bFreePolymerVisible;}

	inline	bool  GetFreeBondDisplayUpdateStatus()				const {return m_bUpdateFreeBondDisplay;}
	inline	bool  GetFreePolymerDisplayUpdateStatus()			const {return m_bUpdateFreePolymerDisplay;}

	// Analysis functions independent of filament type

	inline long	GetFreeBondTotal()		       const {return m_FreeActiveBonds.size();}
	inline long	GetPhantomBondTotal()		   const {return m_FreePhantomBonds.size();}
	inline long	GetFreePolymerTotal()	       const {return m_FreeActivePolymers.size();}
	inline long	GetFilamentTotal()		       const {return GetFreePolymerTotal();}

	inline ActiveBondList    GetFreeBonds()    const {return m_FreeActiveBonds;}
	inline ActiveBondList    GetPhantomBonds() const {return m_FreePhantomBonds;}
	inline ActivePolymerList GetFreePolymers() const {return m_FreeActivePolymers;}

	long		GetBoundBondTotal()		const;
	long		GetMonomerTotal()       const;

	double		GetFreeBondFraction() const;
	double		GetBoundBondFraction() const;

	// The following functions calculate the average properties of the free 
	// bonds and filaments.

	double		CalculateFilamentProperties();
	void        CalculateFilamentInCylindricalPotentialProperties();
	void		CalculateDensityFields();
	void        CalculateEventCounters();

	// The following functions require that the analysis option 
	// CalculateFilamentProperties(), and GetMeanFilamentSize() be called first

	inline double GetMeanFilamentSize()     const {return m_MeanFilamentSize;}
	inline double GetMeanSqFilamentSize()   const {return m_MeanSqFilamentSize;}
	inline double GetSDevFilamentSize()     const {return m_SDevFilamentSize;}

	inline double GetMeanFilamentLength()   const {return m_MeanFilamentLength;}
	inline double GetMeanSqFilamentLength() const {return m_MeanSqFilamentLength;}
	inline double GetSDevFilamentLength()   const {return m_SDevFilamentLength;}

	// The following functions require that the analysis option
	// CalculateFilamentInCylindricalPotentialProperties() be called first.
	// Also, the first two functions calculate the mean square lateral displacement
	// assuming the filaments are aligned along the Z axis.

	inline double GetMeanLatSqDisp()             const {return m_MeanFilamentLatSqDisp;}
	inline double GetVarLatSqDisp()              const {return m_VarFilamentLatSqDisp;}

	inline double GetMeanCM(short int i)         const {return m_MeanFilamentCM[i];}
	inline double GetMeanSqDisp(short int i)     const {return m_MeanFilamentSqDisp[i];}
	inline double GetMeanHeadCoord(short int i)  const {return m_MeanFilamentHead[i];}
	inline double GetMeanTailCoord(short int i)  const {return m_MeanFilamentTail[i];}

	inline double GetVarSqDisp(short int i)      const {return m_VarFilamentSqDisp[i];}
	inline double GetVarHeadCoord(short int i)   const {return m_VarFilamentHead[i];}
	inline double GetVarTailCoord(short int i)   const {return m_VarFilamentTail[i];}

    // Event success/failure counter functions

    inline bool IsCumulativeEventStatisticsOn()  const {return m_bCumulativeEventStatistics;}

	long GetEventSuccessCounter(long i) const;
	long GetEventFailureCounter(long i) const;

	// Functions used by the shadow SimBox to track active polymers

	void AssignActiveBondsToCells(ActiveBondList lFreeActiveBonds);

	// Functions to access ACN's active events so that they can be 
	// modified by command: note that we use "Internal" to distinguish
	// them from the IModifyActiveCellNetwork interface functions

	inline long GetEventTotal()                 const {return m_Events.size();}
	inline long GetPolymerFormsEventTotal()     const {return m_PolymerFormsTotal;}
	inline long GetPolymerDissolvesEventTotal() const {return m_PolymerDissolvesTotal;}

	// Functions to access the restrictions that may be placed on the ACN's
	// events by command

	inline bool GetRestrictMaxPolymers() const {return m_bRestrictPolymers;}
	inline long GetMaxPolymers() const {return m_MaxPolymers;}

	void DecrementPolymerFormsEventTotal();

    // Functions to access the phantom bonds

    inline bool GetPhantomBondsAllowed()    const {return m_bPhantomBonds;}
    inline long GetConstantFreeBondNumber() const {return m_ConstantFreeBondNumber;}
    inline long GetPhantomBondsPerEvent()   const {return m_PhantomBondsPerEvent;}

    // Function used by derived ACN classes that want to keep 
    // the number of free bonds constant as their polymers grow and shrink.

	void ConvertFreeBondsToPhantom();
    void ConvertPhantomBondsToFree();


    // Function to return an event given its name.
	aevActiveEvent* GetInternalEventFromType(const zString eventType);

	const zString GetInternalPolymerFormsEventType() const;
	const zString GetInternalPolymerDissolvesEventType() const;
	const zString GetInternalBondBindsToPolymerHeadEventType() const;
	const zString GetInternalBondBindsToPolymerTailEventType() const;
	const zString GetInternalBondUnbindsFromPolymerHeadEventType() const;
	const zString GetInternalBondUnbindsFromPolymerTailEventType() const;

	aevActiveEvent* GetInternalPolymerFormsEvent();
	aevActiveEvent* GetInternalPolymerDissolvesEvent();
	aevActiveEvent* GetInternalBondBindsToPolymerHeadEvent();
	aevActiveEvent* GetInternalBondBindsToPolymerTailEvent();
	aevActiveEvent* GetInternalBondUnbindsFromPolymerHeadEvent();
	aevActiveEvent* GetInternalBondUnbindsFromPolymerTailEvent();

    // Function to destroy an ATP-dependent event that wraps one of the
    // ACN's bonds whose id is supplied. If the bond is not found, the
    // function returns false; if the event is destroyed it returns true.

    bool DeleteATPDependentEventWrappingBond(long bondId);

    // Functions to set and get the execution period for a named event type.
    // A private helper function that updates stored events' periods is declared
    // below.

	void SetInternalEventExecutionPeriod(const zString eventType, long period);
    long GetInternalEventExecutionPeriodFromName(const zString eventType) const;

    // Functions to enable/disable the broadcasting of event states for analysis.

    bool AddEventSource(const zString eventType, taEventSourceDecorator* const pSource);
	bool RemoveEventSource(const zString eventType, taEventSourceDecorator* const pSource);

    void BroadcastEventState(const aevActiveEvent* const pEvent);


	// ****************************************
	// Protected local functions
protected:

	// Protected constructor prevents this class from being instantiated

	aeActiveCellNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn, double xw, double yw, double zw);

	// Functions to construct cells and map their integer indices to cell pointers

	void MakeCNTCells();

	long MapCelltoIndex(long i, long j, long k, long alpha, long beta, long gamma) const;
	long MapCelltoNNIndex(long alpha, long beta, long gamma) const;

	bool CheckMonomers();

	// Functions to allow ACNs to toggle the display status of their bonds and polymers

	inline void   SetFreeBondDisplayStatus(bool bVisible)			  {m_bFreeBondVisible    = bVisible;}
	inline void   SetFreePolymerDisplayStatus(bool bVisible)		  {m_bFreePolymerVisible = bVisible;}
	void          SetFreePolymerDisplayByMinLength(long minSize);

	inline void	  SetFreeBondDisplayUpdateStatus(bool bVisible)		  {m_bUpdateFreeBondDisplay    = bVisible;}
	inline void	  SetFreePolymerDisplayUpdateStatus(bool bVisible)    {m_bUpdateFreePolymerDisplay = bVisible;}

	void UpdateFreeBondDisplayStatus();
	void UpdateFreePolymerDisplayStatus();

	// Command handler function to limit the number of polymers that can form

	void SetMaxPolymers(long max);

    // Command handler functions to implement and cancel a constant concentration 
    // of free bonds

    void SetConstantFreeBonds(double constantFraction, long phantomsPerEvent);
    void CancelConstantFreeBonds();

	// Analysis functions executed by command

    bool ToggleCumulativeEventStatisticsHandler();
    void ResetACNEventCounters();
	void ResetAllEventCounters();

	inline bool	IsDensityFieldAnalysisOn() const {return !m_DensityFields.empty();}
	inline long GetDensityFieldTotal()	   const {return m_DensityFields.size();}

	void Add1dDensityField(long start, long end, long sample, long analysis,
						   long nx, long ny, long nz, long sliceTotal,
						   double lowerCoord, double upperCoord);

	void Add2dDensityField();
	void Add3dDensityField();

	// Functions to manipulate the free bond container

	void UpdateActiveBondsInCells();
	void AddFreeBond(aeActiveBond* const pBond);
	void RemoveFreeBond(aeActiveBond* const pBond);
	ActiveBondListIterator RemoveFreeBondByIterator(ActiveBondListIterator iterBond);

	void AddFreePolymer(aeActivePolymer* const pPolymer);
	void RemoveFreePolymer(aeActivePolymer* const pPolymer);

    // Functions used to manipulate the phantom bonds

	void SetAllFreeBondsPhantom();
    void SetAllPhantomBondsActive();


	// ****************************************
	// Functions to manipulate the events local to this ACN

	aevActiveEvent*  AddEvent(const zString type);
	void RemoveEvent(aevActiveEvent* pEvent);

	void DeleteEvent(aevActiveEvent* pEvent);
	void UpdateEvents();

	aevActiveEvent* GetEvent(long i);

	// ****************************************
	// Implementation

protected:

	void SetInternalPolymerFormsEvent(aevActiveEvent* const pEvent);
	void SetInternalPolymerDissolvesEvent(aevActiveEvent* const pEvent);
	void SetInternalBondBindsToPolymerHeadEvent(aevActiveEvent* const pEvent);
	void SetInternalBondBindsToPolymerTailEvent(aevActiveEvent* const pEvent);
	void SetInternalBondUnbindsFromPolymerHeadEvent(aevActiveEvent* const pEvent);
	void SetInternalBondUnbindsFromPolymerTailEvent(aevActiveEvent* const pEvent);


	// ****************************************
	// Private functions
private:

    // Helper function used by the commands that change event execution periods
    // to update all stored events, and prototype event instances, in the ACN.

	void UpdateEventPeriodByType(const zString eventType);

	// ****************************************
	// Data members

	// Data accessible to derived classes
protected:

	IACNAccessControl*  m_pIACNAccess;		// Interface pointer to access control class

	ActiveBondList	   m_FreeActiveBonds;		// List of free active bonds (those not in active polymers)
       ActiveBondList     m_FreePhantomBonds;         // List of free phantom bonds (these are also in the free active bond list)
       ActivePolymerList  m_FreeActivePolymers;	// List of active polymers

	DensityFieldSequence	m_DensityFields;	// Set of density fields being calculated


	// Local count of the number of each type event for the ACN

	long    m_PolymerFormsTotal;		    // Current number of aevPolymerForms events
	long    m_PolymerDissolvesTotal;		// Current number of aevPolymerDissolves events

	// Data local to this base class
private:

	IActiveSimBox* const m_pShadow;			// Interface pointer to the active SimBox


        StringLongMap m_mEventExecutionPeriodFromName;  // Maps of event (period, name) pairs
        StringEventSourceMMap  m_mEventSourceFromType;  // Multimap of (event type, event source decorator) pairs

        ActiveEventList m_Events;				// Active events owned by this network

	aevActiveEvent* m_pPolymerFormsEvent;				// Event managing polymer formation
	aevActiveEvent* m_pPolymerDissolvesEvent;			// Event managing polymer breakup
	aevActiveEvent* m_pBondBindsToPolymerHeadEvent;		// Event managing polyer growth at the head
	aevActiveEvent* m_pBondBindsToPolymerTailEvent;		// Event managing polyer growth at the tail
	aevActiveEvent* m_pBondUnbindsFromPolymerHeadEvent;	// Event managing polyer loss at the head
	aevActiveEvent* m_pBondUnbindsFromPolymerTailEvent;	// Event managing polyer loss at the tail

	// More events here...


	aeCNTCellVector	m_vCNTCells;	// Container of CNT cells that hold bond pointers

	const long	m_CNTXCellNo;		// Number of cells in each dimension
	const long	m_CNTYCellNo;
	const long	m_CNTZCellNo;
	const long  m_CNTXYCellTotal;	// Number of cells in an XY plane of the SimBox

	long m_CNTCellTotal;			// Total number of cells in network

	const double m_CNTXCellWidth;	// Width of cells in each dimension
	const double m_CNTYCellWidth;
	const double m_CNTZCellWidth;

	bool	m_bUpdateFreeBondDisplay;		// Check active bond visibility status
	bool	m_bUpdateFreePolymerDisplay;	// Check active polymer visibility status

	bool	m_bFreeBondVisible;				// Active bonds visible in current state
	bool	m_bFreePolymerVisible;			// Active polymers visible in current state
	bool	m_bFreePolymerVisibleMinLength;	// Active polymers visible by minimum length

	long    m_FreePolymerDisplayMinLength;  // Minimum polymer size for display

	bool    m_bRestrictPolymers;			// Implement maximum polymer restriction
	long    m_MaxPolymers;					// Max number of polymers allowed

    bool    m_bPhantomBonds;                // Implement constant free bond concentration
    long    m_ConstantFreeBondNumber;       // Number of active free bonds to be kept constant
    long    m_PhantomBondsPerEvent;         // Number of bonds made phantom/active per event

	// Analysis data common to all types of filament

	double	m_MeanFilamentSize;		// <No of monomers in filament>
	double	m_MeanSqFilamentSize;	// <No of monomers in filament**2>
	double	m_SDevFilamentSize;		// Std Dev of size

	double	m_MeanFilamentLength;	// <End-to-end separation of filament>
	double	m_MeanSqFilamentLength;	// <End-to-end separation of filament**2>
	double	m_SDevFilamentLength;	// Std Dev of end-to-end separation
	

	// Analysis data relating to filaments in a cylindrical potential

	double  m_MeanFilamentLatSqDisp;	// Mean square lateral displacement of monomers in filaments
    double  m_VarFilamentLatSqDisp;		// Variance of square lateral displacement of monomers

	double  m_MeanFilamentCM[3];        // CM coordinates of all filaments
	double  m_MeanFilamentSqDisp[3];    // Mean square displacement of monomers in filaments
	double  m_MeanFilamentHead[3];		// Mean coordinates of head monomers
	double  m_MeanFilamentTail[3];		// Mean coordinates of tail monomers

	double  m_VarFilamentSqDisp[3];     // Variance of square displacement of monomers in filaments
	double  m_VarFilamentHead[3];		// Variance of head monomer coordinates
	double  m_VarFilamentTail[3];		// Variance of head monomer coordinates


	// Analysis data relating to the density fields



    // Analysis of event counters 

    bool m_bCumulativeEventStatistics;  // Flag showing if event stats are cumulative (true=default) or time series (false)

    zLongVector m_EventSuccessCounters;
    zLongVector m_EventFailureCounters;




};

#endif // !defined(AFX_AEACTIVECELLNETWORK_H__429F97D4_9084_4B22_ABED_CA954B4C430C__INCLUDED_)
