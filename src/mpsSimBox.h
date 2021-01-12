// mpsSimBox.h: interface for the mpsSimBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPSSIMBOX_H__ED15AC77_F4B2_4602_B0BF_8BF23416E792__INCLUDED_)
#define AFX_MPSSIMBOX_H__ED15AC77_F4B2_4602_B0BF_8BF23416E792__INCLUDED_



// Forward declarations

class CBead;

// Include files

#include "mpsBorder.h"
#include "mpsFace.h"
#include "mpsEdge.h"
#include "mpsCorner.h"
#include "CNTCell.h"
#include "ExternalCNTCell.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "mpuGhostBead.h"
#include "mpuExtendedBond.h"
#include "mpuExtendedPolymer.h"
#include "mpuExtendedNanoparticle.h"


#include "xxParallelBase.h"

class mpsSimBox : public xxParallelBase
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mpsSimBox(long geometry, long normal, long px, long py, long pz, 
              long lx, long ly, long lz, double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
			  PolymerVector& rvBulkPolymers);

	mpsSimBox(const mpsSimBox& oldSimBox);

	virtual ~mpsSimBox();

	// ****************************************
	// Global functions, static member functions and variables

public:

    static long  GlobalCellCounter;
    static long  GlobalCellCellIntCounter;

    static const long m_InitialEmptyPolymers = 1000;           // Initial number of empty polymer instances
    static const long m_InitialEmptyExtendedPolymers = 1000;   // Initial number of empty extended polymer instances

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

    // Functions to connect the processors in the Simulation Space and assign
    // CNT cells either to the processor's internal "Bulk" region or to its
    // "Border" regions.

    virtual void PartitionSpace() = 0;

    // Functions to update the coordinates of beads in Border regions: these functions
    // have to be provided by derived classes that represent different geometries
    // of the SimBox

    virtual void UpdateBorderPos() = 0;
	virtual void UpdateBorderForce() = 0;


	// ****************************************
	// Public access functions
public:

    // Functions to get the dimension and normal to the parallel SimBox, 
    // and a flag showing if assembly failed or not.

    inline bool GetAssemblyFailed() const {return m_bFailed;}
    inline bool IsExternal()        const {return m_bExternal;}
    inline long GetGeometry()       const {return m_Geometry;}
    inline long GetNormal()         const {return m_Normal;}

    // Functions called by the CSimBox to update the coordinates of beads 
    // in the processor's Space: this includes its Bulk volume and Border regions.

    void UpdatePos();
	void UpdateForce();
	void UpdateMom();

	// Funcion to add bond forces to beads that are in adjacent processors
	void AddExternalBondForcesToBeads(long id, double x, double y, double z, double* pfx, double* pfy, double* pfz);


	long MapCellToIndex(long i, long j, long k, long alpha, long beta, long gamma) const;

	long MapCellToNNIndex(long alpha, long beta, long gamma) const;

	long GetNeighbourPidFromSlot(long index) const;
	
	long GetNeighbourPidFromLocation(long i, long j, long k, long alpha, long beta, long gamma);

	long GetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma) const; 


    // Helper functions for Border instances

    inline long GetProcessorXNo() const {return m_PX;}
    inline long GetProcessorYNo() const {return m_PY;}
    inline long GetProcessorZNo() const {return m_PZ;}

    inline long Getpi() const {return m_pi;}
    inline long Getpj() const {return m_pj;}
    inline long Getpk() const {return m_pk;}

    inline long GetCNTXCellNo() const {return m_LX;}
    inline long GetCNTYCellNo() const {return m_LY;}
    inline long GetCNTZCellNo() const {return m_LZ;}

    inline double GetCNTXCellWidth() const {return m_DX;}
    inline double GetCNTYCellWidth() const {return m_DY;}
    inline double GetCNTZCellWidth() const {return m_DZ;}

    inline double GetSimBoxXLength() const {return m_SimBoxXLength;}
    inline double GetSimBoxYLength() const {return m_SimBoxYLength;}
    inline double GetSimBoxZLength() const {return m_SimBoxZLength;}
    
    // Function to return all the polymers in the owning processor's Bulk space
    
    PolymerVector GetAllBulkPolymers() const;
    
    // Access functions to nanoparticles
    
    void AddNanoparticle(CNanoparticle* const pNano);
    
    void UpdateBulkNanoparticles();
    void UpdateBorderNanoparticles();

	// ****************************************
	// Protected local functions

