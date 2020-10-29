// CompositeLamellaBuilder.h: interface for the CCompositeLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPOSITELAMELLABUILDER_H__2E32FD86_2D5A_11D4_BF36_004095086186__INCLUDED_)
#define AFX_COMPOSITELAMELLABUILDER_H__2E32FD86_2D5A_11D4_BF36_004095086186__INCLUDED_



#include "Builder.h"

class CCompositeLamellaBuilder : private CBuilder  
{
public:
	CCompositeLamellaBuilder(const zLongVector& polymerTypes, 
							 const zDoubleVector& upperFractions,
							 const PolymerCrossLinkVector& crossLinks,
							 long x, long y, long z, double centre, double thickness, 
							 const bool bPatches[2], bool bPolymerise, 
							 double area, double l1, double l2, double bilayercentre, 
							 double upperHead, double lowerHead, bool bLinear);

	virtual ~CCompositeLamellaBuilder();

	virtual bool Assemble(CInitialState& riState);
	virtual bool AssembleP(CInitialState& riState);


protected:

    bool    AssembleRandomP(CInitialState& riState, const zDoubleVector exRegions);

private:

	const	zLongVector&			m_PolymerTypes;
	const	zDoubleVector&			m_UpperFractions;
	const PolymerCrossLinkVector	m_CrossLinks;
	long	m_X;
	long	m_Y;
	long	m_Z;
	double	m_Centre;
	double	m_Thickness;
	bool	m_bLinearise;
	bool    m_bPatches[2];
	
	// Data relating to a polymerised initial state
	bool	m_bPolymerise;

	// Local data
	double	m_BilayerArea;
	double	m_Bilayer1Length;
	double	m_Bilayer2Length;
	double	m_BilayerCentre;
	double	m_UpperHead;
	double	m_LowerHead;

	// Local data

	bool	m_bMonolayer;			// Flag showing whether all polymers are in one monolayer
	long	m_PolymerTotal;			// Total number of all polymers in bilayer
	long	m_UpperTotal;			// Total number of polymers in upper monolayer
	long	m_LowerTotal;			// Total number of polymers in lower monolayer

};

#endif // !defined(AFX_COMPOSITELAMELLABUILDER_H__2E32FD86_2D5A_11D4_BF36_004095086186__INCLUDED_)
