// InitialStateMultiVesicle.h: interface for the CInitialStateMultiVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEMULTIVESICLE_H__20C9A6C3_B67D_42F8_AF37_FC6AECFAD293__INCLUDED_)
#define AFX_INITIALSTATEMULTIVESICLE_H__20C9A6C3_B67D_42F8_AF37_FC6AECFAD293__INCLUDED_


#include "InitialStateData.h"

class CInitialStateMultiVesicle : public CInitialStateData  
{
public:
	CInitialStateMultiVesicle();
	virtual ~CInitialStateMultiVesicle();

	CInitialStateMultiVesicle& operator=(const CInitialStateMultiVesicle& oldISV);

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

	StringSequence m_Polymers;			// Polymers composing vesicle
	double	m_XC;						// Vesicle centre as fraction of SimBox thickness
	double	m_YC;
	double	m_ZC;
	double	m_OuterRadius;				// Vesicle outer monolayer head radius in units of bead diameters
	double	m_Thickness;				// Vesicle bilayer thickness in units of bead radius

	// Local data

	zLongVector   m_PolymerTypes;		// Types of polymer composing micelle

	double	m_InnerRadius;				// Vesicle inner monolayer head radius in units of bead diameters


};

#endif // !defined(AFX_INITIALSTATEMULTIVESICLE_H__20C9A6C3_B67D_42F8_AF37_FC6AECFAD293__INCLUDED_)
