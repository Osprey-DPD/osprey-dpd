// Builder.h: interface for the CBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILDER_H__A2FACF42_3F61_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_BUILDER_H__A2FACF42_3F61_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CInitialState;


#include "xxBase.h"

class CBuilder : public xxBase
{
public:
	CBuilder();
	virtual ~CBuilder();

	// Pure virtual function that must be overridden in every builder class
	// that inherits from CBuilder. It sets the coordinates and velocities of 
	// the beads in a CInitialState object according to the type of initial 
	// configuration specified in the CInitialStateData-derived object.
	// Note that it does not create beads, bonds and polymers but only assembles 
	// them into a given configuration. Each builder must provide means for 
	// dealing with the features that may be turned on by the user, eg., a wall, 
	// gravity, a restart state etc.

	virtual bool Assemble(CInitialState& riState) = 0;

	// Virtual function that must be overridden in every builder class
    // that is to be used in a parallel simulation. It is not a PVF as not all
    // builder classes are yet parallelised.

    virtual bool AssembleP(CInitialState& riState);

protected:
	// Function to return a set of velocities taken from a standard 
	// Maxwell distribution (one with unit mean and variance).

	zDoubleVector MaxwellDist(long n) const;

	// Function to assign velocities to the beads in the initial state taken
	// from a Maxwell distribution. Either 3 or 2 dimensions may be specified,
	// if 2d is chosen, the excluded component is indicated by the normal vector.

	void AssignMaxwellVelocities(CInitialState& riState, long Xn, long Yn, long Zn);

	// Virtual function to position wall beads. It may be overridden by derived
	// classes if they need more specialised assignment.

	virtual void AssignWallBeadCoords(CInitialState& riState) const;

	// Function to copy initial coordinates of beads into non-pbc arrays

	void AssignUnPBCCoords(CInitialState& riState);

	// Helper function that returns true if a point in within a specified
	// spherical region

	bool IsPointWithinSphere(double cx, double cy, double cz, double radius, 
							 double px, double py, double pz) const;

	// Contained classes to assign coordinates to the CInitialState polymers 
	// for specific objects such as bilayers, spheres, randomly filling space.

protected:

	// Random distribution of beads in the SimBox

	class isRandom
	{
	public:
		isRandom(const zLongVector polymerTypes, const zDoubleVector exRegions,
				 long Xn, long Yn, long Zn);
		~isRandom();

		bool Assemble(CInitialState& riState);

	private:

	// Local data relating to filling planar regions with randomly-positioned polymers

		const zLongVector   m_PolymerTypes;
		const zDoubleVector m_ExcludedRegions;
		const long m_ExcludedRegionTotal;	// Number of excluded regions
		const long m_FreeRegionTotal;		// = excluded regions - 1
		const long m_X;
		const long m_Y;						// Normal to planar regions
		const long m_Z;

		long m_PolymerTotal;

		PolymerVector m_vRandom;			// Polymers used to fill regions
	};


	// Random distribution of a single polymer type througout parallel, rectangular
    // slabs covering the SimBox

	class isRandomPolymerSlabs
	{
	public:
		isRandomPolymerSlabs(long polymerType, const zDoubleVector exRegions, long Xn, long Yn, long Zn);
		~isRandomPolymerSlabs();

		bool Assemble(CInitialState& riState);

	private:

	// Local data relating to filling planar regions with randomly-positioned polymers

		const long m_PolymerType;
		const zDoubleVector m_ExcludedRegions;
		const long m_ExcludedRegionTotal;	// Number of excluded regions
		const long m_FreeRegionTotal;		// = excluded regions - 1
		const long m_X;
		const long m_Y;						// Normal to planar regions
		const long m_Z;

		long m_PolymerTotal;

        PolymerVector m_vRandom;			// Polymers used to fill regions
	};


	// Planar bilayer that spans the whole SimBox

	class isBilayer
	{
	public:
		isBilayer(const zLongVector& polymerTypes, long Xn, long Yn, long Zn,
				  double centre, double thickness, 
				  bool bLinear, const zDoubleVector& upperFractions,
				  double area, double length1, double length2,
				  double upperHead, double lowerHead);

		isBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
				  long Xn, long Yn, long Zn, double centre, double thickness, 
				  double area, double length1, double length2,
				  double upperHead, double lowerHead, long upperTotal, long lowerTotal,
				  bool bMonolayer, bool bLinear, const bool bPatches[2]);

		~isBilayer();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a bilayer

		const zLongVector &  m_PolymerTypes;	// Polymer types present in bilayer
		const long   m_X;
		const long   m_Y;						// Normal to bilayer
		const long	 m_Z;
		const double m_BilayerCentre;			// Centre of bilayer in SimBox
		const double m_Thickness;				// Thickness of bilayer
		const double m_BilayerArea;				// Area of SimBox available to bilayer
		const double m_Bilayer1Length;			// Lateral extent of bilayer
		const double m_Bilayer2Length;
		const double m_UpperHead;				// Coordinate of upper monolayer head groups
		const double m_LowerHead;				// Coordinate of lower monolayer head groups
		const long	 m_UpperTotal;
		const long	 m_LowerTotal;
		const bool	 m_bMonolayer;				// Flag showing whether all polymers are in one monolayer
		const bool   m_bLinearise;				// Flag showing whether polymers are to be linearised
		const zDoubleVector& m_UpperFractions;	// Fractions of polymers in upper monolayer
        
		      bool   m_bPatches[2];				// Flag showing if polymers are randomly distributed

		// Local data relating to construction of the bilayer

		zLongVector		m_PolymerTotal;		// No of polymers of each type in bilayer
		zLongVector		m_PolymerUpperTotal;// No of polymers of each type in upper monolayer
		zLongVector		m_PolymerSize;		// Size of each polymer type in bilayer
		zLongVector		m_RandomUpperIndex;	// Shuffled vector of site coordinate indices
		zLongVector		m_RandomLowerIndex;	// Shuffled vector of site coordinate indices
		zDoubleVector	m_UpperXCoord;		// Vector of free coordinates indices for upper monolayer
		zDoubleVector	m_UpperYCoord;		// Vector of free coordinates indices for upper monolayer
		zDoubleVector	m_LowerXCoord;		// Vector of free coordinates for upper monolayer
		zDoubleVector	m_LowerYCoord;		// Vector of free coordinates for upper monolayer

		long    m_PolymerMaxSize;			// No of beads in longest polymer

		long	m_MonolayerTotal[2];		// Number of polymers in each monolayer		
		long	m_LatticeSize[2];			// No of vertices in monolayer lattices
		double	m_MonolayerSign[2];			// -1/+1 for upper/lower monolayers
		double	m_MonolayerHead[2];			// Coordinates of heads in monolayers
		double	m_HexSpace[2];				// Hexagonal lattice spacings
		double	m_HexCAxisSpace[2];			// Bead spacing in lattice normal direction
		double	m_Hexe1X[2];
		double	m_Hexe1Y[2];
		double	m_Hexe2X[2];				// Components of hexagonal lattice vectors
		double	m_Hexe2Y[2];

