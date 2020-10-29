// FreeLamellaBuilder.h: interface for the CFreeLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FREELAMELLABUILDER_H__714FC15B_DAA9_40ED_8668_3760A4590169__INCLUDED_)
#define AFX_FREELAMELLABUILDER_H__714FC15B_DAA9_40ED_8668_3760A4590169__INCLUDED_


#include "Builder.h"

class CFreeLamellaBuilder : private CBuilder  
{
public:
	CFreeLamellaBuilder(const zLongVector& polymerTypes, 
						 const zDoubleVector& upperFractions,
						 const PolymerCrossLinkVector& crossLinks,
						 const bool bPatches[2], bool bPolymerise, bool bLinear,
						 long x, long y, long z, const double centre[3], 
						 double thickness, double area, double l1, double l2, 
						 double bilayer1centre, double bilayer2centre, double bilayer3centre, 
						 double upperHead, double lowerHead);

	virtual ~CFreeLamellaBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	const	zLongVector&			m_PolymerTypes;
	const	zDoubleVector&			m_UpperFractions;
	const PolymerCrossLinkVector	m_CrossLinks;
	bool    m_bPatches[2];
	bool	m_bPolymerise;
	bool	m_bLinearise;
	long	m_X;
	long	m_Y;
	long	m_Z;
	double	m_Centre[3];	// in units of bead diameters
	double	m_Thickness;	//      "        "      "
	double	m_BilayerArea;
	double	m_Bilayer1Length;
	double	m_Bilayer2Length;
	double	m_Bilayer1Centre;
	double	m_Bilayer2Centre;
	double	m_Bilayer3Centre;
	double	m_UpperHead;
	double	m_LowerHead;

	// Local data

	bool	m_bMonolayer;			// Flag showing whether all polymers are in one monolayer
	long	m_PolymerTotal;			// Total number of all polymers in bilayer
	long	m_UpperTotal;			// Total number of polymers in upper monolayer
	long	m_LowerTotal;			// Total number of polymers in lower monolayer

};

#endif // !defined(AFX_FREELAMELLABUILDER_H__714FC15B_DAA9_40ED_8668_3760A4590169__INCLUDED_)
