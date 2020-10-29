// InputData.h: interface for the CInputData class.
//
// This class collects all the data from the Control Data File (CDF) to
// specify a simulation and validates it. It uses a local object derived 
// from xxFile to read the data, either a CInputDPDFile or CInputMDFile.
// It checks that the bead, bond, bondpair and polymer names only contain
// allowed characters and are unique; it creates the internal representation 
// of the types needed in the simulation (converting from CBeadType to CBead 
// for example) and checks that the polymer shape strings are valid.
//
// It also validates all the aggregates, commands, events and processes
// specified for the simulation by calling the appropriate IsDataValid() 
// functions for them. If a restart run is specified, it ensures that the
// restart state's CDF data  is consistent with the restarted run.
//
//
// Last modified: August 2, 2003.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTDATA_H__A2FACF46_3F61_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INPUTDATA_H__A2FACF46_3F61_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CBeadType;
class CBondType;
class CBondPairType;
class CPolymerType;

class CInitialState;
class CInitialStateData;
class CGravityData;
class CWallData;
class CBeadStructure;
class pmInputData;
class pmBeadData;
class pmBondData;
class pmBondPairData;
class pmPolymerData;
class pmCommandNames;
class mpmInitialState;


#include "xxBase.h"

class CInputData : public xxBase
{
	// ****************************************
	// Construction/Destruction
public:

	CInputData(const zString runid);
	virtual ~CInputData();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static bool GlobalIsExternalNameValid(const zString name);	 // Static version of IsExternalNameValid()

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

    // Main routine called by CSimulation to retrieve data from the input file.
    // For a parallel simulation, this is only called by P0.

	bool GetInputData(const zString iFileName);	

    // Functions used to retrieve data from the parallel message instances that
    // define a parallel simulation.

    void ReceiveInputData(const pmInputData* const pMsg); 
	void ReceiveBeadTypeData(const pmBeadData* const pMsg);
	void ReceiveBondTypeData(const pmBondData* const pMsg);
	void ReceiveBondPairTypeData(const pmBondPairData* const pMsg);
    void ReceivePolymerTypeData(const pmPolymerData* const pMsg);
	void ReceiveCommandNames(const pmCommandNames* const pMsg);
	void ReceiveISData(const mpmInitialState* const pMsg);

    // Function used to validate the input data for serial and parallel runs

    bool IsInitialStateValid();

    // Functions used by the CSimulation instance to send and receive the
    // datatypes needed for the simulation. P0 reads the CDF, creates its own 
    // types and then fills up the expected type totals and broadcasts them to 
    // the PN. It thens sends messages to create each new datatype and the PN
    // use the expected type totals to know how many messages to receive. They
    // then create their own copies of the specified datatypes.

    inline long GetExpectedBeadTypeTotal()      const {return m_ExpectedBeadTypeTotal;}
    inline long GetExpectedBondTypeTotal()      const {return m_ExpectedBondTypeTotal;}
    inline long GetExpectedBondPairTypeTotal()  const {return m_ExpectedBondPairTypeTotal;}
    inline long GetExpectedPolymerTypeTotal()   const {return m_ExpectedPolymerTypeTotal;}

    inline long GetBeadTypeTotal()      const {return vBeadTypes.size();}
    inline long GetBondTypeTotal()      const {return vBondTypes.size();}
    inline long GetBondPairTypeTotal()  const {return vBondPairTypes.size();}
    inline long GetPolymerTypeTotal()   const {return vPolymerTypes.size();}

	// ****************************************
	// Functions to return vectors of the types defined in the simulation

	inline const BeadVector&		GetBeadTypes()		const {return vBeadTypes;}
	inline const BondVector&		GetBondTypes()		const {return vBondTypes;}
	inline const BondPairVector&	GetBondPairTypes()	const {return vBondPairTypes;}
	inline const PolymerVector&		GetPolymerTypes()	const {return vPolymerTypes;}