		PolymerVector m_vBilayer;		// Polymers in bilayer
	};

	// Planar bilayer not spanning the SimBox

	class isFreeBilayer
	{
	public:
		isFreeBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
				  long Xn, long Yn, long Zn, double thickness, 
				  double area, double length1, double length2,
				  double centre1, double centre2, double centre3, 
				  double upperHead, double lowerHead, long upperTotal, long lowerTotal,
				  bool bMonolayer, bool bLinear, const bool bPatches[2]);

		~isFreeBilayer();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a bilayer

		const zLongVector &  m_PolymerTypes;	// Polymer types present in bilayer
		const zDoubleVector& m_UpperFractions;	// Fractions of polymers in upper monolayer
		const long   m_X;
		const long   m_Y;						// Normal to bilayer
		const long	 m_Z;
		const double m_Thickness;				// Thickness of bilayer
		const double m_BilayerArea;				// Area of SimBox available to bilayer
		const double m_Bilayer1Length;			// Lateral extent of bilayer
		const double m_Bilayer2Length;
		const double m_Bilayer1Centre;			// Centre of bilayer in SimBox
		const double m_Bilayer2Centre;			//    "
		const double m_Bilayer3Centre;			//    "
		const double m_UpperHead;				// Coordinate of upper monolayer head groups
		const double m_LowerHead;				// Coordinate of lower monolayer head groups
		const long	 m_UpperTotal;
		const long	 m_LowerTotal;
		const bool	 m_bMonolayer;				// Flag showing whether all polymers are in one monolayer
		const bool   m_bLinearise;				// Flag showing whether polymers are to be linearised
		      bool   m_bPatches[2];				// Flag showing if polymers are randomly distributed

		// Local data relating to construction of the bilayer

		zLongVector		m_PolymerTotal;		// No of polymers of each type in bilayer
		zLongVector		m_PolymerUpperTotal;// No of polymers of each type in upper monolayer
		zLongVector		m_PolymerSize;		// Size of each polymer type in bilayer
		zLongVector		m_RandomUpperIndex;	// Shuffled vector of site coordinate indices
		zLongVector		m_RandomLowerIndex;	// Shuffled vector of site coordinate indices
		zDoubleVector	m_UpperXCoord;		// Vector of free coordinates indices for upper monolayer
		zDoubleVector	m_UpperYCoord;		// Vector of free coordinates indices for upper monolayer
		zDoubleVector	m_LowerXCoord;		// Vector of free coordinates for upper monolayer
		zDoubleVector	m_LowerYCoord;		// Vector of free coordinates for upper monolayer

		long    m_PolymerMaxSize;			// No of beads in longest polymer

		long	m_MonolayerTotal[2];		// Number of polymers in each monolayer		
		long	m_LatticeSize[2];			// No of vertices in monolayer lattices
		double	m_MonolayerSign[2];			// -1/+1 for upper/lower monolayers
		double	m_MonolayerHead[2];			// Coordinates of heads in monolayers
		double	m_HexSpace[2];				// Hexagonal lattice spacings
		double	m_HexCAxisSpace[2];			// Bead spacing in lattice normal direction
		double	m_Hexe1X[2];
		double	m_Hexe1Y[2];
		double	m_Hexe2X[2];				// Components of hexagonal lattice vectors
		double	m_Hexe2Y[2];

		PolymerVector m_vBilayer;		// Polymers in bilayer
	};

	// Set of parallel planar bilayers that span the SimBox

	class isMultiBilayer
	{
	public:
		isMultiBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
				  long Xn, long Yn, long Zn, zDoubleVector centres, double thickness, 
				  double area, double length1, double length2,
				  zLongVector upperTotals, zLongVector lowerTotals,
				  bool bLinear);

		~isMultiBilayer();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of multiple lamellae

		const zLongVector &		m_PolymerTypes;		// Polymer types present in lamellae
		const zDoubleVector&	m_UpperFractions;	// Fractions of polymers in upper leaflet
		const long				m_X;
		const long				m_Y;				// Normal to lamellae
		const long				m_Z;
		const zDoubleVector		m_BilayerCentre;	// Centres of lamellae in SimBox
		const double			m_Thickness;		// Thickness of bilayer
		const double			m_BilayerArea;		// Area of SimBox available to lamellae
		const double			m_Bilayer1Length;	// Lateral extent of lamellae
		const double			m_Bilayer2Length;
		zLongVector				m_UpperTotals;		// Numbers of each polymer type in upper leaflet
		zLongVector				m_LowerTotals;		// Numbers of each polymer type in lower leaflet
		const bool				m_bLinearise;		// Flag showing whether polymers are to be linearised
		const long				m_LamellaTotal;		// No of lamellae

		// Local data relating to construction of the bilayer

		zLongVector		m_PolymerTotal;		// No of polymers of each type in lamellae
		zLongVector		m_PolymerUpperTotal;// No of polymers of each type in upper leaflet
		zLongVector		m_PolymerSize;		// Size of each polymer type in lamellae
		zLongVector		m_RandomUpperIndex;	// Shuffled vector of site coordinate indices
		zLongVector		m_RandomLowerIndex;	// Shuffled vector of site coordinate indices
		zDoubleVector	m_UpperXCoord;		// Vector of free coordinates indices for upper leaflet
		zDoubleVector	m_UpperYCoord;		// Vector of free coordinates indices for upper leaflet
		zDoubleVector	m_LowerXCoord;		// Vector of free coordinates for upper leaflet
		zDoubleVector	m_LowerYCoord;		// Vector of free coordinates for upper leaflet

		long    m_PolymerMaxSize;			// No of beads in longest polymer
		long	m_LatticeSize[2];			// No of vertices in monolayer lattices
		double	m_MonolayerSign[2];			// -1/+1 for upper/lower leaflets
		double	m_MonolayerHead[2];			// Coordinates of heads in leaflets
		double	m_HexSpace[2];				// Hexagonal lattice spacings
		double	m_HexCAxisSpace[2];			// Bead spacing in lattice normal direction
		double	m_Hexe1X[2];
		double	m_Hexe1Y[2];
		double	m_Hexe2X[2];				// Components of hexagonal lattice vectors
		double	m_Hexe2Y[2];

		PolymerVector m_vBilayer;		// Polymers in lamellae
	};


	class isMicelle
	{
	public:
		isMicelle(const zLongVector polymerTypes, double xc, double yc, double zc,
				  double radius);
		~isMicelle();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a micelle

		const zLongVector m_PolymerTypes;	// Polymer types present in micelle
		const double m_XC;
		const double m_YC;					// Centre of micelle (fractions of SimBox sides)
		const double m_ZC;
		const double m_Radius;				// Radius of micelle (bead diameters)

		double m_XCentre;					// Coordinates of micelle centre
		double m_YCentre;
		double m_ZCentre;
	    double m_InnerRadius;			// Radius of tail bead sphere
		double m_OuterRadius;			// Radius of head bead sphere
		double m_DTheta;				// Polar angle element
		double m_DPhi;					// Azimuthal angle element
		long m_PolymerTotal;			// No of polymers in micelle

		PolymerVector m_vMicelle;		// Polymers in micelle
	};

	class isWormMicelle
	{
	public:
		isWormMicelle(const zLongVector polymerTypes, long xn, long yn, long zn, double xc, double yc, double zc, 
					  double radius, double halfLength, bool bPatches, bool bPolymerise);
		~isWormMicelle();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a worm-like micelle

		const zLongVector m_PolymerTypes;	// Polymer types present in micelle
		const long m_XN;
		const long m_YN;					// Micelle's long axis normal
		const long m_ZN;
		const double m_XC;
		const double m_YC;					// Centre of micelle (fractions of SimBox sides)
		const double m_ZC;
		const double m_Radius;				// Radius of micelle (bead diameters)
		const double m_HalfLength;			// Half length of micelle (fraction of SimBox side)
		const bool   m_bPatches;			// Flag showing if polymers are randomised
		const bool   m_bPolymerise;			// Flag showing if polymers are polymerised

		double m_XCentre;				// Coordinates of micelle centre (bead diameters)
		double m_YCentre;
		double m_ZCentre;
		double m_Axis[3];				// Moving coordinates of axis
		double m_LeftEnd;				// Coordinate of left end of cylinder
		double m_RightEnd;				// Coordinate of right end of cylinder
		double m_Length;				// Length of cylinder along axis
		double m_Circumference;			// Circumference of cylinder
	    double m_InnerRadius;			// Radius of tail bead cylinder (bead diameters)
		double m_OuterRadius;			// Radius of head bead cylinder (bead diameters)
		double m_Area;					// Surface area of cylinder (equivalent to a rectangle)
		double m_DZ;					// Coordinate spacing along cylinder long axis
		double m_DPhi;					// Azimuthal angle element
		long m_PolymerTotal;			// No of polymers in micelle

		long	m_LatticeSize;			// No of lattice points
		double	m_HexSpace;				// Hexagonal lattice spacing
		double	m_Hexe1X;				// Components of hexagonal lattice first unit vector
		double	m_Hexe1Y;
		double	m_Hexe2X;				// Components of hexagonal lattice second unit vector
		double	m_Hexe2Y;


		PolymerVector	m_vMicelle;			// Polymers in micelle
		zLongVector		m_RandomIndex;		// Shuffled vector of site coordinate indices

	};

    class isFibril
    {
    public:
        isFibril(const zLongVector polymerTypes, long coreBeadType, long linkerBeadType,
                 long xn, long yn, long zn, double xc, double yc, double zc, double halfLength,
                 double coreRadius, double linkerRadius, double pitch, double pitchAngle);
        
        ~isFibril();

        bool Assemble(CInitialState& riState);

    private:
    // Local data relating to construction of a single, Z axis spanning fibril

        const zLongVector m_PolymerTypes;    // Polymer types present in fibril
        const long   m_CoreBeadType;         // Numeric id of core beads in the polymer
        const long   m_LinkerBeadType;       // Numeric id of the linker beads in the polymer
        const long   m_XN;
        const long   m_YN;                   // Fibril long axis hardwired to (0,0,1)
        const long   m_ZN;
        const double m_XC;
        const double m_YC;                   // Centre of fibril hardwired to 1/2 1/2 1/2
        const double m_ZC;
        const double m_HalfLength;           // Half length of fibril (fraction of SimBox side)
        const double m_CoreRadius;           // Radius of fibril core region (bead diameters)
        const double m_LinkerRadius;         // Radius of fibril linker region (bead diameters)
        const double m_Pitch;                // Monomer thickness along fibril (bead diameters)
        const double m_PitchAngle;           // Monomer rotation along fibril (radians)

        
        double m_XCentre;                // Coordinates of fibril centre (bead diameters)
        double m_YCentre;
        double m_ZCentre;
        double m_Axis[3];                // Moving coordinates of axis
        double m_LeftEnd;                // Coordinate of left end of cylinder
        double m_RightEnd;               // Coordinate of right end of cylinder
        double m_Length;                 // Length of cylinder along axis
        double m_Circumference;          // Circumference of cylinder
        double m_InnerRadius;            // Radius of tail bead cylinder (bead diameters)
        double m_OuterRadius;            // Radius of head bead cylinder (bead diameters)
        double m_Area;                   // Surface area of cylinder (equivalent to a rectangle)
        double m_DZ;                     // Coordinate spacing along cylinder long axis
        double m_DPhi;                   // Azimuthal angle element
        long m_CoreTotal;                // No of core polymers in fibril
        long m_LinkerTotal;              // No of linker polymers in fibril

        long    m_LatticeSize;           // No of lattice points
        double  m_HexSpace;              // Hexagonal lattice spacing
        double  m_Hexe1X;                // Components of hexagonal lattice first unit vector
        double  m_Hexe1Y;
        double  m_Hexe2X;                // Components of hexagonal lattice second unit vector
        double  m_Hexe2Y;


        PolymerVector    m_vFibril;      // Core polymers in fibril with no attached linker
        PolymerVector    m_vLinker;      // Linker polymers in fibril that have attached GFP if desired
        zLongVector      m_RandomIndex;  // Shuffled vector of site coordinate indices

    };


	class isVesicle
	{
	public:
		isVesicle(const zLongVector polymerTypes, const zDoubleVector outerFractions,
				  double xc, double yc, double zc, double outerRadius, double innerRadius, 
				  double thickness, bool bPatches[2]);
		~isVesicle();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a vesicle.
	// All radii are in units of the bead diameter

		const zLongVector	m_PolymerTypes;		// Polymer types present in vesicle
		const zDoubleVector m_OuterFractions;	// Fraction of polymers in outer monolayer
		const double m_XC;
		const double m_YC;					// Coordinates of centre of vesicle
		const double m_ZC;
		const double m_OuterRadius;			// Outer radius of vesicle
	    const double m_InnerRadius;			// Inner radius of vesicle
		const double m_Thickness;			// Thickness of bilayer
		double m_bPatches[2];			// Homogeneous (0) or patched distribution (1)

		zLongVector		m_PolymerTotal;		// No of polymers of each type in vesicle
		zLongVector		m_PolymerOuterTotal;// No of polymers of each type in outer monolayer
		zLongVector		m_PolymerSize;		// Size of each polymer type in vesicle
		zLongVector		m_RandomOuterIndex;	// Shuffled vector of site coordinate indices
		zLongVector		m_RandomInnerIndex;	// Shuffled vector of site coordinate indices
		zDoubleVector	m_OuterTheCoord;	// Vector of theta coordinates for outer monolayer
		zDoubleVector	m_OuterPhiCoord;	// Vector of phi      "              "
		zDoubleVector	m_InnerTheCoord;	// Vector of theta coordinates for inner monolayer
		zDoubleVector	m_InnerPhiCoord;	// Vector of phi      "              "

		long	m_AllPolymerTotal;			// No of polymers of all types in vesicle
		long	m_PolymerMaxSize;			// No of beads in longest polymer
		double m_XCentre;					// Coordinates of vesicle centre
		double m_YCentre;
		double m_ZCentre;

		long   m_MeanMonolayerTotal[2];		// Relaxed no of polymers in monolayers: 0 = outer, 1 = inner
		long   m_MonolayerTotal[2];			// No of polymers in monolayers: 0 = outer, 1 = inner
		double m_MonolayerSign[2];			// +1 for outer, -1 for inner monolayer
		double m_MonolayerRadius[2];		// Copy of monolayer radii for use in loops
		double m_BilayerRadius[2];			// Radius of the bilayer midplane for use in loops
		double m_HexCAxisSpace[2];			// Lattice spacing between beads in polymers
		double m_DTheta[2];					// Polar angle element
		double m_DPhi[2];					// Azimuthal angle element
		PolymerVector m_vVesicle;			// Polymers in vesicle
	};

	class isMultiVesicle
	{
	public:
		isMultiVesicle(const zLongVector polymerTypes, const zDoubleVector outerFractions,
				  zDoubleVector xc, zDoubleVector yc, zDoubleVector zc, double outerRadius, double innerRadius, 
				  double thickness, bool bPatches[2]);
		~isMultiVesicle();

		bool Assemble(CInitialState& riState);

	private:
	// Local data relating to construction of a vesicle.
	// All radii are in units of the bead diameter

		const long		    m_VesicleTotal;		// Number of vesicles to create
		const zLongVector	m_PolymerTypes;		// Polymer types present in vesicles
		const zDoubleVector m_OuterFractions;	// Fraction of polymers in outer monolayer
		const zDoubleVector m_XC;
		const zDoubleVector m_YC;				// Coordinates of vesicle centres
		const zDoubleVector m_ZC;
		const double m_OuterRadius;				// Outer radius of vesicles
	    const double m_InnerRadius;				// Inner radius of vesicles
		const double m_Thickness;				// Thickness of bilayer
		double m_bPatches[2];					// Homogeneous (0) or patched distribution (1)

		zLongVector		m_PolymerTotal;			// No of polymers of each type in vesicle
		zLongVector		m_PolymerOuterTotal;	// No of polymers of each type in outer monolayer
		zLongVector		m_PolymerSize;			// Size of each polymer type in vesicle
		zLongVector		m_RandomOuterIndex;		// Shuffled vector of site coordinate indices
		zLongVector		m_RandomInnerIndex;		// Shuffled vector of site coordinate indices
		zDoubleVector	m_OuterTheCoord;		// Vector of theta coordinates for outer monolayer
		zDoubleVector	m_OuterPhiCoord;		// Vector of phi      "              "
		zDoubleVector	m_InnerTheCoord;		// Vector of theta coordinates for inner monolayer
		zDoubleVector	m_InnerPhiCoord;		// Vector of phi      "              "

		long	m_AllPolymerTotal;				// No of polymers of all types in vesicle
		long	m_PolymersPerVesicle;			// No of polymers in each vesicle except the last
		long	m_PolymersInLastVesicle;		// No of polymers in last vesicle
		long	m_PolymerMaxSize;				// No of beads in longest polymer
		double m_XCentre;						// Coordinates of vesicle centre
		double m_YCentre;
		double m_ZCentre;

		long   m_MeanMonolayerTotal[2];		// Relaxed no of polymers in monolayers: 0 = outer, 1 = inner
		long   m_MonolayerTotal[2];			// No of polymers in monolayers: 0 = outer, 1 = inner
		double m_MonolayerSign[2];			// +1 for outer, -1 for inner monolayer
		double m_MonolayerRadius[2];		// Copy of monolayer radii for use in loops
		double m_BilayerRadius[2];			// Radius of the bilayer midplane for use in loops
		double m_HexCAxisSpace[2];			// Lattice spacing between beads in polymers
		double m_DTheta[2];					// Polar angle element
		double m_DPhi[2];					// Azimuthal angle element
		PolymerVector m_vVesicle;			// Polymers in vesicle
	};

protected:
	const double m_CoordErrorLimit;		// Epsilon to ensure wall bead coords lie inside SimBox
	const long   m_MaxwellPointNo;		// Default no of points in velocity distribution
};

#endif // !defined(AFX_BUILDER_H__A2FACF42_3F61_11D3_820E_0060088AD300__INCLUDED_)
