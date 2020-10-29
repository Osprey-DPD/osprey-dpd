// FusionPoreBuilder.h: interface for the CFusionPoreBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSIONPOREBUILDER_H__4D165FD4_BF9A_11D4_BF58_004095086186__INCLUDED_)
#define AFX_FUSIONPOREBUILDER_H__4D165FD4_BF9A_11D4_BF58_004095086186__INCLUDED_


#include "Builder.h"

class CFusionPoreBuilder : private CBuilder  
{
public:
	CFusionPoreBuilder(zLongVector polymerTypes1, zLongVector polymerTypes2,
					long x, long y, long z, const double centre[2], const double thickness[2], 
					bool bLinear, zDoubleVector upperFractions,
					double area, double l1, double l2, const double bilayercentre[2], 
					const double lowerHead[2], const double upperHead[2]);
	virtual ~CFusionPoreBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zLongVector m_PolymerTypes1;
	zLongVector m_PolymerTypes2;
	long m_X;
	long m_Y;
	long m_Z;
	double m_Centre[2];				// as fraction of SimBox length
	double m_Thickness[2];			// in units of bead diameters
	bool   m_bLinearise;
	zDoubleVector m_UpperFractions;
	double m_BilayerArea;
	double m_Bilayer1Length;
	double m_Bilayer2Length;
	double m_BilayerCentre[2];		// in units of bead diameters
	double m_LowerHead[2];			// in units of bead diameters
	double m_UpperHead[2];			// in units of bead diameters

};

#endif // !defined(AFX_FUSIONPOREBUILDER_H__4D165FD4_BF9A_11D4_BF58_004095086186__INCLUDED_)