	// Functions to return const references to the Bead, Bond and Polymer 
	// name maps to allow access of types from their names, and the
	// polymer shapes from their type.

	inline const LongStringMap&  GetBeadTypesMap()          const {return mBeadTypes;}
	inline const LongStringMap&  GetPolymerTypesMap()       const {return mPolymerTypes;}

	inline const StringLongMap&  GetBeadNamesMap()          const {return mBeadNames;}
	inline const StringLongMap&  GetBondNamesMap()          const {return mBondNames;}
	inline const StringLongMap&  GetBondPairNamesMap()      const {return mBondPairNames;}
	inline const StringLongMap&  GetPolymerNamesMap()       const {return mPolymerNames;}

	inline const LongStringMap&  GetPolymerShapesMap()      const {return m_mPolymerShapes;}
	inline const LongLongMap&    GetPolymerSizesMap()       const {return m_mPolymerSizes;}
	inline const LongDoubleMap&  GetPolymerFractionsMap()   const {return m_mPolymerFractions;}

	// Functions to return the types of the beads defining the bond and bondpair types

	inline const LongLongMap&	GetBondHeadTypesMap()		const {return m_mBondHeadTypes;}
	inline const LongLongMap&	GetBondTailTypesMap()		const {return m_mBondTailTypes;}

	inline const LongLongMap&	GetBondPairHeadTypesMap()	const {return m_mBondPairHeadTypes;}
	inline const LongLongMap&	GetBondPairMiddleTypesMap()	const {return m_mBondPairMiddleTypes;}
	inline const LongLongMap&	GetBondPairTailTypesMap()	const {return m_mBondPairTailTypes;}

	// Functions to return information about the polymer types given either
	// their integer type or name

	long GetPolymerTypeSize(long type) const;
	long GetPolymerTypeSize(const zString polyName) const;

	const zString GetPolymerTypeShape(long polyType)const;
	const zString GetPolymerTypeShape(const zString polyName) const;

	double GetPolymerTypeFraction(long polyType) const;
	double GetPolymerTypeFraction(const zString polyName) const;

	// Functions returning details of the bead structure for all simulation types.
	// We pass them by copies so that the CCNTCell class can have its own copies.

	// DPD

	inline const zArray2dDouble*  GetDPDBeadConsIntArray() const {return &m_vvConsInt;}
	inline const zArray2dDouble*  GetDPDBeadDissIntArray() const {return &m_vvDissInt;}
	inline const zArray2dDouble*  GetDPDBeadLGIntArray()   const {return &m_vvLGInt;}

	// MD

	inline const zArray2dDouble*  GetMDBeadLJDepthArray() const {return &m_vvLJDepth;}
	inline const zArray2dDouble*  GetMDBeadLJRangeArray() const {return &m_vvLJRange;}
	inline const zArray2dDouble*  GetMDBeadSCDepthArray() const {return &m_vvSCDepth;}
	inline const zArray2dDouble*  GetMDBeadSCRangeArray() const {return &m_vvSCRange;}

	// ****************************************
	// Access functions for other simulation data

    bool IsDPDLG() const;  // Flag showing if DPD density-dependent force is included

	inline const zString& GetRunId()    const {return runId;}
	inline const zString& GetTitle()    const {return Title;}
	inline const zString& GetDate()     const {return Date;}
	inline const zString& GetComment()  const {return Comment;}

	inline const CInitialStateData* GetISD() const {return m_pISD;} 

	double GetAveBeadDensity() const;
	double GetLambda()		   const;		// DPD only
	double GetCutOffRadius()   const;		// MD only
	double GetMCStepSize()	   const;		// MD only
	double GetStepSize()       const;
	double GetkT()             const;
	double GetCNTXCellWidth()  const;
	double GetCNTYCellWidth()  const;
	double GetCNTZCellWidth()  const;
	double GetSimBoxXLength()  const;
	double GetSimBoxYLength()  const;
	double GetSimBoxZLength()  const;
	double GetSimSpaceXLength() const;
	double GetSimSpaceYLength() const;
	double GetSimSpaceZLength() const;

