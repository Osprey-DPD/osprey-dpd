// BLMVesicleBuilder.h: interface for the CBLMVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLMVESICLEBUILDER_H__B30395C4_CBC1_4E6F_990E_8F553DE605F2__INCLUDED_)
#define AFX_BLMVESICLEBUILDER_H__B30395C4_CBC1_4E6F_990E_8F553DE605F2__INCLUDED_


#include "Builder.h"

class CBLMVesicleBuilder : private CBuilder  
{
public:
	CBLMVesicleBuilder(const zLongVector&				blmPolymerTypes,
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
					   double vxc, double vyc, double vzc,
					   double vxcentre, double vycentre, double vzcentre,
					   double vOuterRadius, double vInnerRadius, double vThickness,
					   const bool bvPatches[2], bool bvPolymerise);

	virtual ~CBLMVesicleBuilder();

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
	double		m_VXC;				// Vesicle centre as fraction of SimBox thickness
	double		m_VYC;
	double		m_VZC;
	double		m_VesicleXCentre;	// Vesicle centre in units of r0
	double		m_VesicleYCentre;
	double		m_VesicleZCentre;
	double		m_VOuterRadius;		// Vesicle outer monolayer head radius in units of bead diameters
	double		m_VInnerRadius;		// Vesicle inner monolayer head radius in units of bead diameters
	double		m_VThickness;		// Vesicle bilayer thickness in units of bead radius
	bool		m_bVPatches[2];		// Distribute polymers randomly (0) or in patches (1)
	bool		m_bVPolymerise;		// Flag showing if vesicle is polymerised

};

#endif // !defined(AFX_BLMVESICLEBUILDER_H__B30395C4_CBC1_4E6F_990E_8F553DE605F2__INCLUDED_)
