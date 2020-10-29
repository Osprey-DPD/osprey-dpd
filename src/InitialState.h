// InitialState.h: interface for the CInitialState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATE_H__83457700_3C59_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INITIALSTATE_H__83457700_3C59_11D3_820E_0060088AD300__INCLUDED_



// Forward declarations

class CSimState;
class CInputData;
class CInitialStateData;
class CLamellaBuilder;
class CCompositeLamellaBuilder;
class IInclusiveRestartState;


#include "SimMPSFlags.h"

#if EnableParallelSimBox == SimMPSEnabled
// Forward declarations and parallel include files
#include "mpaInitialState.h"

#endif

#include "xxState.h"
#include "ISerialiseInclusiveRestartState.h"

class CInitialState : public xxState, public ISerialiseInclusiveRestartState
{
#if EnableParallelSimBox == SimMPSEnabled
    // Friend class to help construct the parallel initial state.
    friend class mpaInitialState;
#endif

	// ****************************************
	// Construction/Destruction
public:

	CInitialState(CSimState* pSimState, const CInputData& rData);
	virtual ~CInitialState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	bool Serialize();

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	// ****************************************
	// Public access functions
public:

	bool Assemble();  // Function to create the initial configuration for a serial simulation

	bool AssembleP(); // Function to create the parallel initial configuration

	// Functions used to add new types created during a simulation

	void AddBeadType(long oldType);
	void AddBondType(CBond* const pBond);
	void AddBondPairType(CBondPair* const pBondPair);
	void AddPolymerType(CPolymer* const pPolymer);

	// Functions to add polymerised bonds created by the builders, and as 
	// the result of commands, to the vAllPolymerisedBonds vector. 
	// The second function concatenates the polymerised bonds to the 
	// vAllBonds vector so that their forces are added to the equations of motion.

	void inline SetPolymerisedState(bool bPolymerise) {m_bIsPolymerised = bPolymerise;}

	void AddPolymerisedBond(CBond* pBond);
	void AddPolymerisedBondtoAllBonds(CBond* pBond);

	// Functions used to recalculate the numbers of each type of object

	void CalculateBeadTypeTotals();
	void CalculateBondTypeTotals();
	void CalculateBondPairTypeTotals();

    // Helper functions used by commands to modify bead states

    void SetBeadTypeInvisible(long type);
	void SetBeadTypeVisible(long type);


	// General access functions to the initial state data

	const zString	GetInitialStateType() const;

    bool            IsDPDLG() const;

	inline long		GetRNGSeed()		  const {return RNGSeed;}
	inline long		GetProcessorsXNo()	  const {return ProcessorsXNo;}
	inline long		GetProcessorsYNo()	  const {return ProcessorsYNo;}
	inline long		GetProcessorsZNo()	  const {return ProcessorsZNo;}
	inline long		GetCNTXCellNo()		  const {return CNTXCellNo;}
	inline long		GetCNTYCellNo()		  const {return CNTYCellNo;}
	inline long		GetCNTZCellNo()		  const {return CNTZCellNo;}
    inline long     GetCNTCellTotal()     const {return CNTXCellNo*CNTYCellNo*CNTZCellNo;}
	inline double	GetCNTXCellWidth()	  const {return CNTXCellWidth;}
	inline double	GetCNTYCellWidth()    const {return CNTYCellWidth;}
	inline double	GetCNTZCellWidth()    const {return CNTZCellWidth;}
	inline double	GetDensity()		  const {return AveBeadDensity;}
	inline double	GetkT()				  const {return kT;}

	double	GetEnergyScale()		const;	// Depth/range of first LJ potential
	double	GetLengthScale()		const;	// or DPD equivalent
	double  GetSimBoxXOrigin()      const;
	double  GetSimBoxYOrigin()      const;  // Coordinates of SimBox origin in simulation Space
	double  GetSimBoxZOrigin()      const;
	double	GetSimBoxXLength()		const;	// Whole SimBox side length
	double	GetSimBoxYLength()		const;
	double	GetSimBoxZLength()		const;
	double	GetSimBoxBulkXLength()	const;	// SimBox excluding walls
	double	GetSimBoxBulkYLength()	const;
	double	GetSimBoxBulkZLength()	const;