protected:

    // Function to check that all bead ids are non-negative
    bool CheckBeadIds() const;
	
	// Function to check that each beads and its owning polymer have the same ids for a simulation containing only single-bead polymers
	bool CheckAllSingleBeadPolymerIds() const;

	bool ConnectNeighbours();


    bool SetNeighbourPidFromCoordinateDifferences(long alpha, long beta, long gamma);

    long GetCellIndexFromCoords(long i, long j, long k) const;
	long GetPidIndexFromCoordinateDifferences(long alpha, long beta, long gamma) const;

    // Functions to package beads that are to be moved to a neighbouring processor

	long UpdateFaceProcessor(long alpha, long beta, long gamma);
	long UpdateEdgeProcessor(long alpha, long beta, long gamma);
	long UpdateCornerProcessor(long alpha, long beta, long gamma, mpsCorner* const pC1);

    // Functions to manage the empty bead instances


    // ********************
    // Functions to return the set of beads in a Face plus its adjacent Edges/Corners,
	// and in an Edge plus its adjacent Corners.

	BeadList GetFaceEdgeCornerBeads(const mpsFace& rFace);

	BeadList GetEdgeCornerBeads(const mpsEdge& rEdge);

	BeadList GetCornerBeads(const mpsCorner& rCorner);

    // ********************
    // Functions to return the set of external CNT cells in a Face plus its
	// adjacent Edge/Corners, and in an Edge plus its adjacent Corners.

    CNTCellVector& GetFaceEdgeCornerExternalCells(mpsFace& rFace);

	CNTCellVector& GetEdgeCornerExternalCells(mpsEdge& rEdge);

	CNTCellVector& GetCornerExternalCells(mpsCorner& rCorner);


    // Function to add external forces to beads in a set of given Border regions
    void AddExternalForcesToBeads(BeadList& rlBeads, zLongVector& rIds, zDoubleVector& rFX, zDoubleVector& rFY, zDoubleVector& rFZ);

    // Debug version of the function that includes the number of interactions per bead
    void AddExternalForcesToBeads(BeadList& rlBeads, zLongVector& rIds, zLongVector& rFCs, zDoubleVector& rFX, zDoubleVector& rFY, zDoubleVector& rFZ);
	
    // Function to convert a list of beads into a list of their owning polymers and move the polymers from the bulk map to the extended map
	
    void MoveOwningPolymersToExtended(long newPid, BeadList& lBeads);
	
    // Function to convert a list of beads into a list of their owning polymers and move the polymers from the extended map to the bulk map:
	// this occurs either when ghost beads move back into the processor's Space so that their owning polymer is entirely within the Space of a single
	// processor, or all the beads in a polymer move to another processor's Space.
	
    void MoveOwningPolymersToBulk(long oldPid, BeadList& lBeads, const zLongVector& vBeadLocations, const zLongVector& vFirstBondIds, const zLongVector& vFirstBPIds, const zLongVector& vPolymerIds, const zLongVector& vPolymerTypes);
	    
    // Function to transfer a bead's owning polymer from the owning processor's bulk polymer container to its extended polymer container.
	// If the polymer is already in the map, the function returns false and does no insertion, otherwise it returns true.
	
	bool TransferBulkPolymerToExtended(long newPid, CAbstractBead* const pBead);
	
	// Inverse function of the above that transfers a polymer back to the bulk map. Return value is as above.
	bool TransferExtendedPolymerToBulk(long oldPid, CAbstractBead* const pBead, long location, long firstBondId, long firstBondPairId, long polyId, long polyType, long polySize);
	
	// Functions to add/remove bonds to the extended map that contain a given bead that enters or leaves the owning processor's Space
	
	void UpdateExtendedBondMapOnBeadEntry(long beadId, mpuExtendedPolymer* const pExtPolymer);
	void UpdateExtendedBondMapOnBeadExit(long beadId, mpuExtendedPolymer* const pExtPolymer);
	
	// Function to update the coordinates of local ghost beads after receiving them from a neighbouring processor.

	bool UpdateGhostBeadCoordinates();
	
	
	// ****************************************
	// Implementation

    // Function showing if the parallel SimBox assembly failed
	void SetAssemblyFailed();

    // Function to store a flag showing if the parallel SimBox instance is external
	void SetExternal();

	// Helper function to retrieve an extended polymer instance from the map given its unique id as the key
	mpuExtendedPolymer* GetExtendedPolymerFromId(long id) const;

    // Helper function to write out the contents of the extended polymers map
	void ListExtendedPolymers() const;
	
	// Helper function to dump a given extended polymer's data
	void ListExtendedPolymer(long polyId) const;

    // Helper function to write out the contents of the extended bonds map
	void ListExtendedBonds() const;

	// Helper function to dump a given extended bond's data
	void ListExtendedBond(long bondId) const;


	// Function to count the total number of beads in this processor's Space
	long GetBeadTotalInCNTCells() const;

	long GetBeadCounterTotal() const {return m_BCTotal;}
	

	// Local members used by derived classes to calculate World observables
	// and write their values to the log file.

	long CalculateBeadTotal();
	long CalculateAngularMomentum();
	long CalculateCMVel();

	void LogWorldBeadTotal();
	void LogWorldAngularMomentum();
	void LogWorldCMVel();

	// Local members used by derived classes to collect statistics on the 
	// number of beads transferred in each direction per processor.
	
    long m_BeadCrossingCounter;
	
	zLongVector m_vBeadCrossing;
	zLongVector m_vBeadCrossing2;


	// ****************************************
	// Private functions