	long   GetRNGSeed()			const;
	long   GetCurrentTime()		const;
	long   GetTotalMCTime()     const;		// MD only
	long   GetTotalTime()		const;
	long   GetProcessorsXNo()	const;      // For parallel runs only
	long   GetProcessorsYNo()	const;
	long   GetProcessorsZNo()	const;
	long   GetCNTXCellNo()		const;
	long   GetCNTYCellNo()		const;
	long   GetCNTZCellNo()		const;
	long   GetDensityPeriod()	const;
	long   GetRestartPeriod()	const;
	long   GetDisplayPeriod()	const;
	long   GetAnalysisPeriod()	const;
	long   GetSamplePeriod()	const;
	inline long  GetGridXCellNo() const {return GridXCellNo;}
	inline long  GetGridYCellNo() const {return GridYCellNo;}
	inline long  GetGridZCellNo() const {return GridZCellNo;}

	// Body force data

	const inline StringSequence GetGravityBeadNames() const {return vGravityBeadNames;}
	const zLongVector GetGravityBeadTypes() const;
	inline double GetGravityXForce() const {return GravityXForce;}
	inline double GetGravityYForce() const {return GravityYForce;}
	inline double GetGravityZForce() const {return GravityZForce;}

	// Wall data

	const inline zString GetWallPolymerName() const {return WallPolymerName;}
	inline double GetWallBeadDensity() const {return WallBeadDensity;}
	inline long GetWallXWidth() const {return WallXWidth;}
	inline long GetWallYWidth() const {return WallYWidth;}
	inline long GetWallZWidth() const {return WallZWidth;}

	// ****************************************
	// Functions to return the set of processes, aggregates, events
	// or commands that the user has defined in the CDF; and the
	// set of names of force targets and command targets that are 
	// created as the result of commands during the simulation. The
	// names are stored here so that the target data can be validated
	// using only the CInputData object.

	const ProcessSequence&		GetProcesses()			const;
	const AggregateSequence&	GetAggregates()			const;
	const EventSequence&		GetEvents()				const;
	const CommandSequence&		GetCommands()			const;
	const StringSequence		GetForceTargetNames()	const;
	const StringSequence		GetCommandTargetNames() const;
	const StringSequence        GetActiveCommandTargetNames() const;
	const StringSequence        GetCommandGroupNames()  const;
    const StringSequence        GetCommandNames()       const;

	// Functions to store locally the names of targets created as the 
	// result of commands to ensure that they are unique when validating 
	// the command data.

	void AddForceTargetName(const zString name);
	void AddCommandTargetName(const zString name);
	void AddActiveCommandTargetName(const zString name);
	void AddCommandGroupName(const zString name);

	bool AddBeadNameToMap(const zString name);

	// ****************************************
	// Functions to show whether constraints are turned on

	inline bool IsGravityPresent()  const {return m_bGravity;}
	inline bool IsWallPresent()		const {return m_bWall;}
	inline bool IsShearPresent()	const {return m_bShear;}

	// ****************************************
	// Functions to wrap private functions that are useful outside this class.
	// and functions to validate data that is stored in this class

	bool IsExternalNameValid(const zString name) const;
	bool IsBeadinMap(const zString name) const;
	bool IsBondinMap(const zString name) const;
	bool IsPolymerinMap(const zString name) const;
	bool IsBondPairinMap(const zString name) const;

	bool IsForceTargetPresent(const zString label) const;
	bool IsCommandTargetPresent(const zString label) const;
	bool IsActiveCommandTargetPresent(const zString label) const;
	bool IsCommandGroupPresent(const zString label) const;


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	bool ReadControlDataFile(const zString iFileName);
	bool WriteDefaultControlDataFile(const zString iFileName);