	inline double GetGravityXForce() const {return GravityXForce;}	// Body force on beads
	inline double GetGravityYForce() const {return GravityYForce;}
	inline double GetGravityZForce() const {return GravityZForce;}

	inline long GetWallXCellNo()	const {return WallXWidth;}	// No of CNT cells in wall thickness
	inline long GetWallYCellNo()	const {return WallYWidth;}
	inline long GetWallZCellNo()	const {return WallZWidth;}

	double GetWallXWidth() const;		// Wall thickness
	double GetWallYWidth() const;
	double GetWallZWidth() const;

	// return a const reference to the vectors holding the beads, bonds or polymers
	// that are present in the initial state. These functions should NOT be used
	// during a parallel run once the simulation has started as beads will move
	// between processors and these containers are not updated to reflect this
	// movement. However, all pointers in these containers are valid as we do not
	// destroy bead instances when they move between processors.

	inline const BeadVector&     GetBeads()				const {return vAllBeads;}
	inline const BondVector&     GetBonds()				const {return vAllBonds;}
	inline const BondPairVector& GetBondPairs()			const {return vAllBondPairs;}
	inline const PolymerVector&  GetPolymers()			const {return vAllPolymers;}
	inline const BeadVector&     GetGravityBeads()		const {return vGravityBeads;}
	inline const AbstractBeadVector& GetWallBeads()		const {return vWallBeads;}
	inline const BondVector&     GetPolymerisedBonds()	const {return vAllPolymerisedBonds;}
	inline const PolymerVector&  GetWallPolymers()		const {return vWallPolymers;}

	// return the total numbers of each type of object created on each processor

	inline long GetBeadTotal()				const {return vAllBeads.size();}
	inline long GetBondTotal()				const {return vAllBonds.size();}
	inline long GetBondPairTotal()			const {return vAllBondPairs.size();}
	inline long GetPolymerTotal()			const {return vAllPolymers.size();}
	inline long GetGravityBeadTotal()		const {return vGravityBeads.size();}
	inline long GetWallBeadTotal()			const {return vWallBeads.size();}
	inline long GetPolymerisedBondTotal()	const {return vAllPolymerisedBonds.size();}
	inline long GetWallPolymerTotal()		const {return vWallPolymers.size();}

    // return the total numbers of each type of object created on all processors

	long GetAllProcessorBeadTotal() const;
	long GetAllProcessorBondTotal() const;
    long GetAllProcessorBondPairTotal() const;
	long GetAllProcessorPolymerTotal() const;

	long GetAllProcessorBeadTotalForType(long type) const;
	long GetAllProcessorBondTotalForType(long type) const;
    long GetAllProcessorBondPairTotalForType(long type) const;
	long GetAllProcessorPolymerTotalForType(long type) const;

	// return the number of types of beads, bonds, bondpairs and polymers 

	inline long GetBeadTypeTotal()		const {return vBeadTypes.size();}
	inline long GetBondTypeTotal()		const {return vBondTypes.size();}
	inline long GetBondPairTypeTotal()	const {return vBondPairTypes.size();}
	inline long GetPolymerTypeTotal()	const {return vPolymerTypes.size();}

	// return a vector of the numbers of each type of bead, bond, bondpair and polymer

	inline const zLongVector& GetBeadTypeTotals()     const {return m_vBeadTypeTotal;}
	inline const zLongVector& GetBondTypeTotals()     const {return m_vBondTypeTotal;}
	inline const zLongVector& GetBondPairTypeTotals() const {return m_vBondPairTypeTotal;}
	inline const zLongVector& GetPolymerTypeTotals()  const {return m_vPolymerTypeTotal;}

	// return the number of each bead, bond and polymer type; and the polymer fractions

	long GetBeadTotalForType(long type) const;
	long GetBondTotalForType(long type) const;
	long GetBondPairTotalForType(long type) const;
	long GetPolymerTotalForType(long type) const;
	long GetPolymerSizeForType(long type) const;
	double GetPolymerFractionForType(long type) const;

	// return a vector of the bead, bond, bondpair and polymer types

