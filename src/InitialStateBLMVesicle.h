// InitialStateBLMVesicle.h: interface for the CInitialStateBLMVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEBLMVESICLE_H__4C9F8B1D_098F_424D_8271_590968C8DE27__INCLUDED_)
#define AFX_INITIALSTATEBLMVESICLE_H__4C9F8B1D_098F_424D_8271_590968C8DE27__INCLUDED_


#include "InitialStateData.h"

class CInitialStateBLMVesicle : public CInitialStateData  
{
public:
	CInitialStateBLMVesicle();
	virtual ~CInitialStateBLMVesicle();

	CInitialStateBLMVesicle& operator=(const CInitialStateBLMVesicle& oldISL);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	static const zString GetType();

protected:
	virtual bool ValidateData(const CInputData& riData);

private:
	static const zString m_Type;			// Keyword used to identify initial state type

	// Data relating to the planar blm

	StringSequence  m_BLMPolymers;				// Polymers composing bilayer
	StringSequence  m_BLMReservoir;				// Polymers in reservoir behind bilayer
	long			m_BLMX;						
	long			m_BLMY;						// Bilayer normal: only x, y or z allowed
	long			m_BLMZ;
	double			m_BLMCentre;				// Bilayer centre as fraction of SimBox size
	double			m_BLMThickness;				// In units of bead diameters
	bool			m_bBLMLinearise;			// Flag showing if polymers  initially straight
	zDoubleVector	m_BLMUpperFractions;		// Fractions of polymers in upper monolayer
	bool			m_bBLMPatches[2];			// Arrange polymers in patches: 0/1

	// Data relating to a polymerised blm initial state
	bool					m_bBLMPolymerise;	// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_BLMCrossLinks;	// Vector of cross-linker pointers holding data

	// Data relating to the vesicle

	StringSequence	m_VPolymers;				// Polymers composing vesicle
	StringSequence	m_VInteriorPolymers;		// Polymers in interior of vesicle
	StringSequence	m_VExteriorPolymers;		// Polymers in exterior of vesicle
	double			m_VXC;						// Vesicle centre as fraction of SimBox size
	double			m_VYC;
	double			m_VZC;
	double			m_VOuterRadius;				// Vesicle outer monolayer head radius (units of r0)
	double			m_VThickness;				// Vesicle bilayer thickness (units of r0)
	zDoubleVector	m_VOuterFractions;			// Fractions of polymers in outer monolayer
	bool			m_bVPatches[2];				// Arrange polymers in patches: 0/1

	// Data relating to a polymerised vesicle initial state
	bool					m_bVPolymerise;		// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_VCrossLinks;		// Vector of cross-linker pointers holding data



	// Local data

	zLongVector m_BLMPolymerTypes;				// Types of polymer composing blm
	zLongVector m_BLMReservoirTypes;			// Types of polymer in blm reservoir
	double		m_BilayerArea;					// Area per monolayer
	double		m_Bilayer1Length;				// Inplane size of blm, 
	double		m_Bilayer2Length;				// NOT necessarily x,y coordinates
	double		m_BilayerCentre;				// Bilayer midplane position (depends on orientation)
	double		m_UpperHead;					// Upper monolayer head beads position
	double		m_LowerHead;					// Lower    "


	zLongVector m_VPolymerTypes;				// Types of polymer composing vesicle
	zLongVector m_VInteriorTypes;				// Types of polymer inside vesicle
	zLongVector m_VExteriorTypes;				// Types of polymer inside vesicle
	double		m_VInnerRadius;					// Vesicle inner monolayer head radius (units of r0)
	double		m_VXCentre;						// Vesicle centre coordinates (in units of r0)					
	double		m_VYCentre;
	double		m_VZCentre;

};

#endif // !defined(AFX_INITIALSTATEBLMVESICLE_H__4C9F8B1D_098F_424D_8271_590968C8DE27__INCLUDED_)
