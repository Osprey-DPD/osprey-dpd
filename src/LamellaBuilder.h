// LamellaBuilder.h: interface for the CLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAMELLABUILDER_H__6CE669F5_04B5_11D4_BF30_004095086186__INCLUDED_)
#define AFX_LAMELLABUILDER_H__6CE669F5_04B5_11D4_BF30_004095086186__INCLUDED_



#include "Builder.h"

class CLamellaBuilder : private CBuilder  
{
public:

	CLamellaBuilder(long polymer, long x, long y, long z,  
					double centre, double thickness, bool blinear, double upperFraction,
					bool bPolymerise, long polyType, double polyFraction, long polyPosition,
					double springConstant, double unstretchedLength,
					double area, double l1, double l2, double bilayercentre, 
					double lowerHead, double upperHead);
					
	virtual ~CLamellaBuilder();

	virtual bool Assemble(CInitialState& riState);
	virtual bool AssembleP(CInitialState& riState);
	
	
protected:

    bool    AssembleRandomP(CInitialState& riState, const zDoubleVector exRegions);

private:

	long	m_PolymerType;
	long	m_X;
	long	m_Y;
	long	m_Z;
	double	m_Centre;
	double	m_Thickness;
	bool	m_bLinearise;
	double	m_UpperFraction;
	bool	m_bPolymerise;
	long	m_PolymerisedType;
	double	m_PolymerisedFraction;
	long	m_PolymerisedPosition;
	double	m_SpringConstant;
	double	m_UnstretchedLength;


	double m_BilayerArea;
	double m_Bilayer1Length;
	double m_Bilayer2Length;
	double m_BilayerCentre;
	double m_LowerHead;
	double m_UpperHead;

	// Local data relating to construction of the bilayer
	
	long	m_PolymerTotal;			// No of polymers in bilayer
	long	m_PolymerSize;			// No of beads per polymer
	double  m_LowerFreeLength;		// Distance between wall and lower bilayer surface
	double  m_UpperFreeLength;		// Distance between wall and upper bilayer surface

	bool	m_bMonolayer;			// Flag showing whether all polymers are in one monolayer
	long	m_MonolayerTotal[2];	// Number of polymers in upper and lower monolayers
	long	m_MonolayerFirstPolymer[2];	// Id of first polymer in each monolayer
	long	m_LatticeSize[2];		// No of vertices in monolayer lattices
	double  m_MonolayerSign[2];		// -1/+1 for upper/lower monolayer
	double  m_MonolayerHead[2];		// -1/+1 for upper/lower monolayer
	double	m_HexSpace[2];			// Hexagonal lattice spacings
	double	m_HexCAxisSpace[2];		// Bead spacing in lattice normal direction
	double	m_Hexe1X[2];
	double	m_Hexe1Y[2];
	double	m_Hexe2X[2];			// Components of hexagonal lattice vectors
	double	m_Hexe2Y[2];


	PolymerVector	m_vBilayer;						// Polymers in bilayer
	PolymerVector	m_vPolymerisedUpperPolymers;	// Index to upper monolayer polymer pointers
	PolymerVector	m_vPolymerisedLowerPolymers;	// Index to lower monolayer polymer pointers

	BondVector		m_vPolymerisedBonds;
};

#endif // !defined(AFX_LAMELLABUILDER_H__6CE669F5_04B5_11D4_BF30_004095086186__INCLUDED_)