	inline const BeadVector&		GetBeadTypes()		const	{return vBeadTypes;}
	inline const BondVector&		GetBondTypes()		const	{return vBondTypes;}
	inline const BondPairVector&	GetBondPairTypes()	const	{return vBondPairTypes;}
	inline const PolymerVector&		GetPolymerTypes()	const	{return vPolymerTypes;}

	// functions to get information about the bead types

    // Functions returning details of the bead structure for all simulation types.
	// We pass them by copies so that the CCNTCell class can have its own copies.

	// DPD and BD

	inline const zArray2dDouble*  GetDPDBeadConsIntArray() const {return &m_vvConsInt;}
	inline const zArray2dDouble*  GetDPDBeadDissIntArray() const {return &m_vvDissInt;}
	inline const zArray2dDouble*  GetDPDBeadLGIntArray()   const {return &m_vvLGInt;}

	// MD

	inline const zArray2dDouble*  GetMDBeadLJDepthArray() const {return &m_vvLJDepth;}
	inline const zArray2dDouble*  GetMDBeadLJRangeArray() const {return &m_vvLJRange;}
	inline const zArray2dDouble*  GetMDBeadSCDepthArray() const {return &m_vvSCDepth;}
	inline const zArray2dDouble*  GetMDBeadSCRangeArray() const {return &m_vvSCRange;}

    // Functions to get and set a single DPD bead-bead interaction: we include
    // the function for setting the density-dependent force parameter even if
    // is not needed in a standard DPD run.

	double GetDPDBeadConsInt(long first, long second) const;
	double GetDPDBeadDissInt(long first, long second) const;
	double GetDPDBeadLGInt(long first, long second) const;

	void SetDPDBeadConsInt(long first, long second, double newValue);
    void SetDPDBeadDissInt(long first, long second, double newValue);
    void SetDPDBeadLGInt(long first, long second, double newValue);


	// Functions indicating the constraints imposed on the SimBox.

	inline bool IsPolymerised()			const {return m_bIsPolymerised;}
	inline bool IsWallPresent()			const {return m_bWall;}
	inline bool IsWallPolymerFlexible()	const {return m_bWallPolymerFlexible;}
	inline bool IsGravityPresent()		const {return m_bGravity;}
	inline bool IsShearPresent()		const {return m_bShear;}

    // Functions to return the set of command targets and decorators
    // imported from a restart state

    CommandTargetSequence GetCommandTargets();
    CommandTargetSequence GetDecorators();

	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	void CreatePolymers();   // Serial versions
	void CreateWallPolymers();

	void SetGravityBeads();	// Function to store bead pointers in a vector
							// if they are affected by the body force

	// Functions to write out the numbers of each type of bead, bond, 
	// bondpair and polymer

	void WriteBeadTypeData()	 const;
	void WriteBondTypeData()	 const;
	void WriteBondPairTypeData() const;
	void WritePolymerData()		 const;

	// Functions to write out the data to an XML or ASCII file

	void SerializeXMLFile();
	void SerializeAsciiFile();


	// Function to add the DPD interactions for a new bead type

	void AddDPDBeadType(long oldType);

	// Function to add the MD interactions for a new bead type

	void AddMDBeadType(long oldType);

    // Functions to add a command target and decorator to the local stores; and 
    // retrieve them given their name.

    bool AddCommandTarget(CCommandTargetNode* const pTarget);
	bool AddDecorator(CCommandTargetNode* const pTarget);

	CCommandTargetNode*   GetCommandTarget(const zString name);
	CCommandTargetNode*   GetDecorator(const zString name);

    // Function to connect each target to its inner and outer decorators
    // after all targets have been read from a restart state

	bool ConnectTargetInnerOuterDecorators(CCommandTargetNode* const pTarget);

	// ****************************************
	// Data members
private:

#if EnableParallelSimBox == SimMPSEnabled
    // Nested class that handles parallel initial states

    mpaInitialState*  m_pParallelIS;
#endif

    IInclusiveRestartState* const m_pIRS; // Interface to CSimState for loading inclusive restart states

    const bool m_bDPDLG;  // Flag showing if the DPDLG force is included

    long RNGSeed;

