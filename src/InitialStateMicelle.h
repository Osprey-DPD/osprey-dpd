// InitialStateMicelle.h: interface for the CInitialStateMicelle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEMICELLE_H__E1A859B3_87BD_11D4_BF49_004095086186__INCLUDED_)
#define AFX_INITIALSTATEMICELLE_H__E1A859B3_87BD_11D4_BF49_004095086186__INCLUDED_


#include "InitialStateData.h"

class CInitialStateMicelle : public CInitialStateData  
{
public:
	CInitialStateMicelle();
	virtual ~CInitialStateMicelle();

	CInitialStateMicelle& operator=(const CInitialStateMicelle& oldISM);

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

	StringSequence m_Polymers;			// Polymers composing micelle
	double	m_XC;						// Micelle centre as fraction of SimBox thickness
	double	m_YC;
	double	m_ZC;
	double	m_Radius;					// Micelle radius in units of bead diameters

	// Local data

	zLongVector   m_PolymerTypes;		// Types of polymer composing micelle

};

#endif // !defined(AFX_INITIALSTATEMICELLE_H__E1A859B3_87BD_11D4_BF49_004095086186__INCLUDED_)
