// InitialStateRestart.h: interface for the CInitialStateRestart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATERESTART_H__0843C365_63B7_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_INITIALSTATERESTART_H__0843C365_63B7_11D3_820E_0060088AD300__INCLUDED_


#include "InitialStateData.h"

// Forward declarations

class CBuilder;


class CInitialStateRestart : public CInitialStateData  
{
public:
	CInitialStateRestart();
	virtual ~CInitialStateRestart();

	CInitialStateRestart& operator=(const CInitialStateRestart& oldISR);

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

	inline const zString GetRunId()   const {return m_RunId;}
	inline long GetStateId()          const {return m_StateId;}

	static const zString GetType();	//  Compares with keyword in control data file

private:
	static const zString m_Type;			// Keyword used to identify initial state type

private:
	zString m_RunId;
	long m_StateId;

};

#endif // !defined(AFX_INITIALSTATERESTART_H__0843C365_63B7_11D3_820E_0060088AD300__INCLUDED_)
