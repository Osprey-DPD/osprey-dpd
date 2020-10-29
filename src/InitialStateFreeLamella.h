// InitialStateFreeLamella.h: interface for the CInitialStateFreeLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEFREELAMELLA_H__8F1EF863_E836_4697_8EF0_1288DC8C5D34__INCLUDED_)
#define AFX_INITIALSTATEFREELAMELLA_H__8F1EF863_E836_4697_8EF0_1288DC8C5D34__INCLUDED_


#include "InitialStateData.h"

class CInitialStateFreeLamella : public CInitialStateData  
{
public:
	CInitialStateFreeLamella();
	virtual ~CInitialStateFreeLamella();

	CInitialStateFreeLamella& operator=(const CInitialStateFreeLamella& oldISL);

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

	StringSequence m_Polymers;				// Polymers composing bilayer
	long	m_X;						
	long	m_Y;							// Bilayer normal: only x, y or z allowed
	long	m_Z;
	double	m_Centre[3];					// Bilayer centre as fraction of SimBox side lengths
	double	m_Thickness;					// Bilayer thickness in units of bead diameters
	double	m_Length;						// Bilayer length in units of bead diameters
	double	m_Width;						// Bilayer width in units of bead diameters
	bool	m_bLinearise;					// Flag showing whether polymer is initially straightened
	zDoubleVector	m_UpperFractions;		// Fractions of polymers in upper monolayer
	bool			m_bPatches[2];			// Arrange polymers in patches: 0/1

	// Data relating to a polymerised initial state
	bool					m_bPolymerise;	// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_CrossLinks;	// Vector of cross-linker pointers holding data

	// Local data

	zLongVector m_PolymerTypes;			// Types of polymer composing lamella
	double		m_BilayerArea;			// Area per monolayer
	double		m_Bilayer1Length;		// Inplane size of bilayer: NOT necessarily x,y coordinates
	double		m_Bilayer2Length;
	double		m_Bilayer1Centre;		// Coordinate of bilayer X midplane (depends on orientation)
	double		m_Bilayer2Centre;		// Coordinate of bilayer Y midplane (depends on orientation)
	double		m_Bilayer3Centre;		// Coordinate of bilayer Z midplane (depends on orientation)
	double		m_UpperHead;			// Coordinate of upper monolayer head beads
	double		m_LowerHead;			// Coordinate of lower monolayer head beads

};

#endif // !defined(AFX_INITIALSTATEFREELAMELLA_H__8F1EF863_E836_4697_8EF0_1288DC8C5D34__INCLUDED_)
