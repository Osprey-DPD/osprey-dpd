// FusionLamellaBuilder.h: interface for the CFusionLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSIONLAMELLABUILDER_H__2E32FD87_2D5A_11D4_BF36_004095086186__INCLUDED_)
#define AFX_FUSIONLAMELLABUILDER_H__2E32FD87_2D5A_11D4_BF36_004095086186__INCLUDED_


#include "Builder.h"

class CFusionLamellaBuilder : private CBuilder  
{
public:
	CFusionLamellaBuilder(zLongVector polymerTypes1, zLongVector polymerTypes2,
					long x, long y, long z,  
					const double centre[2], const double thickness[2], 
					bool bLinear, const zDoubleVector upperFractions, double area, 
					double l1, double l2, const double bilayercentre[2], 
					const double lowerHead[2], const double upperHead[2]);

	virtual ~CFusionLamellaBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zLongVector m_PolymerTypes1;			// Lower bilayer in SimBox
	zLongVector m_PolymerTypes2;			// Upper bilayer in SimBox
	long m_X;
	long m_Y;
	long m_Z;
	double m_Centre[2];						// as fraction of SimBox length
	double m_Thickness[2];					// in units of bead diameters
	bool   m_bLinearise;					// Flag showing whether to linearise the polymers
	bool   m_bPatches[2];					// Unused flag relating to patches of polymers
	const zDoubleVector m_UpperFractions;	// Fractions of polymers in each monolayer

	
	double m_BilayerArea;
	double m_Bilayer1Length;
	double m_Bilayer2Length;
	double m_BilayerCentre[2];		// in units of bead diameters
	double m_UpperHead[2];			// in units of bead diameters
	double m_LowerHead[2];			// in units of bead diameters

	// Local data 

	bool   m_bMonolayer[2];			// Flags showing whether all polymers are in one monolayer
	long   m_PolymerTotal[2];		// Total number of all polymers in each bilayer
	long   m_UpperTotal[2];			// Number of polymers in upper monolayers
	long   m_LowerTotal[2];			// Number of polymers in lower monolayers

};

#endif // !defined(AFX_FUSIONLAMELLABUILDER_H__2E32FD87_2D5A_11D4_BF36_004095086186__INCLUDED_)
