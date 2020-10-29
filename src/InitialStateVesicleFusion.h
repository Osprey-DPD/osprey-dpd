// InitialStateVesicleFusion.h: interface for the CInitialStateVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEVESICLEFUSION_H__C26523BB_76D0_424D_9D3F_ED6110FC3347__INCLUDED_)
#define AFX_INITIALSTATEVESICLEFUSION_H__C26523BB_76D0_424D_9D3F_ED6110FC3347__INCLUDED_


#include "InitialStateData.h"

class CInitialStateVesicleFusion : public CInitialStateData  
{
public:
	CInitialStateVesicleFusion();
	virtual ~CInitialStateVesicleFusion();

	CInitialStateVesicleFusion& operator=(const CInitialStateVesicleFusion& oldISV);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	static const zString GetType();

protected:
	virtual bool ValidateData(const CInputData& riData);

private:
	static const zString m_Type;		// Keyword used to identify initial state type

	StringSequence m_Polymers1;			// Polymers composing first vesicle
	StringSequence m_Polymers2;			// Polymers composing second vesicle
	double	m_XC[2];
	double	m_YC[2];					// Vesicle centres as fractions of SimBox thickness
	double	m_ZC[2];
	double	m_OuterRadius[2];			// Vesicle outer monolayer head radii in units of bead diameter
	double	m_Thickness[2];				// Vesicle bilayer thicknesses in units of bead diameter
	zDoubleVector	m_OuterFractions1;	// Fractions of polymers in outer monolayer
	zDoubleVector	m_OuterFractions2;	// Fractions of polymers in outer monolayer
	bool	m_bPatches1[2];				// Arrange polymers in patches for first vesicle: 0/1
	bool	m_bPatches2[2];				// Arrange polymers in patches for second vesicle

	// Local data

	zLongVector   m_PolymerTypes1;		// Types of polymer composing first vesicle
	zLongVector   m_PolymerTypes2;		// Types of polymer composing second vesicle

	double	m_InnerRadius[2];			// Vesicle inner monolayer head radii in units of bead diameter

};

#endif // !defined(AFX_INITIALSTATEVESICLEFUSION_H__C26523BB_76D0_424D_9D3F_ED6110FC3347__INCLUDED_)
