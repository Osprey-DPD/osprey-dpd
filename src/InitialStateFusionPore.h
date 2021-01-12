// InitialStateFusionPore.h: interface for the CInitialStateFusionPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEFUSIONPORE_H__4D165FD3_BF9A_11D4_BF58_004095086186__INCLUDED_)
#define AFX_INITIALSTATEFUSIONPORE_H__4D165FD3_BF9A_11D4_BF58_004095086186__INCLUDED_


#include "InitialStateData.h"

class CInitialStateFusionPore : public CInitialStateData  
{
public:
	CInitialStateFusionPore();
	virtual ~CInitialStateFusionPore();

	CInitialStateFusionPore& operator=(const CInitialStateFusionPore& oldISL);

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

	StringSequence m_Polymers1;		// Polymers composing lower bilayer
	StringSequence m_Polymers2;		// Polymers composing upper bilayer
	long	       m_X;						
	long	       m_Y;			// Bilayer normal: only x, y or z allowed
	long	       m_Z;
	double	       m_Centre[2];		// Bilayer centres as fraction of SimBox thickness
	double	       m_Thickness[2];		// Bilayer thicknesses in units of bead diameters
	bool	       m_bLinearise;		// Flag showing whether polymer is initially straightened
	zDoubleVector	m_UpperFractions;	// Fractions of polymers in upper monolayer

	// Local data

	zLongVector m_PolymerTypes1;		// Types of polymer composing lamella
	zLongVector m_PolymerTypes2;		// Types of polymer composing lamella
	double m_BilayerArea;			// Area per monolayer
	double m_Bilayer1Length;		// Inplane size of bilayer: NOT necessarily x,y coordinates
	double m_Bilayer2Length;
	double m_BilayerCentre[2];		// Coordinates of bilayer midplanes (depends on orientation)
	double m_UpperHead[2];			// Coordinate of upper monolayer head beads
	double m_LowerHead[2];			// Coordinate of lower monolayer head beads

};

#endif // !defined(AFX_INITIALSTATEFUSIONPORE_H__4D165FD3_BF9A_11D4_BF58_004095086186__INCLUDED_)