    void SetDPDLG(bool bLG);
	void SetTitle(zString title);
	void SetDate(zString date);
	void SetComment(zString comment);

    void SetExpectedBeadTypeTotal(long total);
    void SetExpectedBondTypeTotal(long total);
    void SetExpectedBondPairTypeTotal(long total);
    void SetExpectedPolymerTypeTotal(long total);
	void SetProcessorsXNo(long xproc);
	void SetProcessorsYNo(long yproc);
	void SetProcessorsZNo(long zproc);
	void SetCNTXCellNo(long xcells);
	void SetCNTYCellNo(long ycells);
	void SetCNTZCellNo(long zcells);
	void SetCNTXCellWidth(double xcellwidth);
	void SetCNTYCellWidth(double ycellwidth);
	void SetCNTZCellWidth(double zcellwidth);

	void SetAveBeadDensity(double dens);
	void SetTemp(double temp);
	void SetRNGSeed(long seed);
	void SetLambda(double lambda);					// DPD only
	void SetCutOffRadius(double rcutoff);			// MD only
	void SetMCStepSize(double step);
	void SetTotalMCTime(long steps);
	void SetStepSize(double step);
	void SetTotalTime(long steps);
	void SetSamplePeriod(long period);
	void SetAnalysisPeriod(long period);
	void SetDisplayPeriod(long period);
	void SetDensityPeriod(long period);
	void SetRestartPeriod(long period);
	void SetGridZCellNo(long zcells);
	void SetGridYCellNo(long ycells);
	void SetGridXCellNo(long xcells);

	void SetISD(const CInitialStateData* pISD);
	void SetWallData(const CWallData* pWD);
	void SetGravityData(const CGravityData* pGD);
	void SetShearData();

	// Convert data into dimensionless quantities for MD simulations
	void NormalizeMD(LongBeadTypeMap      mNewBeadTypes,
					 LongBondTypeMap      mNewBondTypes,
					 LongBondPairTypeMap  mNewBondPairTypes,
					 LongPolymerTypeMap   mNewPolymerTypes);

	// Create the bead, bond, bondpair and polymer templates

	bool CreateNewTypes(LongBeadTypeMap      mNewBeadTypes,
						LongBondTypeMap      mNewBondTypes,
						LongBondPairTypeMap  mNewBondPairTypes,
						LongPolymerTypeMap   mNewPolymerTypes);

	// Functions to parse the polymer shape string, validate the tokens 
	// and create the appropriate pointer structures to hold beads, bonds 
	// and bondpairs

	bool AddPolymerType(zString name, zString shape, double fraction, zString head, zString tail);
	bool AddPolymerType(zString name, zString shape, double fraction);
	bool AddBondPairType(zString name1, zString name2, zString name3, double Strength, double Phi0);
	bool AddBondType(zString name1, zString name2, double spring, double length0);
	bool AddBeadType(zString name, const CBeadStructure* pStructure);

	zString AddBeadtoPolymer(BeadVector& vNewBeads, BondVector& vNewBonds, 
								 zString current_beadname, zString next_beadname, 
								 long* pCurrentBeadIndex, bool bIncrementBeadIndex);

	zString ParseElement(bool& bLinear, BeadVector& vNewBeads, BondVector& vNewBonds, 
							 zString current_beadname, zString element, 
							 long* pCurrentBeadIndex, bool bIncrementBeadIndex);

	zString ExpandElement(zString op, zString token);
	zString RemoveBrackets(zString oldString) const;
	zString RemoveSpaces(zString oldString) const;
	zString CDR(const zString oldString) const;
	zString CAR(const zString oldString) const;
	zString FIRST(const zString shape) const;
	zString LAST(const zString shape) const;

	bool AreAllBeadNamesValid(const zString shape) const;
	bool IsNameValid(const zString name) const;
	bool IsShapeValid(const zString shape) const;
	bool IsBeadUniqueInPolymer(const zString bead, const zString shape) const;
	bool IsElementValid(zString element) const;

