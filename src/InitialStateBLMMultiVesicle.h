// InitialStateBLMMultiVesicle.h: interface for the CInitialStateBLMMultiVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEBLMMULTIVESICLE_H__DCA81AB9_93A8_406C_AF40_06059E92FE64__INCLUDED_)
#define AFX_INITIALSTATEBLMMULTIVESICLE_H__DCA81AB9_93A8_406C_AF40_06059E92FE64__INCLUDED_


#include "InitialStateData.h"

class CInitialStateBLMMultiVesicle : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction
public:

	CInitialStateBLMMultiVesicle();
	virtual ~CInitialStateBLMMultiVesicle();

	CInitialStateBLMMultiVesicle& operator=(const CInitialStateBLMMultiVesicle& oldISL);


	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static const zString m_Type;			// Keyword used to identify initial state type

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxCommand pointer,
	// respond to the SimBox's request to execute and return the name of the command,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;


	virtual bool	Assemble(CInitialState& riState) const;

	virtual CInitialStateData* Copy() const;

	// ****************************************
	// Public access functions
public:



	// ****************************************
	// Protected local functions
protected:

	virtual bool ValidateData(const CInputData& riData);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

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
	zDoubleVector	m_VXC;						// Vesicle centres as fraction of SimBox size
	zDoubleVector	m_VYC;
	zDoubleVector	m_VZC;
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


	zLongVector		m_VPolymerTypes;			// Types of polymer composing vesicle
	zLongVector		m_VInteriorTypes;			// Types of polymer inside vesicle
	zLongVector		m_VExteriorTypes;			// Types of polymer inside vesicle
	double			m_VInnerRadius;				// Vesicle inner monolayer head radius (units of r0)
	zDoubleVector	m_VXCentre;					// Vesicle centre coordinates (in units of r0)					
	zDoubleVector	m_VYCentre;
	zDoubleVector	m_VZCentre;

};

#endif // !defined(AFX_INITIALSTATEBLMMULTIVESICLE_H__DCA81AB9_93A8_406C_AF40_06059E92FE64__INCLUDED_)