    long ProcessorsXNo;   // Number of processors in X dimension of simulation space
    long ProcessorsYNo;   // Number of processors in Y dimension of simulation space
    long ProcessorsZNo;   // Number of processors in Z dimension of simulation space

	long CNTXCellNo;
	long CNTYCellNo;
	long CNTZCellNo;

	double CNTXCellWidth;
	double CNTYCellWidth;
	double CNTZCellWidth;

	double AveBeadDensity;
	double kT;

	BeadVector		vBeadTypes;		
	BondVector		vBondTypes;       
	BondPairVector  vBondPairTypes;       
	PolymerVector	vPolymerTypes;

	zLongVector		m_vBeadTypeTotal;		// Numbers of each bead type
	zLongVector		m_vBondTypeTotal;		// Numbers of each bond type
	zLongVector		m_vBondPairTypeTotal;	// Numbers of each bond type
	zLongVector		m_vPolymerTypeTotal;	// Numbers of each polymer type

	BeadVector     vAllBeads;				// All beads in the SimBox (bulk and wall)
	BondVector     vAllBonds;				// All bonds in the polymers
	BondPairVector vAllBondPairs;			// All bondpairs in the polymers
	PolymerVector  vAllPolymers;			// All polymers except wall polymers

	BeadVector		vGravityBeads;			// Beads affected by body force
	AbstractBeadVector  vWallBeads;			// Wall beads occuring in wall volume
	BondVector     vAllPolymerisedBonds;	// All polymerised bonds
	PolymerVector  vWallPolymers;			// Polymers bound to the walls

	// Map from polymer types to their size and number fraction

	LongLongMap         m_mPolymerSizes;
	LongDoubleMap       m_mPolymerFractions;

	// Data relating to the constraints that can be imposed on the SimBox.
	// Each constraint has a flag showing if it is active, and its required
	// data are presented after all such flags.

	bool m_bWall;							// Walls may be on any face of the SimBox
	bool m_bWallPolymerFlexible;			// Wall polymers may have more than one bead
	bool m_bGravity;						// Is there an external body force
	bool m_bShear;							// Are sliding BCs used
	bool m_bIsPolymerised;					// Polymers in the initial state may be bonded

	
	zString WallPolymerName;				// name of polymer bound by its head to wall
	long WallPolymerType;					// index into polymer vector for wall polymer
	double WallBeadDensity;					// volume density not surface
	long WallXWidth;						// in numbers of CNT cells
	long WallYWidth;
	long WallZWidth;

	zLongVector vGravityBeadTypes;			// Types of beads affected by body force
	double GravityXForce;					// Components of body force
	double GravityYForce;
	double GravityZForce;

	// Bead structure data includes the interaction parameters for DPD and MD
	// simulations. They are stored as a vector of vectors.
	// These are never both used in the same simulation, it being prevented by 
	// the functions CreateNewTypes() and AddBeadType().

	zArray2dDouble m_vvConsInt;	// DPD
	zArray2dDouble m_vvDissInt;
	zArray2dDouble m_vvLGInt;

	zArray2dDouble m_vvLJDepth;	// MD
	zArray2dDouble m_vvLJRange;
	zArray2dDouble m_vvSCDepth;
	zArray2dDouble m_vvSCRange;

    CommandTargetSequence m_CommandTargets;		// Command targets read from a restart state
    CommandTargetSequence m_Decorators;			// Command target decorators read from a restart state


	const CInitialStateData* const m_pISD;	// Pointer to the initial state data object
	
	// Temporary storage of entity totals for use in serialisation. Only P0 can write to file so the Serialize() function is 
	// is only called by P0 and hence it cannot communicate with the other processors. We hold the totals in these locations
	// before writing them to file.
	
	long  m_TempBeadTotal;    
	long  m_TempBondTotal;    
	long  m_TempBondPairTotal;    
	long  m_TempPolymerTotal;    
	
	zLongVector  m_vTempBeadTypeTotal;
	zLongVector  m_vTempBondTypeTotal;
	zLongVector  m_vTempBondPairTypeTotal;
	zLongVector  m_vTempPolymerTypeTotal;
	
};

#endif // !defined(AFX_INITIALSTATE_H__83457700_3C59_11D3_820E_0060088AD300__INCLUDED_)
