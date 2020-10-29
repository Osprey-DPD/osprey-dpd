// PEPBuilder.h: interface for the CPEPBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PEPBUILDER_H__4ED9B39C_9E56_4CCA_B674_F44A17DF8307__INCLUDED_)
#define AFX_PEPBUILDER_H__4ED9B39C_9E56_4CCA_B674_F44A17DF8307__INCLUDED_



#include "Builder.h"

class CPEPBuilder : private CBuilder  
{
	// ****************************************
	// Construction/Destruction:
public:

	CPEPBuilder(const zLongVector&				blmPolymerTypes,
					   const zLongVector&				blmReservoirTypes,
					   const zDoubleVector&				blmUpperFractions,
					   const PolymerCrossLinkVector&	blmCrossLinks,
					   long blmx, long blmy, long blmz,
					   double blmCentre, double blmThickness,
					   bool bblmLinear, const bool bblmPatches[2], bool bblmPolymerise,
					   double area, double l1, double l2, double bilayerCentre, 
					   double upperHead, double lowerHead);

	virtual ~CPEPBuilder();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	virtual bool Assemble(CInitialState& riState);

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
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

};

#endif // !defined(AFX_PEPBUILDER_H__4ED9B39C_9E56_4CCA_B674_F44A17DF8307__INCLUDED_)