private:

    // Functions to return the numbers of empty polymer and extended polymer instances available
	
    inline long GetEmptyPolymerTotal()         const {return m_lEmptyPolymers.size();}
    inline long GetEmptyExtendedPolymerTotal() const {return m_lEmptyExtendedPolymers.size();}

    // Functions to create new polymer instances to wrap those that enter the processor's Space
	
    bool CreateEmptyPolymers();
    bool CreateEmptyExtendedPolymers();
	
	// Function to remove an entry from the extended bond map defined by the pair (key, extended bond id)

	void RemoveExtendedBondFromMap(long key, long bondId);
	
	// Debug functions to write out information on the processor's extended polymers and bonds
	
	void DumpExtendedPolymer(long polyId, bool bDumpAll);
	void DumpExtendedBond(long bondId, bool bDumpAll);

	// ****************************************
	// Data members

protected:

    const long m_LX;  
    const long m_LY;  // No of CNT cells in each dimension of each SimBox
    const long m_LZ;

    const double m_DX;  
    const double m_DY;  // Width of CNT cells in each dimension
    const double m_DZ;

    const double m_SimBoxXLength;  
    const double m_SimBoxYLength;  // Size of SimBox in each dimension
    const double m_SimBoxZLength;  

    const CNTCellVector&           m_rvCNTCells;              // All CNT cells in processor's Space
    CNTCellVector                  m_vBulkCNTCells;           // CNT cells within the bulk of processor's Space
    PolymerList                    m_lEmptyPolymers;          // Set of empty polymers waiting to be filled by single-bead polymers re-entering
    ExtendedPolymerList            m_lEmptyExtendedPolymers;  // Set of empty extended polymers waiting to be filled
    LongPolymerMap                 m_mBulkPolymers;           // Set of polymers wholly within processor Space
    LongExtendedPolymerMap         m_mExtendedPolymers;       // Set of polymers that cross processor boundaries
    LongExtendedBondMMap           m_mmExtendedBonds;         // Set of bonds that cross processor boundaries
    LongNanoparticleMap            m_mBulkNanoparticles;      // Set of nanoparticles wholly within processor space
    LongExtendedNanoparticleMap    m_mBorderNanoparticles;    // Set of nanoparticles that may span processor boundaries

// Local data required for calculation of World observables used for debug and
// to characterise the processor World.

    long   m_BeadTotal;                 // Total number of beads in thisprocessor's Space.
    long   m_ExtendedBondCounter;       // Number of extended bonds held by all processors: used by derived classes for debug purposes
    long   m_ExtendedBondForceCounter;  // Number of extended bonds on all processors whose force have been calculated per timestep: "
    double m_AngMom[3];                 // Total angular momentum    "
    double m_CMVel[3];                  // CM velocity for all beads in processor's Space



// mpsBorder-derived classes that hold beads in the boundary regions of 
// the processor's Space

   mpsFace* m_pR;          // id = 0, Right face 
   mpsFace* m_pL;          // id = 1, Left face
   mpsFace* m_pT;          // id = 2, Top face
   mpsFace* m_pB;          // id = 3, Bottom face
   mpsFace* m_pU;          // id = 4, Up face border region
   mpsFace* m_pD;          // id = 5, Down face border region

   mpsEdge* m_pTR;         // id = 0, Top-Right edge
   mpsEdge* m_pBR;         // id = 1, Bottom-Right edge
   mpsEdge* m_pUR;         // id = 2, Up-Right edge
   mpsEdge* m_pDR;         // id = 3, Down-Right edge
   mpsEdge* m_pTL;         // id = 4, Top-Left edge
   mpsEdge* m_pBL;         // id = 5, Bottom-Left edge
   mpsEdge* m_pUL;         // id = 6, Up-Left edge
   mpsEdge* m_pDL;         // id = 7, Down-Left edge
   mpsEdge* m_pUT;         // id = 8, Up-Top edge
   mpsEdge* m_pDT;         // id = 9, Down-Top edge
   mpsEdge* m_pUB;         // id = 0, Up-Bottom edge
   mpsEdge* m_pDB;         // id = 11, Down-Bottom edge

   mpsCorner* m_pUTR;      // id = 0, Up-Top-Right corner
   mpsCorner* m_pDTR;      // id = 1, Down-Top-Right corner
   mpsCorner* m_pUBR;      // id = 2, Up-Bottom-Right corner
   mpsCorner* m_pDBR;      // id = 3, Down-Bottom-Right corner
   mpsCorner* m_pUTL;      // id = 4, Up-Top-Left corner
   mpsCorner* m_pDTL;      // id = 5, Down-Top-Left corner
   mpsCorner* m_pUBL;      // id = 6, Up-Bottom-Left corner
   mpsCorner* m_pDBL;      // id = 7, Down-Bottom-Left corner
   
