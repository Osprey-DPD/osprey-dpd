// InitialStatePEP.h: interface for the CInitialStatePEP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEPEP_H__C7326B90_D0A2_4643_8A33_5A2E4DD6DEBE__INCLUDED_)
#define AFX_INITIALSTATEPEP_H__C7326B90_D0A2_4643_8A33_5A2E4DD6DEBE__INCLUDED_



#include "InitialStateData.h"

class CInitialStatePEP : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStatePEP();

	CInitialStatePEP& operator=(const CInitialStatePEP& oldISL);

	virtual ~CInitialStatePEP();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static const zString m_Type;	// Keyword used to identify initial state type

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
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

	// Data relating to the particles

	bool			m_bRandomlyOriented;		// Flag showing random/ordered orientation
	long			m_ParticleNo;				// Number of particles
	double			m_MinZFraction;				// Lower edge of particle zone
	double			m_MaxZFraction;				// Upper edge of particle zone
	double			m_SMA;
	double			m_SMB;						// Semi-axes of particles
	double			m_SMC;
	double			m_SpringConstant;			// Hookean spring constant for polymerisation bonds
	double			m_UnstretchedLength;		// Unstretched length for polymerisation bonds
	double			m_Theta;					// Polar angle for oriented particles
	double			m_Phi;						// Azimuthal angle for oriented particles

	// Data relating to patches on the particles: note that all particles have the 
	// same number and type of patches. The patch Z coordinates are specified as 
	// fractions of the particles' long axis in the range [0,1].

	bool			m_bPatches;					// Flag showing if patches are present
	long			m_PatchNo;					// Number of patches per particle
	zDoubleVector	m_PatchZC;					// Z coordinate of patches in particle coordinate system
	zDoubleVector	m_PatchRadius;				// Radii of patches
	zDoubleVector   m_PatchPsi;					// Azimuthal angle of patches in particle coordinate system

	// Data relating to the "hard" surface 

	bool            m_bSurface;					// Flag showing if surface is present
	zString		    m_BLMPolymer;				// Polymer type composing surface
	long			m_BLMX;						
	long			m_BLMY;						// Surface normal: only x, y or z allowed
	long			m_BLMZ;
	double			m_BLMCentre;				// Surface centre as fraction of SimBox size
	double			m_BLMThickness;				// In units of bead diameters

	// Data relating to a polymerised surface
	bool					m_bBLMPolymerise;	// Flag showing if surface is polymerised or frozen
	PolymerCrossLinkVector	m_BLMCrossLinks;	// Vector of cross-linker pointers holding data


	// Local data

	long		m_BLMPolymerType;				// Type of polymer composing blm
	double		m_BilayerArea;					// Area per monolayer
	double		m_Bilayer1Length;				// Inplane size of blm, 
	double		m_Bilayer2Length;				// NOT necessarily x,y coordinates
	double		m_BilayerCentre;				// Bilayer midplane position (depends on orientation)
	double		m_UpperHead;					// Upper monolayer head beads position
	double		m_LowerHead;					// Lower    "

};

#endif // !defined(AFX_INITIALSTATEPEP_H__C7326B90_D0A2_4643_8A33_5A2E4DD6DEBE__INCLUDED_)
