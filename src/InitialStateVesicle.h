// InitialStateVesicle.h: interface for the CInitialStateVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEVESICLE_H__EC62FB37_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_INITIALSTATEVESICLE_H__EC62FB37_3C4F_11D4_BF3C_004095086186__INCLUDED_




#include "InitialStateData.h"

class CInitialStateVesicle : public CInitialStateData  
{
public:
	CInitialStateVesicle();
	virtual ~CInitialStateVesicle();

	CInitialStateVesicle& operator=(const CInitialStateVesicle& oldISV);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	void SetData(const mpmInitialState *const pMsg);

	static const zString GetType();
	
	// Public access functions to the IS data
	
    inline const zString GetPolymer()               const {return m_Polymers.at(0);}
    inline const zString GetInteriorPolymer()       const {return m_InteriorPolymers.at(0);}
    inline double        GetXCentre()               const {return m_XC;}
    inline double        GetYCentre()               const {return m_YC;}
    inline double        GetZCentre()               const {return m_ZC;}
    inline double        GetOuterRadius()           const {return m_OuterRadius;}
    inline double        GetThickness()             const {return m_Thickness;}
    inline double        GetOuterFraction()         const {return m_OuterFractions.at(0);}
    inline  bool         GetOuterPatches()          const {return m_bPatches[0];}
    inline  bool         GetInnerPatches()          const {return m_bPatches[1];}



protected:
	virtual bool ValidateData(const CInputData& riData);

private:
	static const zString m_Type;		// Keyword used to identify initial state type

	StringSequence	m_Polymers;		// Polymers composing vesicle
	StringSequence	m_InteriorPolymers;	// Polymers in interior of vesicle
	double		m_XC;			// Vesicle centre as fraction of SimBox thickness
	double		m_YC;
	double		m_ZC;
	double		m_OuterRadius;		// Vesicle outer monolayer head radius in units of bead diameter
	double		m_Thickness;		// Vesicle bilayer thickness in units of bead diameter
	zDoubleVector	m_OuterFractions;	// Fractions of polymers in outer monolayer
	bool		m_bPatches[2];		// Arrange polymers in patches: 0/1

	// Data relating to a polymerised initial state
	bool		m_bPolymerise;	// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_CrossLinks;	// Vector of cross-linker pointers holding data

	// Local data

	zLongVector   m_PolymerTypes;		// Types of polymer composing vesicle
	zLongVector   m_InteriorPolymerTypes;	// Types of polymer inside vesicle

	double	m_InnerRadius;			// Vesicle inner monolayer head radius in units of bead diameter


};

#endif // !defined(AFX_INITIALSTATEVESICLE_H__EC62FB37_3C4F_11D4_BF3C_004095086186__INCLUDED_)
