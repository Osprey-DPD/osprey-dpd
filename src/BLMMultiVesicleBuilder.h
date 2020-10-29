// BLMMultiVesicleBuilder.h: interface for the CBLMMultiVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLMMULTIVESICLEBUILDER_H__0BD72581_E477_4EC4_8BE0_80856AC919E0__INCLUDED_)
#define AFX_BLMMULTIVESICLEBUILDER_H__0BD72581_E477_4EC4_8BE0_80856AC919E0__INCLUDED_


#include "Builder.h"

class CBLMMultiVesicleBuilder : private CBuilder  
{
public:
	CBLMMultiVesicleBuilder(const zLongVector&				blmPolymerTypes,
					   const zLongVector&				blmReservoirTypes,
					   const zDoubleVector&				blmUpperFractions,
					   const PolymerCrossLinkVector&	blmCrossLinks,
					   long blmx, long blmy, long blmz,
					   double blmCentre, double blmThickness,
					   bool bblmLinear, const bool bblmPatches[2], bool bblmPolymerise,
					   double area, double l1, double l2, double bilayerCentre, 
					   double upperHead, double lowerHead,
					   const zLongVector&				vPolymerTypes,
					   const zLongVector&				vInteriorTypes,
					   const zLongVector&				vExteriorTypes,
					   const zDoubleVector&				vOuterFractions,
					   const PolymerCrossLinkVector&	vCrossLinks,
					   const zDoubleVector& vxc, const zDoubleVector& vyc, const zDoubleVector& vzc,
					   const zDoubleVector& vxcentre, const zDoubleVector& vycentre, const zDoubleVector& vzcentre,
					   double vOuterRadius, double vInnerRadius, double vThickness,
					   const bool bvPatches[2], bool bvPolymerise);

	virtual ~CBLMMultiVesicleBuilder();

	virtual bool Assemble(CInitialState& riState);

private:

	// Data required for the BLM bilayer

	const	zLongVector&			m_BLMPolymerTypes;
	const	zLongVector&			m_BLMReservoirTypes;
	const	zDoubleVector&			m_BLMUpperFractions;
	const PolymerCrossLinkVector	m_BLMCrossLinks;
	long	m_BLMX;
	long	m_BLMY;
	long	m_BLMZ;
	double	m_BLMCentre;
	double	m_BLMThickness;
	bool	m_bBLMLinearise;
	bool    m_bBLMPatches[2];	
	bool	m_bBLMPolymerise;

	double	m_BilayerArea;
	double	m_Bilayer1Length;
	double	m_Bilayer2Length;
	double	m_BilayerCentre;
	double	m_BilayerUpperHead;
	double	m_BilayerLowerHead;

	// Local data for the bilayer

	bool	m_bBLMMonolayer;		// Flag showing whether all polymers are in one monolayer
	long	m_BLMPolymerTotal;		// Total number of all polymers in bilayer
	long	m_BLMUpperTotal;		// Total number of polymers in upper monolayer
	long	m_BLMLowerTotal;		// Total number of polymers in lower monolayer

	// Data required for the vesicle

	const zLongVector&				m_VPolymerTypes;		// Types of polymer composing vesicle
	const zLongVector&				m_VInteriorTypes;		// Types of polymer in vesicle interior
	const zLongVector&				m_VExteriorTypes;		// Types of polymer in vesicle interior
	const zDoubleVector&			m_VOuterFractions;		// Fractions of polymers in outer monolayer
	const PolymerCrossLinkVector&	m_VCrossLinks;			// Polymerisation data 
	const zDoubleVector&			m_VXC;					// Vesicle centres as fraction of SimBox thickness
	const zDoubleVector&			m_VYC;
	const zDoubleVector&			m_VZC;
	const zDoubleVector&			m_VesicleXCentre;		// Vesicle centres in units of r0
	const zDoubleVector&			m_VesicleYCentre;
	const zDoubleVector&			m_VesicleZCentre;
	double							m_VOuterRadius;			// Vesicle outer monolayer head radius in units of bead diameters
	double							m_VInnerRadius;			// Vesicle inner monolayer head radius in units of bead diameters
	double							m_VThickness;			// Vesicle bilayer thickness in units of bead radius
	bool							m_bVPatches[2];			// Distribute polymers randomly (0) or in patches (1)
	bool							m_bVPolymerise;			// Flag showing if vesicle is polymerised

};

#endif // !defined(AFX_BLMMULTIVESICLEBUILDER_H__0BD72581_E477_4EC4_8BE0_80856AC919E0__INCLUDED_)
