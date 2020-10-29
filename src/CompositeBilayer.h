// CompositeBilayer.h: interface for the CCompositeBilayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOSITEBILAYER_H__94E4661A_1107_4CE1_9D44_784B3D55E032__INCLUDED_)
#define AFX_COMPOSITEBILAYER_H__94E4661A_1107_4CE1_9D44_784B3D55E032__INCLUDED_


// Forward declarations

class CCompositeBilayerData;
class CSliceProfile;
class CCellProfileSet;
class aaScalarProfile;
class aaVectorProfile;
class aaStressTensor1d;

#include "Polymer.h"
#include "Analysis.h"

class CCompositeBilayer : public CAnalysis
{
	// ****************************************
	// Construction/Destruction
public:

	CCompositeBilayer(const zString runId, long samplePeriod, const CCompositeBilayerData* const pAD);
	virtual ~CCompositeBilayer();


	// ****************************************
	// IEventXXX interface functions for all supported events
public:

	virtual bool	LamellaMonolayerDomainSupported() const;
	virtual long	GetRaftTotal()					  const;
	virtual double	GetMeanRaftSize()				  const;
	virtual double	GetMeanRaftPerimeter()			  const;

	virtual bool	LamellaMonolayerPhaseSeparationSupported() const;
	virtual bool	HasPhaseSeparated() const;

	virtual bool LamellaBilayerDomainSupported() const;

	virtual bool LamellaBilayerPhaseSeparationSupported() const;


	// ****************************************
	// IBilayerXXX interface functions for all supported processes
public:

	double GetCMXPos() const;
	double GetCMYPos() const;
	double GetCMZPos() const;

	// ****************************************
	// Public access functions
public:

	inline const zString GetPolymer() 	 const {return m_Polymer;}
	inline long			 GetXNormal()	 const {return m_X;}
	inline long			 GetYNormal()	 const {return m_Y;}
	inline long			 GetZNormal()	 const {return m_Z;}

	inline long			 GetSliceTotal() const {return m_SliceTotal;}
	inline long			 GetRowTotal()   const {return m_RowTotal;}
	inline long			 GetCellTotal()  const {return m_CellTotal;}

	inline double		GetSliceWidth()  const {return m_SliceWidth;}
	inline double		GetRowWidth()    const {return m_RowWidth;}
	inline double		GetCellWidth()   const {return m_CellWidth;}

	// ****************************************
	// PVS inherited from CAnalysis base class

	// Function to get information about the bilayer from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);


	// ****************************************
	// CAnalysisTool forwarding functions




private:

	void Thickness(CCellProfileSet* const pCPS);
	void AreaPerLipid(void);
	void Shape1d();
	void Stress1d(const ISimBox* const pISimBox);
	void SurfaceTension();

private:
	// Data specified by user for bilayer analysis

	zString			m_Polymer;				// Major polymer composing bilayer
	StringSequence	m_CoPolymers;			// Other polymers in bilayer

	long m_X;								// Normal to bilayer
	long m_Y;
	long m_Z;

	long m_SliceTotal;						// No of slices in SimBox profile
	long m_RowTotal;						// Number of rows used per slice in shape analysis
	long m_CellTotal;						// No of cells used per row in shape analysis

	zString			m_Solvent;				// Polymer name representing surrounding solvent

	zString			m_FRAPPolymer;			// Polymer name representing FRAP target

	StringSequence	m_RaftPolymers;			// Raft-forming polymers
	bool			m_bRaftMonolayers[2];	// Which monolayers to monitor for rafts: upper/lower

	// Local data used in the analysis

	CTimeSeriesData* m_pTSD;				// Store time-dependent data here

	long m_LipidType;						// Type of bilayer's major component polymer
											// (referred to as a lipid in this code)
	zLongVector m_CoPolymerTypes;			// Types of copolymers in bilayer
	zLongVector m_CoPolymerHeadTypes;		// Types of copolymer head beads in bilayer
	zLongVector m_CoPolymerTailTypes;		// Types of copolymer tail beads in bilayer
	zLongVector m_CoPolymerBeadTypes;		// Numbers of bead types in each copolymer
	zLongVector m_CoPolymerBondTypes;		// Numbers of bond types in each copolymer
	zLongVector m_CoPolymerBondPairTypes;	// Numbers of bondpair types in each copolymer

	long m_LipidHeadType;					// Type of lipid head bead
	long m_LipidTailType;					// Type of lipid tail bead
	long m_SolventType;						// Type of solvent polymer
	long m_SolventHeadType;					// Type of solvent polymer head beads

	// FRAP analysis tool data

	long m_FRAPPolymerType;					// Type of polymer used in FRAP analysis
	long m_FRAPHeadType;					// Type of FRAP polymer head bead
	long m_FRAPTailType;					// Type of FRAP polymer taul bead

	// Raft analysis tool data

	zLongVector m_RaftPolymerTypes;			// Raft polymer types
	zLongVector m_RaftHeadBeadTypes;		// Raft polymer head bead types
	zLongVector m_RaftTailBeadTypes;		// Raft polymer head bead types

	bool m_bRafts[2];						// Flag showing if rafts exist in monolayers

	// Bilayer data

	long m_LipidBeadTypes;					// Number of bead types in a lipid polymer
	long m_LipidBondTypes;					// Number of bond types in a lipid polymer
	long m_LipidBondPairTypes;				// Number of bondpair types in a lipid polymer


	CSliceProfile* m_pLipidSP;				// SliceProfile for lipids

	aaScalarProfile* m_pLipidHeadDensity;	// Density of lipid heads in bilayer
	aaScalarProfile* m_pLipidTailDensity;	// Density of lipid tails in bilayer
	aaScalarProfile* m_pSolventDensity;		// Density of solvent in bilayer

