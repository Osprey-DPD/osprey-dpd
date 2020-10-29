// InclusionBead.h: interface for the CInclusionBead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INCLUSIONBEAD_H__EC62FB39_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_INCLUSIONBEAD_H__EC62FB39_3C4F_11D4_BF3C_004095086186__INCLUDED_


#include "AbstractBead.h"

class CInclusionBead : public CAbstractBead  
{
	friend class CPolymer;	// Needs to create wall beads

public:
	virtual ~CInclusionBead();

	virtual bool SetMovable();
	virtual bool SetNotMovable();
	virtual bool SetFrozen();
	virtual bool SetNotFrozen();

protected:
	CInclusionBead();


};

#endif // !defined(AFX_INCLUSIONBEAD_H__EC62FB39_3C4F_11D4_BF3C_004095086186__INCLUDED_)