// Pids of adjacent processors assigned and used by derived classes. Note that all pids are assigned an illegal value in the 
// constructor so that derived classes must assign correct values to those pids that they use.

    long UFacePid;
    long DFacePid;
    long RFacePid;
    long LFacePid;
    long TFacePid;
    long BFacePid;

    long TREdgePid;
    long TLEdgePid;
    long BREdgePid;
    long BLEdgePid;

    long UREdgePid;
    long ULEdgePid;
    long UTEdgePid;
    long UBEdgePid;

    long DREdgePid;
    long DLEdgePid;
    long DTEdgePid;
    long DBEdgePid;

    long UTRCornerPid;
    long UTLCornerPid;
    long UBRCornerPid;
    long UBLCornerPid;
    long DTRCornerPid;
    long DTLCornerPid;
    long DBRCornerPid;
    long DBLCornerPid;
   
// Local data used by derived classes in writing out debug and logging information about a run   
   

    long localBeadTotalTimer;  // Bead total counter
	
    zDoubleVector  m_vXCMVel;  // CM velocity for each processor accumulated on P0
    zDoubleVector  m_vYCMVel;
    zDoubleVector  m_vZCMVel;
 

private:

    const long m_Geometry;  // Simulation space is: linear, planar or cubic
    const long m_Normal;    // Normal to simulation space (1,2 or 3 for X, Y, or Z)
    const long m_BCTotal;   // Number of bead crossing counters needed

    const long m_PX;
    const long m_PY;  // No of processors in each dimension 
    const long m_PZ;                    
                                   // this is equal to (Lx-2)*(Ly-2)*(Lz-2)
    long m_pi;
    long m_pj;  // Location of containing processor in the processor geometry
    long m_pk;

    bool m_bFailed;    // Flag showing if the parallel SimBox creation failed
    bool m_bExternal;  // Flag showing if this processor is on the boundary

    long m_NPid[27];   // Pids of containing processor's 26 nearest neighbours including itself
	
    long velCounter;
	
    // Debugging data that checks bead properties for each Border region

    long m_IntPerBeadCounter;        // Counter for samples of the interactions/bead and beads/cell

    // Beads per cell
	double  m_MeanBulkBeadsPerCell;
	double  m_MeanFaceBeadsPerCell[6];   
	double  m_MeanEdgeBeadsPerCell[12];  
	double  m_MeanCornerBeadsPerCell[8]; 

	double  m_MeanSqBulkBeadsPerCell;
	double  m_MeanSqFaceBeadsPerCell[6];  
	double  m_MeanSqEdgeBeadsPerCell[12];   
	double  m_MeanSqCornerBeadsPerCell[8]; 

	double  m_SDBulkBeadsPerCell;
	double  m_SDFaceBeadsPerCell[6];   
	double  m_SDEdgeBeadsPerCell[12];    
	double  m_SDCornerBeadsPerCell[8]; 

    // Interactions per bead
	double  m_MeanBulkIntPerBead;
	double  m_MeanFaceIntPerBead[6];    // U, D, T, B, R, L
	double  m_MeanEdgeIntPerBead[12];    // UT, UB, UR, UL, DT, DB, DR, DL, TR, TL, BR, BL
	double  m_MeanCornerIntPerBead[8];  // UTR, UTL, UBR, UBL, DTR, DTL, DBR, DBL

	double  m_MeanSqBulkIntPerBead;
	double  m_MeanSqFaceIntPerBead[6];    // U, D, T, B, R, L
	double  m_MeanSqEdgeIntPerBead[12];    // UT, UB, UR, UL, DT, DB, DR, DL, TR, TL, BR, BL
	double  m_MeanSqCornerIntPerBead[8];  // UTR, UTL, UBR, UBL, DTR, DTL, DBR, DBL

	double  m_SDBulkIntPerBead;
	double  m_SDFaceIntPerBead[6];    // U, D, T, B, R, L
	double  m_SDEdgeIntPerBead[12];    // UT, UB, UR, UL, DT, DB, DR, DL, TR, TL, BR, BL
	double  m_SDCornerIntPerBead[8];  // UTR, UTL, UBR, UBL, DTR, DTL, DBR, DBL

};

#endif // !defined(AFX_MPSSIMBOX_H__ED15AC77_F4B2_4602_B0BF_8BF23416E792__INCLUDED_)