	// Functions to check that constraints on the SimBox are valid
	bool IsGravityValid() const;
	bool IsGravityBeadValid() const;

	bool IsWallValid() const;
	bool IsWallPolymerinMap() const;
	bool IsWallBeadDensityValid() const;
	bool IsWallThicknessValid() const;

	bool IsRestartDataValid(LongBeadTypeMap    mNewBeadTypes,
							LongBondTypeMap    mNewBondTypes,
							LongPolymerTypeMap mNewPolymerTypes) const;


	// ****************************************
	// Functions to send the ValidateData message to all xxProcessObject,
	// CAnalysisData and xxEventObjects, and the IsDataValid message to xxCommand objects

	bool  AreProcessesValid(ProcessObjectSequence   vProcessObjects);
	bool  AreAggregatesValid(AnalysisObjectSequence vAnalysisObjects);
	bool  AreEventsValid(EventObjectSequence        vEventObjects);
	bool  AreCommandsValid(CommandObjectSequence    vCommandObjects);		
	bool  AreParallelProcessorsValid() const;

														
	// ****************************************
	// Data members
private:

	// Strings used by the parser to verify the validity of bead, bond and 
	// polymer names, and the names of targets specified in commands.

	static const zString spaceChar;
	static const zString branchChar;
	static const zString loopChar;
	static const zString validChars;
	static const zString validNameChars; 
	static const zString validNumOpChars;
	static const zString validSeparatorOpChars;

	// Identifying number for the run that cannot be changed during the 
	// lifetime of the CInputData object.

	const zString runId;

	// Object needed to assemble the initial bead configuration in the simulation

	CInitialStateData* m_pISD;
								
	// ****************************************
	// User input data read from the CDF: this includes both DPD and MD data 

	zString Title;							// Information strings for user
	zString Date;
	zString Comment;

	double AverageBeadDensity;
	double Lambda;							// DPD only
	double RCutOff;							// MD only
	double MCStepSize;						// MD only
	double kT;
	double StepSize;
	double CNTXCellWidth;					// Width of cells in units of bead radii
	double CNTYCellWidth;
	double CNTZCellWidth;

	long AnalysisPeriod;					// Periods at which to write various data
	long DensityPeriod;
	long DisplayPeriod;
	long RestartPeriod;
	long SamplePeriod;
	long GridXCellNo;						// Global analysis grid size
	long GridYCellNo;
	long GridZCellNo;

    long ProcXNo;                           // Processors per dimension for parallel run
    long ProcYNo;
    long ProcZNo;
	long CNTXCellNo;						// SimBox size in terms of CNT cells
	long CNTYCellNo;
	long CNTZCellNo;
	long CurrentTime;
	long TotalMCTime;						// MD only
	long TotalTime;
	long RNGSeed;

	StringSequence vGravityBeadNames;		// Body force data derived from CGravityData object
	double GravityXForce;
	double GravityYForce;
	double GravityZForce;

	zString WallPolymerName;				// Wall data derived from CWallData object
	double WallBeadDensity;
	long WallXWidth;
	long WallYWidth;
	long WallZWidth;

	ProcessSequence   m_vProcesses;				// Vector of processes to simulate
	AggregateSequence m_vAggregates;			// Vector of aggregates to search for 
	EventSequence	  m_vEvents;				// Vector of events to check 
	CommandSequence   m_lCommands;				// List of user-defined commands
	StringSequence    m_vForceTargetNames;		// Force target names from Selection commands
	StringSequence    m_vCommandTargetNames;	// Command target names from SelectBeadTypeInRegion commands
	StringSequence    m_vACTNames;				// Active Command target names
    StringSequence    m_vCommandGroupNames;     // Command group names
    StringSequence    m_vCommandNames;          // Names of scheduled commands

	// Types of bead, bond and polymer used to construct the CBead, etc, 
	// representations needed to copy-construct the many copies of beads, 
	// bonds and polymers used in the actual simulation.

