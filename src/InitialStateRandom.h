// InitialStateRandom.h: interface for the CInitialStateRandom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATERANDOM_H__0843C364_63B7_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INITIALSTATERANDOM_H__0843C364_63B7_11D3_820E_0060088AD300__INCLUDED_


#include "InitialStateData.h"

// Forward declarations


class CInitialStateRandom : public CInitialStateData  
{
public:

    CInitialStateRandom();
	virtual ~CInitialStateRandom();

	CInitialStateRandom& operator=(const CInitialStateRandom& oldISR);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	void SetData(const mpmInitialState *const pMsg);


	static const zString GetType();
	
    // Parallel code function to calculate how many polymers of each type must be created in each processor's space
	// in order to assemble the initial configuration
	
    virtual void CalculatePolymerFractionsP(double lx, double ly, double lz, double xorigin, double yorigin, double zorigin, double xspace, double yspace, double zspace, 
	                                     zDoubleVector& rvPolymerFractions) const;    


private:
	static const zString m_Type;			// Keyword used to identify initial state type

};

#endif // !defined(AFX_INITIALSTATERANDOM_H__0843C364_63B7_11D3_820E_0060088AD300__INCLUDED_)
