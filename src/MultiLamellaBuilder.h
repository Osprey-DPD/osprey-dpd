// MultiLamellaBuilder.h: interface for the CMultiLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTILAMELLABUILDER_H__2E32FD88_2D5A_11D4_BF36_004095086186__INCLUDED_)
#define AFX_MULTILAMELLABUILDER_H__2E32FD88_2D5A_11D4_BF36_004095086186__INCLUDED_


#include "Builder.h"

class CMultiLamellaBuilder : private CBuilder  
{
public:
	CMultiLamellaBuilder(const zLongVector polymerTypes, long x, long y, long z,  
						 const zDoubleVector centres, const zDoubleVector upperFractions, 
						 bool bLinear, double thickness, double area, double l1, double l2);

	virtual ~CMultiLamellaBuilder();

	virtual bool Assemble(CInitialState& riState);

private:

	const zLongVector	m_PolymerTypes;		// Polymers making up lamellae
	const long			m_X;
	const long			m_Y;				// Normal to lamellae
	const long			m_Z;
	const zDoubleVector	m_BilayerCentres;	// Coordinates of lamellae centres
	const zDoubleVector	m_UpperFractions;	// Polymer fractions in upper monolayers
	const bool			m_bLinearise;		// Are polymers linearised?
	const double		m_Thickness;		
	const double		m_BilayerArea;		
	const double		m_Bilayer1Length;
	const double		m_Bilayer2Length;

	// Local data

	const long			m_LamellaTotal;		// Number of lamellae
	zLongVector			m_LowerTotals;		// Number of polymers in each lower monolayer
	zLongVector			m_UpperTotals;		//   "          "             upper     "
};

#endif // !defined(AFX_MULTILAMELLABUILDER_H__2E32FD88_2D5A_11D4_BF36_004095086186__INCLUDED_)