	long m_LipidTotal;						// Max no of lipids in bilayer
	long m_InnerTotal;						// No of lipids in inner monolayer
	long m_OuterTotal;						// No of lipids in outer monolayer
	long m_MissingTotal;					// Lipids not in either monolayer

	PolymerList m_lLipids;					// List of lipids in bilayer
	PolymerList m_lInnerLipids;				// List of lipids with axes against bilayer normal
	PolymerList m_lOuterLipids;				// List of lipids parallel to bilayer normal

	double m_rcmHead[3];					// CM coordinates of bilayer from lipid heads
	double m_rcmTail[3];					// CM coordinates of bilayer from lipid tails
    double m_InnerHeight1;					// Average height of inner monolayer heads
    double m_InnerHeight2;					// Average height of inner monolayer tails
	double m_OuterHeight1;					// Average height of outer monolayer heads
	double m_OuterHeight2;					// Average height of outer monolayer tails
	double m_BilayerHeight1;				// Bilayer height from heads
	double m_BilayerHeight2;				// Bilayer height from tails
	double m_InnerWidth;					// Average width of inner monolayer
	double m_OuterWidth;					// Average width of outer monolayer
	double m_BilayerWidth;					// Average of both monolayers
	double m_InnerArea;						// Average area of inner monolayer
	double m_OuterArea;						// Average area of outer monolayer
	double m_InnerAN;						// A/N for lipids in inner monolayer
	double m_OuterAN;						// A/N for lipids in outer monolayer
	double m_ProjArea;						// Projected area of bilayer, i.e., box area
	double m_InnerAreaRatio;				// Ratio of inner area to projected area
	double m_OuterAreaRatio;				// Ratio of outer area to projected area
	double m_VolumePerLipid;				// Bilayer volume per lipid

	// Data relating to the grid  analysis of the bilayer shape

	double m_SliceWidth;					// Width of a slice in bead diameters
	double m_RowWidth;						// Width of a row in units of CNT cells
	double m_RowWidthSq;					// Square of the above
	double m_CellWidth;						// Width of a cell in units of CNT cells
	double m_CellWidthSq;					// Square of the above
	double m_RowOverCell;					// Ratio of row width to cell width
	double m_RowOverCellSq;					// Square of the above
	double m_RowCellTotal;					// Product of number of rows and cells

	LongPolymerMap m_mPolymerFromHeadId;		// Map taking head bead id to polymer
	LongPolymerMap m_mPolymerFromTailId;		// Map taking tail bead id to polymer

	CellProfileSetVector  m_vBilayerProfiles;	// Cell profiles spanning bilayer

	long            m_1dProfileCounter;		// Counter for calls to Thickness;
	zDoubleValArray m_aInnerHeadNo2d;		// n(x,y) for inner monolayer for head beads
	zDoubleValArray m_aOuterHeadNo2d;		// n(x,y) for outer monolayer      "
	zDoubleValArray m_aInnerHead2d;			// h(x,y) for inner monolayer      "
	zDoubleValArray m_aOuterHead2d;			// h(x,y) for outer monolayer      "

	zDoubleValArray m_aInnerTailNo2d;		// n(x,y) for inner monolayer for tail beads
	zDoubleValArray m_aOuterTailNo2d;		// n(x,y) for outer monolayer      "
	zDoubleValArray m_aInnerTail2d;			// h(x,y) for inner monolayer      "
	zDoubleValArray m_aOuterTail2d;			// h(x,y) for outer monolayer      "

	zDoubleValArray m_aInnerAN2d;			// Area per lipid for inner monolayer
	zDoubleValArray m_aOuterAN2d;			// Area per lipid for outer monolayer

	zLongValArray   m_aHeightFieldDim;		// Dimensions of height field 1d data
	zLongValArray   m_aHeightFieldInt;		// and the interval between data points

	// Data relating to the stress tensor in the fluid

	long   m_StressGridTotal;					// Size of grid used for stress profile
	double m_SurfaceTension;					// Surface tension across bilayer

	aaStressTensor1d* m_pStress1d;				// Stress tensor profile of bilayer
	aaScalarProfile*  m_pTotalStress;			// Stress profile for all interactions
	aaScalarProfile*  m_pWWStress;				// Stress profile for W-W interactions
	aaScalarProfile*  m_pLipidHeadWStress;		// Stress profile for H-W interactions
	aaScalarProfile*  m_pLipidTailWStress;		// Stress profile for T-W interactions
	aaScalarProfile*  m_pLipidHeadHeadStress;	// Stress profile for H-H interactions
	aaScalarProfile*  m_pLipidHeadTailStress;	// Stress profile for H-T interactions
	aaScalarProfile*  m_pLipidTailTailStress;	// Stress profile for T-T interactions

	ScalarProfileSequence	m_vBondStress;		// Bond stress profiles
	ScalarProfileSequence	m_vBondPairStress;	// 3-body bondpair stress profiles

	zLongVector		  m_vBeadTypeIndex;			// Bead types in lipid polymer
	zLongVector		  m_vBondTypeIndex;			// Bond types in lipid polymer
	zLongVector		  m_vBondPairTypeIndex;		// Bondpair types in lipid polymer

	zLongVector		  m_vBeadStressIndex;		// Bead-Bead index for stress tensor
	zLongVector		  m_vBondStressIndex;		// Bond index for stress tensor
	zLongVector		  m_vBondPairStressIndex;	// Bondpair index for stress tensor

};


#endif // !defined(AFX_COMPOSITEBILAYER_H__94E4661A_1107_4CE1_9D44_784B3D55E032__INCLUDED_)