	StringLongMap		mBeadNames;
	StringLongMap		mBondNames;
	StringLongMap		mBondPairNames;
	StringLongMap		mPolymerNames;

	// Maps giving access to polymer structure details.

	LongStringMap		m_mPolymerShapes;     // Map of <polymer type, shape string>
	LongLongMap         m_mPolymerSizes;      // Map of <polymer type, size>
	LongDoubleMap       m_mPolymerFractions;  // Map of <polymer type, number fraction>

	// Maps from bond and bondpair types to their head and tail bead types

	LongLongMap			m_mBondHeadTypes;
	LongLongMap			m_mBondTailTypes;

	LongLongMap			m_mBondPairHeadTypes;
	LongLongMap			m_mBondPairMiddleTypes;
	LongLongMap			m_mBondPairTailTypes;

	// Map from numeric types to their (zString) names that is used when creating 
	// CBondPairs for polymers and passing type data to PN in a parallel run.

	LongStringMap		mBeadTypes;
	LongStringMap		mPolymerTypes;

	// Bead-bead interactions, dissipations and bond parameters cannot be declared
	// until we know how many bead and bond types there are. We create empty
	// vectors holding a representative of each type of bead, bond, bondpair and 
	// polymer allow them to grow when the number of bead types, etc is known.
	// Note that we use vectors of pointers not objects to minimise calls to
	// constructors and copy constructors.

	BeadVector     vBeadTypes;
	BondVector     vBondTypes;
	BondPairVector vBondPairTypes;
	PolymerVector  vPolymerTypes;

	// Bead structure data includes the interaction parameters for DPD and MD
	// simulations. They are stored as a vector of vectors.
	// These are never both used in the same simulation, it being prevented by 
	// the functions CreateNewTypes() and AddBeadType().

	zArray2dDouble m_vvConsInt;	// DPD
	zArray2dDouble m_vvDissInt;
	zArray2dDouble m_vvLGInt;   // Liquid-Gas force parameters

	zArray2dDouble m_vvLJDepth;	// MD
	zArray2dDouble m_vvLJRange;
	zArray2dDouble m_vvSCDepth;
	zArray2dDouble m_vvSCRange;

	// Local implementation variables

    bool m_bLG;                 // Flag showing if density-dependent LG force is included
	bool m_bGravity;			// External body force defined flag
	bool m_bWall;				// Wall defined flag
	bool m_bShear;				// Shear defined flag

	bool m_bBondTypeMissing;	// Flag used in AddPolymer() to indicate missing bonds

    // Expected number of types of bead, bond, bondpair and polymer that are
    // required by PN when running a parallel simulation

    long m_ExpectedBeadTypeTotal;
    long m_ExpectedBondTypeTotal;
    long m_ExpectedBondPairTypeTotal;
    long m_ExpectedPolymerTypeTotal;


    BeadTypeVector m_vBeadTypesPN; // Bead type instances created on PN in a parallel run

	// ********************
	// Internal class to hold data needed to define loops in the polymer 
	// shape strings. It cannot be declared private because the typedef then
	// fails in the InputData.cpp file. But it is not meant to be accessed from 
	// outside this class.

public:

	class LoopTarget
	{
	public:
		LoopTarget(long id, zString name, CAbstractBead* pBead);
		~LoopTarget();

		long GetId() const;
		zString GetName() const;
		CAbstractBead* GetBead() const;

	private:
		long m_id;
		zString m_Name;
		CAbstractBead* m_pBead;
	};

	// ********************

private:

	// Map of loop targets using their id as the key and the LoopTarget objects
	// as the values. This class is internal to CInputData so cannot be accessed
	// from outside.

	std::map<long, LoopTarget*> m_mLoopTargets;

};

#endif // !defined(AFX_INPUTDATA_H__A2FACF46_3F61_11D3_820E_0060088AD300__INCLUDED_)
