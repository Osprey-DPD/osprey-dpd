// xxConstraint.h: interface for the xxConstraint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXCONSTRAINT_H__C760D643_DD85_11D3_BF22_004095086186__INCLUDED_)
#define AFX_XXCONSTRAINT_H__C760D643_DD85_11D3_BF22_004095086186__INCLUDED_

#include "xxBase.h"

class xxConstraint : public xxBase  
{
public:
	virtual ~xxConstraint();

	inline bool IsDataValid() const {return m_bDataValid;}

	friend zOutStream& operator<<(zOutStream& os, const xxConstraint& constraint);
	friend zInStream&  operator>>(zInStream&  is,       xxConstraint& constraint);

// Protected constructor ensures that only subclasses can be instantiated.

protected:
	xxConstraint(bool dataValid);
	void SetDataValid(bool bValid);

protected:
	bool m_bDataValid;			// Flag showing whether data is valid
};

#endif // !defined(AFX_XXCONSTRAINT_H__C760D643_DD85_11D3_BF22_004095086186__INCLUDED_)
